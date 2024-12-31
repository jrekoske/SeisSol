# SPDX-FileCopyrightText: 2024 SeisSol Group
#
# SPDX-License-Identifier: BSD-3-Clause

import numpy as np
from yateto import Tensor
from yateto.input import parseJSONMatrixFile
from kernels.multsim import OptionalDimTensor
from kernels.common import generate_kernel_name_prefix
from yateto import Tensor, Scalar, simpleParameterSpace


def addKernels(generator, aderdg, matricesDir, targets=["cpu"]):
    for target in targets:
        name_prefix = generate_kernel_name_prefix(targets)

        vtkbase = parseJSONMatrixFile(f"{matricesDir}/vtkbase.json")
        vtko = parseJSONMatrixFile(f"{matricesDir}/vtko{aderdg.order}.json")

        # the following is due to a shortcut in Yateto where 1-column matrices are interpreted as rank-1 vectors

        maxOrder = 8
        rangeLimit = maxOrder + 1

        qb = Tensor("qb", (aderdg.numberOf3DBasisFunctions(),))
        pb = Tensor("pb", (aderdg.numberOf2DBasisFunctions(),))
        pn = Tensor("pn", (aderdg.numberOf2DBasisFunctions(),))
        xv = [
            Tensor(f"xv({i})", (((i + 1) * (i + 2) * (i + 3)) // 6,))
            for i in range(rangeLimit)
        ]
        xf = [
            Tensor(f"xf({i})", (((i + 1) * (i + 2)) // 2,)) for i in range(rangeLimit)
        ]

        generator.addFamily(
            f"{name_prefix}projectBasisToVtkVolume",
            simpleParameterSpace(rangeLimit),
            lambda i: xv[i]["p"]
            <= vtko.byName(f"collvv({aderdg.order},{i})")["pb"] * qb["b"],
            target=target,
        )
        generator.addFamily(
            f"{name_prefix}projectBasisToVtkFace",
            simpleParameterSpace(rangeLimit),
            lambda i: xf[i]["p"]
            <= vtko.byName(f"collff({aderdg.order},{i})")["pb"] * pb["b"],
            target=target,
        )
        generator.addFamily(
            f"{name_prefix}projectNodalToVtkFace",
            simpleParameterSpace(rangeLimit),
            lambda i: xf[i]["p"]
            <= vtko.byName(f"collff({aderdg.order},{i})")["pb"]
            * aderdg.db.MV2nTo2m["bm"]
            * pn["m"],
            target=target,
        )

        generator.addFamily(
            f"{name_prefix}projectBasisToVtkFaceFromVolume",
            simpleParameterSpace(rangeLimit, 4),
            lambda i, j: xf[i]["p"]
            <= vtko.byName(f"collvf({aderdg.order},{i},{j})")["pb"] * qb["b"],
            target=target,
        )


def includeTensors(matricesDir, includeTensors):
    vtkbase = parseJSONMatrixFile(f"{matricesDir}/vtkbase.json")
    for x in vtkbase.__dict__:
        if isinstance(vtkbase.__dict__[x], dict):
            for y in vtkbase.__dict__[x]:
                includeTensors.add(vtkbase.__dict__[x][y])
        else:
            includeTensors.add(vtkbase.__dict__[x])
