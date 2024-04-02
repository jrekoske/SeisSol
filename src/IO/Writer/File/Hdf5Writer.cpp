#include "Hdf5Writer.hpp"

#include <H5Ppublic.h>
#include <IO/Datatype/HDF5Type.hpp>
#include <IO/Datatype/Inference.hpp>
#include <IO/Datatype/MPIType.hpp>
#include <hdf5.h>
#include <stack>
#include <string>
#include <yaml-cpp/yaml.h>

#include "utils/logger.h"

namespace {
static hid_t _eh(hid_t data) {
  if (data < 0) {
    logError() << "HDF5 error:" << data;
  }
  return data;
}
} // namespace

namespace seissol::io::writer::file {

Hdf5File::Hdf5File(MPI_Comm comm) : comm(comm) {}

void Hdf5File::openFile(const std::string& name) {
  hid_t h5falist = _eh(H5Pcreate(H5P_FILE_ACCESS));
#ifdef H5F_LIBVER_V18
  _eh(H5Pset_libver_bounds(h5falist, H5F_LIBVER_V18, H5F_LIBVER_V18));
#else
  _eh(H5Pset_libver_bounds(h5falist, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST));
#endif
  _eh(H5Pset_fapl_mpio(h5falist, comm, MPI_INFO_NULL));
  file = _eh(H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, h5falist));
  _eh(H5Pclose(h5falist));

  handles.push(file);
}
void Hdf5File::openGroup(const std::string& name) {
  // cf. https://stackoverflow.com/a/18468735
  auto existenceTest = _eh(H5Lexists(handles.top(), name.c_str(), H5P_DEFAULT));
  hid_t handle;
  if (existenceTest > 0) {
    handle = _eh(H5Gopen(handles.top(), name.c_str(), H5P_DEFAULT));
  } else {
    handle = _eh(H5Gcreate(handles.top(), name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));
  }
  handles.push(handle);
}
void Hdf5File::writeAttribute(const async::ExecInfo& info,
                              const std::string& name,
                              std::shared_ptr<DataSource> source) {
  // TODO: store datatype separately
  if (source->distributed()) {
    logError() << "Attempted to write an HDF5 attributed as a distributed source.";
  }
  hid_t h5space;
  if (source->shape().empty()) {
    h5space = _eh(H5Screate(H5S_SCALAR));
  } else {
    auto& shape = source->shape();
    std::vector<hsize_t> hshape(shape.begin(), shape.end());
    h5space = _eh(H5Screate_simple(shape.size(), hshape.data(), nullptr));
  }
  hid_t h5type = datatype::convertToHdf5(source->datatype());
  hid_t handle =
      _eh(H5Acreate(handles.top(), name.c_str(), h5type, h5space, H5P_DEFAULT, H5P_DEFAULT));
  _eh(H5Awrite(handle, h5type, source->getPointer(info)));
  _eh(H5Aclose(handle));
  _eh(H5Sclose(h5space));
}
void Hdf5File::writeData(const async::ExecInfo& info,
                         const std::string& name,
                         std::shared_ptr<DataSource> source,
                         std::shared_ptr<datatype::Datatype> targetType,
                         int compress) {
  MPI_Datatype sizetype = datatype::convertToMPI(datatype::inferDatatype<std::size_t>());

  std::size_t trueCount = source->count(info);
  std::size_t dimprod = 1;
  for (auto dimension : source->shape()) {
    assert(trueCount % dimension == 0);
    trueCount /= dimension;
    dimprod *= dimension;
  }

  int rank = 0;
  MPI_Comm_rank(comm, &rank);
  // if we don't write distributed data, only one rank needs to do the work
  const std::size_t count = (source->distributed() || rank == 0) ? trueCount : 0;
  const auto& dimensions = source->shape();
  // TODO: adjust chunksize according to dimensions and datatype size
  const std::size_t chunksize = std::max(std::size_t(1), std::size_t(2'000'000'000) / (source->datatype()->size() * dimprod));

  const std::size_t actualDimensions =
      source->distributed() ? dimensions.size() + 1 : dimensions.size();

  std::size_t allcount = count;
  std::size_t offset = 0;

  std::size_t rounds = (count + chunksize - 1) / chunksize;

  MPI_Allreduce(MPI_IN_PLACE, &rounds, 1, sizetype, MPI_MAX, comm);
  MPI_Allreduce(&count, &allcount, 1, sizetype, MPI_SUM, comm);
  MPI_Exscan(&count, &offset, 1, sizetype, MPI_SUM, comm);

  std::vector<hsize_t> globalSizes;
  std::vector<hsize_t> localSizes;

  std::size_t chunkcount = std::min(chunksize, count);

  if (source->distributed()) {
    globalSizes.push_back(allcount);
    localSizes.push_back(chunkcount);
  }
  for (const auto& dim : dimensions) {
    globalSizes.push_back(dim);
    localSizes.push_back(dim);
  }
  hid_t h5space = _eh(H5Screate_simple(globalSizes.size(), globalSizes.data(), nullptr));
  hid_t h5memspace = _eh(H5Screate_simple(localSizes.size(), localSizes.data(), nullptr));

  std::vector<hsize_t> writeStart;
  std::vector<hsize_t> writeLength;

  if (source->distributed()) {
    writeStart.push_back(offset);
    writeLength.push_back(chunkcount);
  }
  for (const auto& dim : dimensions) {
    writeStart.push_back(0);
    writeLength.push_back(dim);
  }

  hid_t h5dxlist = H5Pcreate(H5P_DATASET_XFER);
  _eh(h5dxlist);
  _eh(H5Pset_dxpl_mpio(h5dxlist, H5FD_MPIO_COLLECTIVE));

  hid_t h5memtype = datatype::convertToHdf5(source->datatype());

  hid_t h5type = datatype::convertToHdf5(targetType);
  h5type = _eh(H5Tcopy(h5type));
  _eh(H5Tcommit(handles.top(),
                (name + std::string("_Type")).c_str(),
                h5type,
                H5P_DEFAULT,
                H5P_DEFAULT,
                H5P_DEFAULT));

  hid_t h5filter = H5P_DEFAULT;
  if (compress > 0) {
    h5filter = _eh(H5Pcreate(H5P_DATASET_CREATE));
    _eh(H5Pset_chunk(h5filter, actualDimensions, writeLength.data()));
    int deflateStrength = compress;
    _eh(H5Pset_deflate(h5filter, deflateStrength));
  }

  hid_t h5data =
      H5Dcreate(handles.top(), name.c_str(), h5type, h5space, H5P_DEFAULT, h5filter, H5P_DEFAULT);

  std::size_t written = 0;

  std::vector<hsize_t> nullstart(actualDimensions);

  const char* data = reinterpret_cast<const char*>(source->getPointer(info));

  for (std::size_t i = 0; i < rounds; ++i) {
    if (source->distributed()) {
      writeStart[0] = offset + written;
      writeLength[0] = std::min(count - written, chunkcount);
    }

    const char* dataloc = data;

    _eh(H5Sselect_hyperslab(
        h5memspace, H5S_SELECT_SET, nullstart.data(), nullptr, writeLength.data(), nullptr));

    _eh(H5Sselect_hyperslab(
        h5space, H5S_SELECT_SET, writeStart.data(), nullptr, writeLength.data(), nullptr));

    _eh(H5Dwrite(h5data, h5memtype, h5memspace, h5space, h5dxlist, dataloc));

    if (source->distributed()) {
      written += writeLength[0];
    }
  }

  if (compress > 0) {
    _eh(H5Pclose(h5filter));
  }
  _eh(H5Tclose(h5type));
  _eh(H5Sclose(h5space));
  _eh(H5Sclose(h5memspace));
  _eh(H5Dclose(h5data));
  _eh(H5Pclose(h5dxlist));
}
void Hdf5File::closeGroup() {
  _eh(H5Gclose(handles.top()));
  handles.pop();
}
void Hdf5File::closeFile() { _eh(H5Fclose(file)); }

Hdf5Writer::Hdf5Writer(MPI_Comm comm) : comm(comm) {}

void Hdf5Writer::writeAttribute(const async::ExecInfo& info,
                                instructions::Hdf5AttributeWrite write) {
  Hdf5File file(comm);
  if (openFiles.find(write.location.file()) == openFiles.end()) {
    file.openFile(write.location.file());
    openFiles.insert({write.location.file(), file});
  }
  file = openFiles.at(write.location.file());
  for (auto groupname : write.location.groups()) {
    file.openGroup(groupname);
  }
  file.writeAttribute(info, write.name, write.dataSource);
  for (auto _ : write.location.groups()) {
    file.closeGroup();
  }
}

void Hdf5Writer::writeData(const async::ExecInfo& info, instructions::Hdf5DataWrite write) {
  Hdf5File file(comm);
  if (openFiles.find(write.location.file()) == openFiles.end()) {
    file.openFile(write.location.file());
    openFiles.insert({write.location.file(), file});
  }
  file = openFiles.at(write.location.file());
  for (auto groupname : write.location.groups()) {
    file.openGroup(groupname);
  }
  file.writeData(info, write.name, write.dataSource, write.targetType, 0);
  for (auto _ : write.location.groups()) {
    file.closeGroup();
  }
}

void Hdf5Writer::finalize() {
  for (auto [_, file] : openFiles) {
    file.closeFile();
  }
}

} // namespace seissol::io::writer::file
