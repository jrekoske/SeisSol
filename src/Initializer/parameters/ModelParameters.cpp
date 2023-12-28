#include "ModelParameters.h"

namespace seissol::initializers::parameters {

ModelParameters readModelParameters(ParameterReader& baseReader) {
  auto reader = baseReader.readSubNode("equations");

  const std::string boundaryFileName = reader.readWithDefault("boundaryfileName", std::string(""));
  const std::string materialFileName =
      reader.readOrFail<std::string>("materialfileName", "No material file given.");
  const bool hasBoundaryFile = boundaryFileName != "";

  const bool plasticity = reader.readWithDefault("plasticity", false);
  const bool useCellHomogenizedMaterial =
      reader.readWithDefault("usecellhomogenizedmaterial", true);

  const double gravitationalAcceleration =
      reader.readWithDefault("gravitationalacceleration", 9.81);
  const double tv = reader.readWithDefault("tv", 0.1);

  auto readFreqCentral = [&reader]() {
    double freqCentral;
    if (isModelViscoelastic()) {
      freqCentral = reader.readOrFail<double>(
          "freqcentral", "equations.freqcentral is needed for the attenuation fitting.");
    } else {
      reader.markUnused("freqcentral");
    }
    return freqCentral;
  };

  auto readFreqRatio = [&reader]() {
    double freqRatio;
    if (isModelViscoelastic()) {
      freqRatio = reader.readOrFail<double>(
          "freqratio", "equations.freqratio is needed for the attenuation fitting.");
      if (freqRatio <= 0) {
        logError()
            << "The freqratio parameter must be positive---but that is currently not the case.";
      }
    } else {
      reader.markUnused("freqratio");
    }
    return freqRatio;
  };

  const double freqCentral = readFreqCentral();
  const double freqRatio = readFreqRatio();

  reader.warnDeprecated({"adjoint", "adjfileName", "anisotropy"});
  reader.warnUnknown();
  return ModelParameters{hasBoundaryFile,
                         plasticity,
                         useCellHomogenizedMaterial,
                         freqCentral,
                         freqRatio,
                         gravitationalAcceleration,
                         tv,
                         boundaryFileName,
                         materialFileName};
}
} // namespace seissol::initializers::parameters
