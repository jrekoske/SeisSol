// SPDX-FileCopyrightText: 2021-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause
// SPDX-LicenseComments: Full text under /LICENSE and /LICENSES/

#ifndef SEISSOL_SRC_TESTS_MODEL_VALUES_H_
#define SEISSOL_SRC_TESTS_MODEL_VALUES_H_

#include "Kernels/Precision.h"
#include <Equations/Datastructures.h>
#include <Model/Datastructures.h>
#include <array>

namespace seissol::unit_test {

template <typename T>
struct SolutionData;

struct ElasticSolutionData {
  const inline static std::array<std::array<real, 9>, 9> SolutionHomogeneousLocal = {
      {{0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -8.100000000000001e6,
        0.0000000000000000,
        0.0000000000000000},
       {-0.1666862222222223,
        1.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -2.700316800000001e6,
        0.0000000000000000,
        0.0000000000000000},
       {-0.1666862222222223,
        0.0000000000000000,
        1.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -2.700316800000001e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -4.676399999999999e6,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -4.676399999999999e6},
       {-3.086419753086419e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -5.345992643914123e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -5.345992643914123e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000}}};
  const inline static std::array<std::array<real, 9>, 9> SolutionHomogeneousNeighbor = {
      {{0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        8.100000000000001e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.1666862222222223,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        2.700316800000001e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.1666862222222223,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        2.700316800000001e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        4.676399999999999e6,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        4.676399999999999e6},
       {3.086419753086419e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.5000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        5.345992643914123e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.4999999999999999,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        5.345992643914123e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.4999999999999999}}};

  const inline static std::array<std::array<real, 9>, 9> SolutionBoundary = {
      {{0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {-6.172839506172840e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1.000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -1.069198528782824e-7,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1.000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -1.069198528782824e-7,
        0.0000000000000000,
        0.0000000000000000,
        1.000000000000000}}};
  const inline static std::array<std::array<real, 9>, 9> SolutionHeterogeneousLocal = {
      {{1 - 0.6090225563909775,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -6.333834586466166e6,
        0.0000000000000000,
        0.0000000000000000},
       {-0.2030313383458647,
        1.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -2.111525918796993e6,
        0.0000000000000000,
        0.0000000000000000},
       {-0.2030313383458647,
        0.0000000000000000,
        1.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -2.111525918796993e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1 - 0.6426804463745808,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -3.341938321147820e6,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1 - 0.6426804463745808,
        0.0000000000000000,
        0.0000000000000000,
        -3.341938321147820e6},
       {-3.759398496240601e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1 - 0.3909774436090225,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -6.871529877411907e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        1 - 0.3573195536254192,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        -6.871529877411907e-8,
        0.0000000000000000,
        0.0000000000000000,
        1 - 0.3573195536254192}}};
  const inline static std::array<std::array<real, 9>, 9> SolutionHeterogeneousNeighbor = {
      {{0.6090225563909775,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        6.333834586466166e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.2030313383458647,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        2.111525918796993e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.2030313383458647,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        2.111525918796993e6,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.6426804463745808,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        3.341938321147820e6,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.6426804463745808,
        0.0000000000000000,
        0.0000000000000000,
        3.341938321147820e6},
       {3.759398496240601e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.3909774436090225,
        0.0000000000000000,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        6.871529877411907e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.3573195536254192,
        0.0000000000000000},
       {0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        0.0000000000000000,
        6.871529877411907e-8,
        0.0000000000000000,
        0.0000000000000000,
        0.3573195536254192}}};
};

template <>
struct SolutionData<model::ViscoElasticMaterial> : public ElasticSolutionData {
  static auto zeroExtend(std::vector<double> input) -> std::vector<double> {
    constexpr auto TargetSize = model::ViscoElasticMaterial::Mechanisms * 4 + 3;
    input.reserve(TargetSize);
    while (input.size() < TargetSize) {
      input.emplace_back(0);
    }
    return input;
  }

  const inline static std::vector<double> MaterialVal1 =
      zeroExtend({2700, 3.23980992e10, 3.24038016e10});
  const inline static std::vector<double> MaterialVal2 = zeroExtend({2600, 1.04e10, 2.08e10});
};

template <>
struct SolutionData<model::AnisotropicMaterial> : public ElasticSolutionData {
  const inline static std::vector<double> MaterialVal1{
      2700,        97200000000, 32403801600, 32403801600, 0,           0,          0, 97200000000,
      32403801600, 0,           0,           0,           97200000000, 0,          0, 0,
      32398099200, 0,           0,           32398099200, 0,           32398099200};
  const inline static std::vector<double> MaterialVal2{
      2600.0,      41600000000, 20800000000, 20800000000, 0,           0,          0, 41600000000,
      20800000000, 0,           0,           0,           41600000000, 0,          0, 0,
      10400000000, 0,           0,           10400000000, 0,           10400000000};
};

template <>
struct SolutionData<model::ElasticMaterial> : public ElasticSolutionData {
  const inline static std::vector<double> MaterialVal1{2700, 3.23980992e10, 3.24038016e10};
  const inline static std::vector<double> MaterialVal2{2600, 1.04e10, 2.08e10};
};

template <>
struct SolutionData<model::PoroElasticMaterial> {
  const inline static std::vector<double> MaterialVal1{
      40.0e9,  // bulk_solid
      2.5e3,   // rho
      12.0e9,  // lambda
      10.0e9,  // mu
      0.2,     // porosity
      6.0e-13, // permeability
      3,       // tortuosity
      2.5e9,   // bulk_fluid
      1.04e3,  // rho_fluid
      0.001    // viscosity
  };
  const inline static std::vector<double> MaterialVal2{
      30.0e9,  // bulk_solid
      2.6e3,   // rho
      10.0e9,  // lambda
      10.0e9,  // mu
      0.3,     // porosity
      1.0e-13, // permeability
      3,       // tortuosity
      2.5e9,   // bulk_fluid
      1.04e3,  // rho_fluid
      0.001    // viscosity
  };

  const inline static std::array<std::array<real, 13>, 13> SolutionHomogeneousLocal = {
      {{
           0.5000000000000002,
           0,
           0,
           8.463503994287163e-29,
           0,
           2.47137926931649e-19,
           -4389525.111360244,
           3.502662072688081e-08,
           -5.040896905451785e-28,
           -2.629247685461062e-19,
           -977639.0235531013,
           0,
           0,
       },
       {
           0.1874999999999996,
           0,
           0,
           1.857149960071388e-14,
           0,
           -1.281006677446092e-19,
           -1971951.591277787,
           -2.470180925222847e-07,
           6.7731628768218e-13,
           -0.1666666666666665,
           -2526316.414737303,
           0,
           0,
       },
       {
           0.1874999999999996,
           0,
           0,
           1.857149960071388e-14,
           0,
           -1.281006677446092e-19,
           -1971951.591277787,
           -2.470180925222847e-07,
           6.7731628768218e-13,
           -0.1666666666666665,
           -2526316.414737303,
           0,
           0,
       },
       {
           1.690040124521934e-39,
           0,
           0,
           0.4999999999999999,
           0,
           2.010188582532744e-24,
           5.183803965100016e-33,
           -2312286.026136616,
           9.296261938579674e-18,
           7.939164680194708e-39,
           -3.947862542976744e-32,
           0,
           0,
       },
       {
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
       },
       {
           1.540633583851713e-17,
           0,
           0,
           -5.174753011974996e-11,
           0,
           0.5000000000000001,
           4.521303319907433e-10,
           0.0007228895474719477,
           -2312286.026136617,
           -9.088078303319519e-16,
           -3.140767709564498e-09,
           0,
           0,
       },
       {
           -5.893435192085569e-08,
           0,
           0,
           -2.903355523706452e-21,
           0,
           3.494498766609407e-41,
           0.4999999999999998,
           5.77605767839056e-29,
           -1.53050622585841e-19,
           -8.892666381852574e-09,
           -6.130456187116809e-17,
           0,
           0,
       },
       {
           -3.654478955931039e-46,
           0,
           0,
           -1.081181121946673e-07,
           0,
           -4.346755893974288e-31,
           -1.120926197387689e-39,
           0.5000000000000001,
           -2.010188582532745e-24,
           -1.716734995250462e-45,
           8.536708907013681e-39,
           0,
           0,
       },
       {
           -2.36025331238427e-23,
           0,
           0,
           -3.269213327541938e-17,
           0,
           -1.081181121946672e-07,
           -4.134923132662101e-16,
           5.300776569049673e-11,
           0.4999999999999999,
           -5.597202296524324e-23,
           -2.55302477176394e-15,
           0,
           0,
       },
       {
           -3.738084804849114e-17,
           0,
           0,
           -7.468712372530655e-28,
           0,
           2.229512207345478e-19,
           977639.0235531009,
           5.228035118178036e-07,
           -2.051488213397387e-27,
           0.4999999999999998,
           6479105.342714675,
           0,
           0,
       },
       {
           8.892666381852569e-09,
           0,
           0,
           9.162215964347138e-21,
           0,
           -3.945551500257116e-41,
           4.446672540737802e-17,
           -2.887994428328849e-28,
           1.026615895636786e-19,
           3.992739799627148e-08,
           0.5000000000000001,
           0,
           0,
       },
       {
           2.436319303954024e-47,
           0,
           0,
           7.207874146311145e-09,
           0,
           2.897837262649523e-32,
           7.472841315917922e-41,
           -0.03333333333333331,
           1.340125721688495e-25,
           1.144489996833641e-46,
           -5.691139271342449e-40,
           0,
           0,
       },
       {
           -8.255190824165578e-24,
           0,
           0,
           -1.318371214781458e-18,
           0,
           7.207874146311152e-09,
           -1.55923770334338e-17,
           1.297332470276869e-11,
           -0.03333333333333334,
           -6.247145429617549e-23,
           -6.684519384975977e-16,
           0,
           0,
       }}};
  const inline static std::array<std::array<real, 13>, 13> SolutionHomogeneousNeighbor = {
      {{0.4999999999999999,
        0,
        0,
        -1.190412747560393e-28,
        0,
        -2.471379269316491e-19,
        4389525.111360244,
        -3.502662072688083e-08,
        1.067882579733609e-28,
        -1.209287653370315e-17,
        977639.0235531017,
        0,
        0},
       {0.1874999999999994,
        0,
        0,
        1.857149960071381e-14,
        0,
        1.281006677446091e-19,
        1971951.591277787,
        2.470180925222866e-07,
        6.773162876821786e-13,
        -0.1666666666666668,
        2526316.414737305,
        0,
        0},
       {0.1874999999999994,
        0,
        0,
        1.857149960071381e-14,
        0,
        1.281006677446091e-19,
        1971951.591277787,
        2.470180925222866e-07,
        6.773162876821786e-13,
        -0.1666666666666668,
        2526316.414737305,
        0,
        0},
       {-1.243467537591766e-39,
        0,
        0,
        0.5,
        0,
        -3.07326262582929e-24,
        -1.280945852505192e-32,
        2312286.026136616,
        -1.26360242080726e-17,
        -6.415874452864988e-39,
        5.410042456020009e-32,
        0,
        0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {-1.540633583851699e-17,
        0,
        0,
        4.726277772806496e-11,
        0,
        0.4999999999999999,
        -4.521303319907422e-10,
        -0.0007652772627346411,
        2312286.026136617,
        9.088078303319519e-16,
        3.140767709564501e-09,
        0,
        0},
       {5.893435192085568e-08,
        0,
        0,
        2.903355523706447e-21,
        0,
        -1.28395568175428e-41,
        0.5000000000000001,
        -6.949216162666939e-29,
        1.530506225858411e-19,
        8.892666381852574e-09,
        7.924780480128654e-17,
        0,
        0},
       {3.500557027482573e-46,
        0,
        0,
        1.081181121946673e-07,
        0,
        -1.436817387442287e-31,
        -1.075414263565625e-39,
        0.4999999999999998,
        -3.649207327275915e-25,
        1.392131478663452e-45,
        3.060517628796396e-39,
        0,
        0},
       {2.360253312384269e-23,
        0,
        0,
        2.557453487920571e-17,
        0,
        1.081181121946672e-07,
        4.134923132662101e-16,
        -9.242904997144632e-11,
        0.5000000000000001,
        5.597202296524327e-23,
        2.55302477176394e-15,
        0,
        0},
       {6.16633970232134e-18,
        0,
        0,
        7.508938948745366e-28,
        0,
        -2.229512207345478e-19,
        -977639.023553101,
        -5.228035118178036e-07,
        2.195248889781828e-27,
        0.5000000000000002,
        -6479105.342714677,
        0,
        0},
       {-8.892666381852569e-09,
        0,
        0,
        -9.162215964347144e-21,
        0,
        5.278609428394355e-41,
        -5.055177051866323e-17,
        2.535797660378048e-28,
        -1.026615895636785e-19,
        -3.992739799627149e-08,
        0.4999999999999998,
        0,
        0},
       {9.964692137551865e-47,
        0,
        0,
        -7.207874146311148e-09,
        0,
        1.556768126006534e-31,
        4.470652145717153e-40,
        -0.03333333333333331,
        6.487911606181175e-25,
        4.547840280438291e-46,
        -2.853795996276277e-39,
        0,
        0},
       {1.850774688176051e-24,
        0,
        0,
        -7.551397928722263e-19,
        0,
        -7.207874146311147e-09,
        -2.101790969054809e-17,
        9.320177767830649e-12,
        -0.03333333333333333,
        8.294266125822062e-24,
        -3.193382370190986e-16,
        0,
        0}}};

  const inline static std::array<std::array<real, 13>, 13> SolutionBoundary = {
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {-1.178687038417114e-07,
        0,
        0,
        -5.806711047412886e-21,
        0,
        -6.619017753679851e-26,
        1,
        0,
        0,
        -1.778533276370514e-08,
        0,
        0,
        0},
       {-8.291991583867109e-46,
        0,
        0,
        -2.162362243893345e-07,
        0,
        -1.003692972097634e-30,
        0,
        1,
        0,
        -4.746389553592061e-45,
        0,
        0,
        0},
       {-5.386788203447544e-23,
        0,
        0,
        -4.85667037956646e-17,
        0,
        -2.162362243893345e-07,
        0,
        0,
        1,
        2.810903019224021e-22,
        0,
        0,
        0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {1.778533276370514e-08,
        0,
        0,
        1.832443192869417e-20,
        0,
        4.439830903411459e-26,
        0,
        0,
        0,
        7.985479599254296e-08,
        1,
        0,
        0},
       {-1.826372134990954e-46,
        0,
        0,
        1.44157482926223e-08,
        0,
        -2.219962747904799e-31,
        0,
        0,
        0,
        -3.297977528930888e-46,
        0,
        1,
        0},
       {-3.257361657232767e-24,
        0,
        0,
        1.453770086626451e-18,
        0,
        1.44157482926223e-08,
        0,
        0,
        0,
        -4.279082210850334e-23,
        0,
        0,
        1}}};
  const inline static std::array<std::array<real, 13>, 13> SolutionHeterogeneousLocal = {
      {{0.4822638562418708,
        0,
        0,
        4.376881085591646e-13,
        0,
        -1.383582263695924e-19,
        -4218177.988236742,
        -1.981972046482653e-06,
        1.758708384163152e-12,
        0.00799918269252934,
        -839304.8363814871,
        0,
        0},
       {0.1620177620399895,
        0,
        0,
        7.622285640764506e-13,
        0,
        -9.628274367308288e-19,
        -1654742.24389565,
        -3.637898968600934e-06,
        4.577930714727024e-12,
        -0.1188476361802999,
        -1856842.482447654,
        0,
        0},
       {0.1620177620399895,
        0,
        0,
        7.622285640764506e-13,
        0,
        -9.628274367308288e-19,
        -1654742.24389565,
        -3.637898968600934e-06,
        4.577930714727024e-12,
        -0.1188476361802999,
        -1856842.482447654,
        0,
        0},
       {2.023722669581749e-39,
        0,
        0,
        0.4933588285670036,
        0,
        2.702489290021727e-24,
        7.828799717040293e-34,
        -2281573.450333226,
        1.217020676879377e-17,
        6.349274611405449e-39,
        -1.055575311342912e-31,
        0,
        0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {8.092970374528155e-17,
        0,
        0,
        6.576943563501973e-11,
        0,
        0.4933588285670035,
        1.74031450124452e-09,
        0.0002639856538783131,
        -2281573.450333225,
        6.027239219374464e-17,
        9.173558402085076e-09,
        0,
        0},
       {-6.110070900206422e-08,
        0,
        0,
        5.283851210365958e-20,
        0,
        -4.754876799209161e-26,
        0.5177361437581293,
        -2.586198288042453e-13,
        6.053771028814364e-20,
        -9.548539269110054e-09,
        -0.00426310861286184,
        0,
        0},
       {-4.493833954031427e-46,
        0,
        0,
        -1.095541740308607e-07,
        0,
        -6.001088150292786e-31,
        -1.738446009255723e-40,
        0.5066411714329965,
        -2.702489290021726e-24,
        -1.409905925405305e-45,
        2.343987269822128e-38,
        0,
        0},
       {-9.191097451002951e-24,
        0,
        0,
        -1.2717194289099e-17,
        0,
        -1.095541740308607e-07,
        -8.682572713504101e-17,
        -6.68955586269571e-11,
        0.5066411714329965,
        1.792763731756867e-22,
        4.423056679653573e-16,
        0,
        0},
       {-0.004263108612861052,
        0,
        0,
        2.356267944925192e-13,
        0,
        1.289830182549986e-19,
        839304.8363814934,
        -6.611579907737894e-07,
        2.39454859162168e-13,
        0.4101098526376077,
        5322625.437331663,
        0,
        0},
       {9.548539269109883e-09,
        0,
        0,
        -1.731644660382059e-20,
        0,
        1.465484578601191e-26,
        0.00799918269252799,
        1.298552062280834e-13,
        5.089717035744783e-20,
        4.696328924961065e-08,
        0.5898901473623921,
        0,
        0},
       {4.493833954031431e-47,
        0,
        0,
        1.095541740308608e-08,
        0,
        6.001088150292791e-32,
        1.738446009255724e-41,
        -0.05066411714329968,
        2.702489290021728e-25,
        1.409905925405306e-46,
        -2.343987269822129e-39,
        0,
        0},
       {6.644236898289751e-24,
        0,
        0,
        1.571263828747854e-18,
        0,
        1.095541740308608e-08,
        8.231399295854594e-17,
        5.577139613322033e-12,
        -0.05066411714329966,
        4.543552437583499e-23,
        7.656484166819926e-16,
        0,
        0}}};
  const inline static std::array<std::array<real, 13>, 13> SolutionHeterogeneousNeighbor = {
      {{0.5177361437581292,
        0,
        0,
        -4.376881085591663e-13,
        0,
        1.383582263695936e-19,
        4218177.988236742,
        1.981972046482662e-06,
        -1.758708384163145e-12,
        -0.007999182692528884,
        839304.836381482,
        0,
        0},
       {0.1927300177050109,
        0,
        0,
        -6.677260325290082e-14,
        0,
        2.432843272162704e-19,
        1861585.024382605,
        6.07598001350127e-07,
        8.86225828466746e-14,
        -0.1996297426304958,
        2088947.792753609,
        0,
        0},
       {0.1927300177050109,
        0,
        0,
        -6.677260325290082e-14,
        0,
        2.432843272162704e-19,
        1861585.024382605,
        6.07598001350127e-07,
        8.86225828466746e-14,
        -0.1996297426304958,
        2088947.792753609,
        0,
        0},
       {-2.378502977243562e-42,
        0,
        0,
        0.5066411714329965,
        0,
        -1.520972811158703e-24,
        -1.300352693558004e-33,
        2281573.450333226,
        -6.940112449375517e-18,
        6.707084413491403e-39,
        6.285480165808244e-32,
        0,
        0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {-8.092970374528168e-17,
        0,
        0,
        -1.300779762538393e-11,
        0,
        0.5066411714329964,
        -1.74031450124452e-09,
        -5.410309607605456e-05,
        2281573.450333225,
        -6.027239219374512e-17,
        -9.173558402085066e-09,
        0,
        0},
       {6.11007090020642e-08,
        0,
        0,
        -5.283851210365961e-20,
        0,
        4.75487679920915e-26,
        0.4822638562418706,
        2.58619828804244e-13,
        -6.053771028814379e-20,
        9.548539269109781e-09,
        0.004263108612859719,
        0,
        0},
       {5.831531270554741e-46,
        0,
        0,
        1.095541740308607e-07,
        0,
        2.313707541094819e-31,
        -8.253151747131145e-41,
        0.4933588285670035,
        1.777130470503441e-24,
        3.150441391086121e-45,
        -1.44713121581263e-38,
        0,
        0},
       {9.191097451002919e-24,
        0,
        0,
        1.916376311515397e-17,
        0,
        1.095541740308607e-07,
        8.682572713504101e-17,
        6.610357556092326e-11,
        0.4933588285670035,
        -1.79276373175687e-22,
        -4.423056679653564e-16,
        0,
        0},
       {0.004263108612861053,
        0,
        0,
        -2.356267944925189e-13,
        0,
        -1.289830182549988e-19,
        -839304.8363814934,
        6.611579907737879e-07,
        -2.394548591621691e-13,
        0.5898901473623923,
        -5322625.437331662,
        0,
        0},
       {-9.548539269109882e-09,
        0,
        0,
        1.731644660382059e-20,
        0,
        -1.46548457860119e-26,
        -0.007999182692527994,
        -1.298552062280833e-13,
        -5.089717035744778e-20,
        -4.696328924961063e-08,
        0.4101098526376077,
        0,
        0},
       {8.122521461359644e-47,
        0,
        0,
        -7.303611602057378e-09,
        0,
        1.229230784647851e-31,
        4.375744160630138e-41,
        -0.03289058857113356,
        6.141185128024143e-25,
        1.451854820793332e-46,
        -4.73713701648469e-39,
        0,
        0},
       {3.03553802411192e-24,
        0,
        0,
        -4.994745233244326e-19,
        0,
        -7.303611602057379e-09,
        2.913523093800152e-18,
        -2.603135533709323e-12,
        -0.03289058857113355,
        2.608469976321034e-23,
        -9.278188997416607e-17,
        0,
        0}}};
};

template <>
struct SolutionData<model::AcousticMaterial> : public ElasticSolutionData {
  const inline static std::vector<double> MaterialVal1{2700, 3.24038016e10};
  const inline static std::vector<double> MaterialVal2{2600, 2.08e10};

  // TODO: add solutions here
};

} // namespace seissol::unit_test

#endif // SEISSOL_SRC_TESTS_MODEL_VALUES_H_
