// SPDX-FileCopyrightText: 2013-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

/******************************************************************************
** Copyright (c) 2014-2015, Intel Corporation                                **
** All rights reserved.                                                      **
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions        **
** are met:                                                                  **
** 1. Redistributions of source code must retain the above copyright         **
**    notice, this list of conditions and the following disclaimer.          **
** 2. Redistributions in binary form must reproduce the above copyright      **
**    notice, this list of conditions and the following disclaimer in the    **
**    documentation and/or other materials provided with the distribution.   **
** 3. Neither the name of the copyright holder nor the names of its          **
**    contributors may be used to endorse or promote products derived        **
**    from this software without specific prior written permission.          **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  **
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
******************************************************************************/
/* Alexander Heinecke (Intel Corp.)
******************************************************************************/
/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Alexander Breuer (breuer AT mytum.de, http://www5.in.tum.de/wiki/index.php/Dipl.-Math._Alexander_Breuer)
 *
 */


#ifndef SEISSOL_SRC_EQUATIONS_ELASTIC_KERNELS_TIMEBASE_H_
#define SEISSOL_SRC_EQUATIONS_ELASTIC_KERNELS_TIMEBASE_H_

#include "Common/Constants.h"
#include "generated_code/kernel.h"

#ifdef ACL_DEVICE
#include <device.h>
#endif // ACL_DEVICE

namespace seissol {
  struct GlobalData;
} // namespace seissol

namespace seissol::kernels {

class TimeBase {
  protected:
    static void checkGlobalData(GlobalData const* global, size_t alignment);
#ifdef USE_STP
    kernel::spaceTimePredictor m_krnlPrototype;
#else
    kernel::derivative m_krnlPrototype;
#endif
    kernel::projectDerivativeToNodalBoundaryRotated projectDerivativeToNodalBoundaryRotated;


  /*
   *! Offsets of the derivatives.
   *
   * * Offset counting starts at the zeroth derivative with o_derivativesOffset[0]=0; increasing derivatives follow:
   *   1st derivative: o_derivativesOffset[1]
   *   2nd derivative: o_derivativesOffset[2]
   *   ...
   * * Offset are always counted from position zero; for example the sixth derivative will include all jumps over prior derivatives 0 to 5.
   */
  unsigned int m_derivativesOffsets[ConvergenceOrder];

#ifdef ACL_DEVICE
    kernel::gpu_derivative deviceKrnlPrototype;
    kernel::gpu_projectDerivativeToNodalBoundaryRotated deviceDerivativeToNodalBoundaryRotated;
    device::DeviceInstance& device = device::DeviceInstance::getInstance();
#endif

public:
    /**
     * Constructor, which initializes the time kernel.
     **/
    TimeBase();

};

} // namespace seissol::kernels




#endif // SEISSOL_SRC_EQUATIONS_ELASTIC_KERNELS_TIMEBASE_H_

