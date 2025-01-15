// SPDX-FileCopyrightText: 2014-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause
//
// SPDX-FileContributor: Sebastian Rettenberger

#include "SeisSol.h"

#include <climits>
#include <memory>
#include <optional>
#include <sys/resource.h>
#include <utils/logger.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

#include "Initializer/Parameters/SeisSolParameters.h"
#include "Modules/Modules.h"
#include "Monitoring/Unit.h"
#include "Parallel/Helper.h"
#include "Parallel/MPI.h"
#include "Parallel/Pin.h"

namespace seissol {

bool SeisSol::init(int argc, char* argv[]) {
  const auto rank = seissol::MPI::mpi.rank();

  if (rank == 0) {
    const auto& hostNames = seissol::MPI::mpi.getHostNames();
    logInfo() << "Running on (rank=0):" << hostNames.front();
  }

#ifdef USE_MPI
  logInfo(rank) << "Using MPI with #ranks:" << seissol::MPI::mpi.size();
  logInfo(rank) << "Node-wide (shared memory) MPI with #ranks/node:"
                << seissol::MPI::mpi.sharedMemMpiSize();
  seissol::MPI::mpi.printAcceleratorDeviceInfo();
  // TODO (Ravil, David): switch to reading MPI options from the parameter-file.
  seissol::MPI::mpi.setDataTransferModeFromEnv();

  printPersistentMpiInfo(seissol::MPI::mpi);
#endif
#ifdef ACL_DEVICE
  printUSMInfo(MPI::mpi);
  printMPIUSMInfo(MPI::mpi);
  printDeviceHostSwitch(MPI::mpi);

  if (!useUSM() && deviceHostSwitch() > 0) {
    logWarning(rank) << "Using the host-device execution on non-USM systems is not fully supported "
                        "yet. Expect incorrect results.";
  }
#endif
#ifdef _OPENMP
  pinning.checkEnvVariables();
  logInfo(rank) << "Using OMP with #threads/rank:" << omp_get_max_threads();
  if (!parallel::Pinning::areAllCpusOnline()) {
    logInfo(rank) << "Some CPUs are offline. Only online CPUs are considered.";
    logInfo(rank) << "Online Mask            (this node)   :"
                  << parallel::Pinning::maskToString(pinning.getOnlineMask());
  }
  logInfo(rank) << "OpenMP worker affinity (this process):"
                << parallel::Pinning::maskToString(
                       seissol::parallel::Pinning::getWorkerUnionMask());
  logInfo(rank) << "OpenMP worker affinity (this node)   :"
                << parallel::Pinning::maskToString(seissol::parallel::Pinning::getNodeMask());

  seissol::printCommThreadInfo(seissol::MPI::mpi);
  if (seissol::useCommThread(seissol::MPI::mpi)) {
    auto freeCpus = pinning.getFreeCPUsMask();
    logInfo(rank) << "Communication thread affinity        :"
                  << parallel::Pinning::maskToString(freeCpus);
    if (parallel::Pinning::freeCPUsMaskEmpty(freeCpus)) {
      logError()
          << "There are no free CPUs left. Make sure to leave one for the communication thread. If "
             "you want to run SeisSol without a communication thread (and instead use polling), "
             "then try running with the environment variable \"SEISSOL_COMMTHREAD=0\". ";
    }
  }
#endif // _OPENMP

  // Check if the ulimit for the stacksize is reasonable.
  // A low limit can lead to segmentation faults.
  rlimit rlim{};
  if (getrlimit(RLIMIT_STACK, &rlim) == 0) {
    const auto rlimInKb = rlim.rlim_cur / 1024;
    // Softlimit (rlim_cur) is enforced by the kernel.
    // This limit is pretty arbitrarily set to 2GiB.
    constexpr auto ReasonableStackLimitInKb = 0x200'000ULL;                    // [kiB] (2 GiB)
    constexpr auto ReasonableStackLimit = ReasonableStackLimitInKb * 0x400ULL; // [B] (2 GiB)
    if (rlim.rlim_cur == RLIM_INFINITY) {
      logInfo(rank) << "The stack size ulimit is unlimited.";
    } else {
      logInfo(rank) << "The stack size ulimit is" << rlimInKb
                    << "[kiB] ( =" << UnitByte.formatPrefix(rlim.rlim_cur).c_str() << ").";
    }
    if (rlim.rlim_cur < ReasonableStackLimit) {
      logWarning(rank)
          << "Stack size of" << rlimInKb
          << "[kiB] ( =" << UnitByte.formatPrefix(rlim.rlim_cur).c_str()
          << ") is lower than recommended minimum of" << ReasonableStackLimitInKb
          << "[kiB] ( =" << UnitByte.formatPrefix(ReasonableStackLimit).c_str() << ")."
          << "You can increase the stack size by running the command: ulimit -Ss unlimited.";
    }
  } else {
    logError() << "Stack size cannot be determined because getrlimit syscall failed!";
  }

  // Call post MPI initialization hooks
  seissol::Modules::callHook<ModuleHook::PostMPIInit>();

  // Initialize the ASYNC I/O library
  if (!m_asyncIO.init()) {
    return false;
  }

  m_memoryManager->initialize();

  m_memoryManager->setInputParams(
      std::make_shared<seissol::initializer::parameters::SeisSolParameters>(m_seissolParameters));

  return true;
}

void SeisSol::finalize() {
  // Cleanup ASYNC I/O library
  m_asyncIO.finalize();

  Modules::callHook<ModuleHook::Shutdown>();

  const int rank = seissol::MPI::mpi.rank();

  m_timeManager.freeDynamicResources();

  seissol::MPI::finalize();

  logInfo(rank) << "SeisSol done. Goodbye.";
}

void SeisSol::setBackupTimeStamp(const std::string& stamp) {
  m_backupTimeStamp = stamp;
  seissol::MPI::mpi.broadcastContainer(m_backupTimeStamp, 0);
}

void SeisSol::loadCheckpoint(const std::string& file) {
  checkpointLoadFile = std::make_optional<std::string>(file);
}

} // namespace seissol
