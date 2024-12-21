// SPDX-FileCopyrightText: 2015-2024 SeisSol Group
// SPDX-FileCopyrightText: 2023 Intel Corporation
//
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de,
 *http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 **/

#ifndef SEISSOL_SRC_SOURCETERM_NRF_H_
#define SEISSOL_SRC_SOURCETERM_NRF_H_

#include <Eigen/Dense>
#include <array>
#include <cstddef>
#include <vector>

namespace seissol::sourceterm {

// (NOTE: naming also used in rconv; hence avoid clang-tidy styleguiding here)

// NOLINTNEXTLINE
typedef struct Subfault_units {
  char* tinit;
  char* timestep;
  char* mu;
  char* area;
  char* tan1;
  char* tan2;
  char* normal;
  // NOLINTNEXTLINE
} Subfault_units;

using Subfault = struct Subfault {
  double tinit{};
  double timestep{};
  double mu{};
  double area{};
  Eigen::Vector3d tan1;
  Eigen::Vector3d tan2;
  Eigen::Vector3d normal;
};

using Offsets = std::array<unsigned, 3U>;

struct NRF {
  std::vector<Eigen::Vector3d> centres;
  std::vector<Subfault> subfaults;
  std::vector<Offsets> sroffsets;
  std::array<std::vector<double>, 3U> sliprates;
  [[nodiscard]] std::size_t size() const { return centres.size(); }
};
} // namespace seissol::sourceterm

#endif // SEISSOL_SRC_SOURCETERM_NRF_H_
