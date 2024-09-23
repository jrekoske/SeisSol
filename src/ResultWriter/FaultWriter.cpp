// SPDX-FileCopyrightText: 2017-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Sebastian Rettenberger (sebastian.rettenberger AT tum.de, http://www5.in.tum.de/wiki/index.php/Sebastian_Rettenberger)
 *
 */

#include "Parallel/MPI.h"

#include <algorithm>
#include <cassert>
#include <cstring>

#include "FaultWriter.h"
#include "AsyncCellIDs.h"
#include "SeisSol.h"
#include "Modules/Modules.h"
#include "DynamicRupture/Output/OutputManager.h"

void seissol::writer::FaultWriter::setUp()
{
  setExecutor(m_executor);

  if (isAffinityNecessary()) {
    const auto freeCpus = seissolInstance.getPinning().getFreeCPUsMask();
    logInfo(seissol::MPI::mpi.rank()) << "Fault writer thread affinity:"
      << parallel::Pinning::maskToString(freeCpus);
    if (parallel::Pinning::freeCPUsMaskEmpty(freeCpus)) {
      logError() << "There are no free CPUs left. Make sure to leave one for the I/O thread(s).";
    }
    setAffinityIfNecessary(freeCpus);
  }
}

void seissol::writer::FaultWriter::init(const unsigned int* cells, const double* vertices,
	const unsigned int* faultTags,
	unsigned int nCells, unsigned int nVertices,
	int* outputMask, const real** dataBuffer,
	const char* outputPrefix,
	double interval,
	xdmfwriter::BackendType backend,
	const std::string& backupTimeStamp)
{
	const int rank = seissol::MPI::mpi.rank();

	logInfo(rank) << "Initializing XDMF fault output.";

	// Initialize the asynchronous module
	async::Module<FaultWriterExecutor, FaultInitParam, FaultParam>::init();

	m_enabled = true;

	FaultInitParam param;
	param.timestep = m_timestep;
	param.backend = backend;
	param.backupTimeStamp = backupTimeStamp;

	// Create buffer for output prefix
	unsigned int bufferId = addSyncBuffer(outputPrefix, strlen(outputPrefix)+1, true);
	assert(bufferId == FaultWriterExecutor::OUTPUT_PREFIX); NDBG_UNUSED(bufferId);

	AsyncCellIDs<3> cellIds(nCells, nVertices, cells, seissolInstance);

	// Create mesh buffers
	bufferId = addSyncBuffer(cellIds.cells(), nCells * 3 * sizeof(int));
	assert(bufferId == FaultWriterExecutor::CELLS);
	bufferId = addSyncBuffer(vertices, nVertices * 3 * sizeof(double));
	assert(bufferId == FaultWriterExecutor::VERTICES);

	bufferId = addSyncBuffer(faultTags, nCells * sizeof(unsigned int));
	assert(bufferId == FaultWriterExecutor::FAULTTAGS);

	// Create data buffers
	std::fill_n(param.outputMask, FaultInitParam::OUTPUT_MASK_SIZE, false);
	if (outputMask[0]) {
		param.outputMask[0] = true;
		param.outputMask[1] = true;
	}
	if (outputMask[1]) {
		param.outputMask[2] = true;
		param.outputMask[3] = true;
		param.outputMask[4] = true;
	}
	if (outputMask[2])
		param.outputMask[5] = true;
	if (outputMask[3]) {
		param.outputMask[6] = true;
		param.outputMask[7] = true;
	}
	if (outputMask[4]) {
		param.outputMask[8] = true;
		param.outputMask[9] = true;
		param.outputMask[10] = true;
	}
	if (outputMask[5]) {
		param.outputMask[11] = true;
		param.outputMask[12] = true;
	}
	if (outputMask[6])
		param.outputMask[13] = true;
	if (outputMask[7])
		param.outputMask[14] = true;
	if (outputMask[8])
		param.outputMask[15] = true;
	if (outputMask[9])
		param.outputMask[16] = true;
	if (outputMask[10])
		param.outputMask[17] = true;
        if (outputMask[11]) {
                param.outputMask[18] = true;
                param.outputMask[19] = true;
        }
	for (unsigned int i = 0; i < FaultInitParam::OUTPUT_MASK_SIZE; i++) {
		if (param.outputMask[i]) {
			addBuffer(dataBuffer[m_numVariables++], nCells * sizeof(real));
		}
	}

	//
	// Send all buffers for initialization
	//
	sendBuffer(FaultWriterExecutor::OUTPUT_PREFIX);

	sendBuffer(FaultWriterExecutor::CELLS);
	sendBuffer(FaultWriterExecutor::VERTICES);
	sendBuffer(FaultWriterExecutor::FAULTTAGS);

	// Initialize the executor
	callInit(param);

	// Remove unused buffers
	removeBuffer(FaultWriterExecutor::OUTPUT_PREFIX);
	removeBuffer(FaultWriterExecutor::CELLS);
	removeBuffer(FaultWriterExecutor::VERTICES);
	removeBuffer(FaultWriterExecutor::FAULTTAGS);

	// Register for the synchronization point hook
	Modules::registerHook(*this, ModuleHook::SimulationStart);
  Modules::registerHook(*this, ModuleHook::SynchronizationPoint);
	setSyncInterval(interval);
}

void seissol::writer::FaultWriter::simulationStart()
{
	syncPoint(0.0);
}

void seissol::writer::FaultWriter::syncPoint(double currentTime)
{
	SCOREP_USER_REGION("faultoutput_elementwise", SCOREP_USER_REGION_TYPE_FUNCTION)

	if (callbackObject) {
		callbackObject->updateElementwiseOutput();
	}
	write(currentTime);
}

