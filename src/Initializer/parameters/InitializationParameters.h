#ifndef SEISSOL_INITIALIZATION_PARAMETERS_H
#define SEISSOL_INITIALIZATION_PARAMETERS_H

#include <Eigen/Dense>

#include "Initializer/InputAux.hpp"
#include "ParameterReader.h"

namespace seissol::initializers::parameters {

enum class InitializationType : int {
  Zero,
  Planarwave,
  SuperimposedPlanarwave,
  Travelling,
  Scholte,
  Snell,
  Ocean0,
  Ocean1,
  Ocean2,
  PressureInjection
};

struct InitializationParameters {
  InitializationType type;
  Eigen::Vector3d origin;
  Eigen::Vector3d kVec;
  Eigen::Vector<double, NUMBER_OF_QUANTITIES> ampField;
  double magnitude;
  double width;
};

InitializationParameters readInitializationParameters(ParameterReader& baseReader);
} // namespace seissol::initializers::parameters

#endif
