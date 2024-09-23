// SPDX-FileCopyrightText: 2015-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 */


#ifndef SEISSOL_SRC_EQUATIONS_ELASTIC_KERNELS_INTERFACE_H_
#define SEISSOL_SRC_EQUATIONS_ELASTIC_KERNELS_INTERFACE_H_

#include "Initializer/Tree/InterfaceHelper.h"
#include "Initializer/LTS.h"
#include "Equations/elastic/Kernels/GravitationalFreeSurfaceBC.h"

namespace seissol::kernels {
    struct LocalTmp {
        alignas(Alignment) std::array<real, tensor::averageNormalDisplacement::size()> nodalAvgDisplacements[4];
        GravitationalFreeSurfaceBc gravitationalFreeSurfaceBc;
        LocalTmp(double graviationalAcceleration) : gravitationalFreeSurfaceBc(graviationalAcceleration) {};
    };
#ifndef ACL_DEVICE
    LTSTREE_GENERATE_INTERFACE_GETTERED(LocalData, initializer::LTS, cellInformation, localIntegration, neighboringIntegration, dofs, faceDisplacements, boundaryMapping, material)
    LTSTREE_GENERATE_INTERFACE_GETTERED(NeighborData, initializer::LTS, cellInformation, neighboringIntegration, dofs)
#else
    LTSTREE_GENERATE_INTERFACE_GETTERED(LocalData, initializer::LTS, cellInformation, localIntegration, neighboringIntegration, dofs, faceDisplacements, faceDisplacementsDevice, plasticity, boundaryMapping, material)
    LTSTREE_GENERATE_INTERFACE_GETTERED(NeighborData, initializer::LTS, cellInformation, neighboringIntegration, dofs)
#endif
  }




#endif // SEISSOL_SRC_EQUATIONS_ELASTIC_KERNELS_INTERFACE_H_

