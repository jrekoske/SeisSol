// SPDX-FileCopyrightText: 2016-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause
//
// SPDX-FileContributor: Sebastian Rettenberger

#ifndef SEISSOL_SRC_READER_ASAGIREADER_H_
#define SEISSOL_SRC_READER_ASAGIREADER_H_

#ifdef USE_ASAGI

#include "Parallel/MPI.h"
#include "easi/util/AsagiReader.h"
#include <asagi.h>

#include "utils/env.h"
#include "utils/logger.h"

#include "AsagiModule.h"
#include "Monitoring/Instrumentation.h"

namespace seissol::asagi {
enum class NumaCacheMode { Off, On, Cache };

class AsagiReader : public easi::AsagiReader {
  private:
  /** Prefix for environment variables */
  const std::string envPrefix;

  /** Number of threads used by ASAGI */
  unsigned int asagiThreads;

#ifdef USE_MPI
  /** MPI communicator used by ASAGI */
  MPI_Comm comm;
#endif

  public:
  AsagiReader(const char* envPrefix
#ifdef USE_MPI
              ,
              MPI_Comm comm = seissol::MPI::mpi.comm()
#endif
  );

  virtual ::asagi::Grid* open(const char* file, const char* varname);
  virtual unsigned numberOfThreads() const;

  private:
  static NumaCacheMode getNumaMode();
};

} // namespace seissol::asagi

#endif

#endif // SEISSOL_SRC_READER_ASAGIREADER_H_
