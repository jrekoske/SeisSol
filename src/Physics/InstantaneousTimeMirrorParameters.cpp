#include "Initializer/InputAux.hpp"
#include "InstantaneousTimeMirrorParameters.h"
#include <utils/logger.h>

namespace seissol::initializers::ITM {
ITMParameters readITMParametersFromYaml(std::shared_ptr<YAML::Node>& params) {
  using namespace seissol::initializers;

  const auto equationsParams = (*params)["equations"];
  const double ITMTime = getWithDefault(equationsParams, "itmtime", 1.0);
  const double ITMVelocityScalingFactor =
      getWithDefault(equationsParams, "itmvelocityscalingfactor", 1000.0);
  const double ITMStartingTime = getWithDefault(equationsParams, "itmstartingtime", 5.5);
  const bool ITMToggle = getWithDefault(equationsParams, "itmenable", 1);
  const int reflectionType = getWithDefault(equationsParams, "itmreflectiontype", 1);
  return ITMParameters(
      ITMTime, ITMVelocityScalingFactor, ITMStartingTime, ITMToggle, reflectionType);
}

ITMParameters::ITMParameters(double ITMTime,
                             double ITMVelocityScalingFactor,
                             double ITMStartingTime,
                             bool ITMToggle,
                             int reflectionType)
    : ITMTime(ITMTime), ITMVelocityScalingFactor(ITMVelocityScalingFactor),
      ITMStartingTime(ITMStartingTime), ITMToggle(ITMToggle) {
  if (ITMTime < 0.0) {
    logError() << "ITM Time is less than zero. It should be positive!\n";
  }
  if (ITMVelocityScalingFactor < 0.0) {
    logError() << "ITM Velocity Scaling Factor is less than zero. It should be positive!\n";
  }
  if (ITMStartingTime < 0.0) {
    logError() << "ITM Starting Time can not be less than zero\n";
  }

  if (reflectionType < 1 || reflectionType > 4) {
    logError() << "Reflection Type can only be 1, 2, 3 or 4\n";
  }
}

double ITMParameters::getITMTime() const { return ITMTime; }

double ITMParameters::getITMVelocityScalingFactor() const { return ITMVelocityScalingFactor; }

double ITMParameters::getITMStartingTime() const { return ITMStartingTime; }

bool ITMParameters::getITMToggle() const { return ITMToggle; }

int ITMParameters::getReflectionType() const { return reflectionType; }

} // namespace seissol::initializers::ITM