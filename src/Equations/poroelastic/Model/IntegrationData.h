// SPDX-FileCopyrightText: 2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SEISSOL_SRC_EQUATIONS_POROELASTIC_MODEL_INTEGRATIONDATA_H_
#define SEISSOL_SRC_EQUATIONS_POROELASTIC_MODEL_INTEGRATIONDATA_H_

#include "Common/Constants.h"
#include "Datastructures.h"
#include "Kernels/Precision.h"
#include "generated_code/tensor.h"

namespace seissol::model {

struct PoroelasticLocalData {
  real sourceMatrix[seissol::tensor::ET::size()];
  real G[PoroElasticMaterial::NumQuantities];
  real typicalTimeStepWidth;
  real Zinv[PoroElasticMaterial::NumQuantities][ConvergenceOrder * ConvergenceOrder];
};
struct PoroelasticNeighborData {};

} // namespace seissol::model

#endif // SEISSOL_SRC_EQUATIONS_POROELASTIC_MODEL_INTEGRATIONDATA_H_
