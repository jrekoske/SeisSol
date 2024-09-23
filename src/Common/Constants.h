// SPDX-FileCopyrightText: 2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SEISSOL_SRC_COMMON_CONSTANTS_H_
#define SEISSOL_SRC_COMMON_CONSTANTS_H_

#include <cstddef>

namespace seissol {
// TODO: remove these, once properly templated
constexpr std::size_t ConvergenceOrder = CONVERGENCE_ORDER;

constexpr std::size_t Alignment = ALIGNMENT;
constexpr std::size_t Vectorsize = VECTORSIZE;

constexpr std::size_t PagesizeHeap = 2097152;
constexpr std::size_t PagesizeStack = 4096;

constexpr std::size_t zeroLengthArrayHandler(std::size_t x) { return x == 0 ? 1 : x; }
} // namespace seissol


#endif // SEISSOL_SRC_COMMON_CONSTANTS_H_

