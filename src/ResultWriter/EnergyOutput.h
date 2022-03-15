/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de,
 * http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 * @section LICENSE
 * Copyright (c) 2018, SeisSol Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 */

#ifndef ENERGYOUTPUT_H
#define ENERGYOUTPUT_H

#include <Initializer/typedefs.hpp>
#include <Initializer/DynamicRupture.h>
#include <Initializer/tree/LTSTree.hpp>
#include <Geometry/MeshReader.h>
#include <Initializer/LTS.h>
#include <Initializer/tree/Lut.hpp>

namespace seissol {
namespace writer {

real computePlasticMoment(MeshReader const& i_meshReader,
                          seissol::initializers::LTSTree* i_ltsTree,
                          seissol::initializers::LTS* i_lts,
                          seissol::initializers::Lut* i_ltsLut);

real computeStaticWork(GlobalData const* global,
                       real* degreesOfFreedomPlus,
                       real* degreesOfFreedomMinus,
                       DRFaceInformation const& faceInfo,
                       DRGodunovData const& godunovData,
                       real slip[seissol::tensor::slipInterpolated::size()]);

void printEnergies(GlobalData const* global,
                   seissol::initializers::DynamicRupture* dynRup,
                   seissol::initializers::LTSTree* dynRupTree,
                   MeshReader const& i_meshReader,
                   seissol::initializers::LTSTree* i_ltsTree,
                   seissol::initializers::LTS* i_lts,
                   seissol::initializers::Lut* i_ltsLut,
                   bool usePlasticity);

} // namespace writer
} // namespace seissol

#endif // ENERGYOUTPUT_H
