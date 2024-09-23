// SPDX-FileCopyrightText: 2015-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de,
 *http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 */

#ifndef SEISSOL_SRC_INITIALIZER_POINTMAPPER_H_
#define SEISSOL_SRC_INITIALIZER_POINTMAPPER_H_

#include "Geometry/MeshReader.h"
#include <Eigen/Dense>

namespace seissol::initializer {
/** Finds the tetrahedrons that contain the points.
 *  In "contained" we save if the point source is contained in the mesh.
 *  We use short here as bool. For MPI use cleanDoubles afterwards.
 */
void findMeshIds(const Eigen::Vector3d* points,
                 const seissol::geometry::MeshReader& mesh,
                 std::size_t numPoints,
                 short* contained,
                 unsigned* meshId);

void findMeshIds(const Eigen::Vector3d* points,
                 const std::vector<Vertex>& vertices,
                 const std::vector<Element>& elements,
                 std::size_t numPoints,
                 short* contained,
                 unsigned* meshIds);
#ifdef USE_MPI
void cleanDoubles(short* contained, std::size_t numPoints);
#endif
} // namespace seissol::initializer

#endif // SEISSOL_SRC_INITIALIZER_POINTMAPPER_H_
