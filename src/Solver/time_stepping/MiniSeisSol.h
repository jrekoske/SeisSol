// SPDX-FileCopyrightText: 2017-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 * 
 */

#ifndef SEISSOL_SRC_SOLVER_TIME_STEPPING_MINISEISSOL_H_
#define SEISSOL_SRC_SOLVER_TIME_STEPPING_MINISEISSOL_H_

#include "Initializer/MemoryManager.h"

namespace seissol {
  void localIntegration(GlobalData* globalData,
                        initializer::LTS& lts,
                        initializer::Layer& layer,
                        seissol::SeisSol& seissolInstance);

  void localIntegrationOnDevice(CompoundGlobalData& globalData,
                                initializer::LTS& lts,
                                initializer::Layer& layer,
                                seissol::SeisSol& seissolInstance,
                                seissol::parallel::runtime::StreamRuntime& runtime);
  
  void fillWithStuff(real* buffer,
                     unsigned nValues);

  void fakeData(initializer::LTS& lts,
                initializer::Layer& layer,
                FaceType faceTp = FaceType::Regular);
  
  double miniSeisSol(initializer::MemoryManager& memoryManager,
                     bool usePlasticity,
                     seissol::SeisSol& seissolInstance);
  constexpr real miniSeisSolTimeStep = 1.0;
} //namespace seissol


#endif // SEISSOL_SRC_SOLVER_TIME_STEPPING_MINISEISSOL_H_

