#include <cmath>
#include <array>
#include <numeric>

#ifdef USE_POROELASTIC
#include <armadillo>
#else
#include <Eigen/Eigenvalues>
#endif

#include <Kernels/precision.hpp>
#include <Physics/InitialField.h>
#include <Equations/Setup.h>
#include <Model/common.hpp>
#include <yateto/TensorView.h>
#include <utils/logger.h>
#include <Solver/Interoperability.h>

extern seissol::Interoperability e_interoperability;

seissol::physics::Planarwave::Planarwave(const CellMaterialData& materialData, double phase, std::array<double, 3> kVec)
  : m_varField{0,1,2},
    m_ampField{100.0, 100.0, 100.0},
    m_phase(phase),
    m_kVec(kVec)
{
  assert(m_varField.size() == m_ampField.size());

  std::array<std::complex<double>, NUMBER_OF_QUANTITIES*NUMBER_OF_QUANTITIES> planeWaveOperator;
  seissol::model::getPlaneWaveOperator(materialData.local, m_kVec.data(), planeWaveOperator.data());
#ifdef USE_POROELASTIC
  computeEigenvaluesWithArmadillo(planeWaveOperator);
#else
  computeEigenvaluesWithEigen3(planeWaveOperator);
#endif
}

void seissol::physics::Planarwave::computeEigenvaluesWithEigen3(std::array<std::complex<double>, NUMBER_OF_QUANTITIES*NUMBER_OF_QUANTITIES> M) {
  using Matrix = Eigen::Matrix<std::complex<double>, NUMBER_OF_QUANTITIES, NUMBER_OF_QUANTITIES, Eigen::ColMajor>;
  Matrix op(M.data());
  Eigen::ComplexEigenSolver<Matrix> ces;
  ces.compute(op);
  
  //sort eigenvalues so that we know which eigenvalue corresponds to which mode
  auto eigenvalues = ces.eigenvalues();
  std::vector<size_t> sortedIndices(NUMBER_OF_QUANTITIES);
  std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
  std::sort(sortedIndices.begin(), sortedIndices.end(), [&eigenvalues](size_t a, size_t b) {
    return eigenvalues[a].real() < eigenvalues[b].real();
  });

  for (size_t i = 0; i < NUMBER_OF_QUANTITIES; ++i) {
    m_lambdaA[i] = eigenvalues(sortedIndices[i],0);
  }

  auto eigenvectors = ces.eigenvectors();

  auto R = yateto::DenseTensorView<2,std::complex<double>>(const_cast<std::complex<double>*>(m_eigenvectors.data()), {NUMBER_OF_QUANTITIES, NUMBER_OF_QUANTITIES});
  for (size_t j = 0; j < NUMBER_OF_QUANTITIES; ++j) {
    for (size_t i = 0; i < NUMBER_OF_QUANTITIES; ++i) {
      R(i,j) = eigenvectors(i,sortedIndices[j]);
    }
  }
  
}
void seissol::physics::Planarwave::computeEigenvaluesWithArmadillo(std::array<std::complex<double>, NUMBER_OF_QUANTITIES*NUMBER_OF_QUANTITIES> M) {
#ifdef USE_POROELASTIC
  using Matrix = typename arma::Mat<std::complex<double>>::template fixed<NUMBER_OF_QUANTITIES, NUMBER_OF_QUANTITIES>;
  using Vector = typename arma::Col<std::complex<double>>::template fixed<NUMBER_OF_QUANTITIES>;

  Matrix op(M.data());
  Matrix arma_eigenvectors(arma::fill::zeros);
  Vector arma_eigenvalues(arma::fill::zeros);
  arma::eig_gen(arma_eigenvalues, arma_eigenvectors, op, "balance");
  
  //sort eigenvalues so that we know which eigenvalue corresponds to which mode
  std::array<size_t, NUMBER_OF_QUANTITIES> sortedIndices;
  std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
  std::sort(sortedIndices.begin(), sortedIndices.end(), [&arma_eigenvalues](size_t a, size_t b) {
    return arma_eigenvalues(a).real() < arma_eigenvalues(b).real();
  });

  for (size_t i = 0; i < NUMBER_OF_QUANTITIES; ++i) {
    m_lambdaA[i] = arma_eigenvalues(sortedIndices[i]);
  }

  auto R = yateto::DenseTensorView<2,std::complex<double>>(const_cast<std::complex<double>*>(m_eigenvectors.data()), {NUMBER_OF_QUANTITIES, NUMBER_OF_QUANTITIES});
  for (size_t j = 0; j < NUMBER_OF_QUANTITIES; ++j) {
    for (size_t i = 0; i < NUMBER_OF_QUANTITIES; ++i) {
      R(i,j) = arma_eigenvectors(i,sortedIndices[j]);
    }
  }
#endif
}

void seissol::physics::Planarwave::evaluate(double time,
                                            std::vector<std::array<double, 3>> const& points,
					    const CellMaterialData& materialData,
                                            yateto::DenseTensorView<2,real,unsigned>& dofsQP ) const
{
  dofsQP.setZero();

  auto R = yateto::DenseTensorView<2,std::complex<double>>(const_cast<std::complex<double>*>(m_eigenvectors.data()), {NUMBER_OF_QUANTITIES, NUMBER_OF_QUANTITIES});
  for (unsigned v = 0; v < m_varField.size(); ++v) {
    const auto omega =  m_lambdaA[m_varField[v]];
    for (unsigned j = 0; j < dofsQP.shape(1); ++j) {
      for (size_t i = 0; i < points.size(); ++i) {
        dofsQP(i,j) += (R(j, m_varField[v]) * m_ampField[v] *
                        std::exp(std::complex<double>(0.0, 1.0) * (
                          omega * time - m_kVec[0]*points[i][0] - m_kVec[1]*points[i][1] - m_kVec[2]*points[i][2] + std::complex<double>(m_phase, 0)))).real();
      }
    }
  }
}

seissol::physics::SuperimposedPlanarwave::SuperimposedPlanarwave(const CellMaterialData& materialData, real phase)
  : m_kVec({{{M_PI, 0.0, 0.0},
             {0.0, M_PI, 0.0},
             {0.0, 0.0, M_PI}}}),
    m_phase(phase),
    m_pw({Planarwave(materialData, phase, m_kVec.at(0)),
          Planarwave(materialData, phase, m_kVec.at(1)),
          Planarwave(materialData, phase, m_kVec.at(2))})
{ 
}

void seissol::physics::SuperimposedPlanarwave::evaluate( double time,
                                                        std::vector<std::array<double, 3>> const& points,
                                                        const CellMaterialData& materialData,
                                                        yateto::DenseTensorView<2,real,unsigned>& dofsQP ) const
{
  dofsQP.setZero();
 
  real dofsPW_data[tensor::dofsQP::size()];
  yateto::DenseTensorView<2,real,unsigned> dofsPW = init::dofsQP::view::create(dofsPW_data);

  for (int pw = 0; pw < 3; pw++) {
    //evaluate each planarwave
    m_pw.at(pw).evaluate(time, points, materialData, dofsPW);
    //and add results together
    for (unsigned j = 0; j < dofsQP.shape(1); ++j) {
      for (size_t i = 0; i < points.size(); ++i) {
        dofsQP(i,j) += dofsPW(i,j);
      }
    }
  }
}

seissol::physics::TravellingWave::TravellingWave(const CellMaterialData& materialData)
  //set the wave number vector such that it points in the direction of intendet travelling
  //the magnitude of the vector is the inverse of the wavelength
  : Planarwave(materialData, 0.0, {0.0, 0.0, -10}),
  //origin is a point on the wavefront at time zero
    m_origin({0.0, 0.0, -0.5})
{
}

void seissol::physics::TravellingWave::evaluate(double time,
				       	        std::vector<std::array<double, 3>> const& points,
				       	        const CellMaterialData& materialData,
				       	        yateto::DenseTensorView<2,real,unsigned>& dofsQp) const {
  dofsQp.setZero();

  auto R = yateto::DenseTensorView<2,std::complex<double>>(const_cast<std::complex<double>*>(m_eigenvectors.data()), {NUMBER_OF_QUANTITIES, NUMBER_OF_QUANTITIES});
  for (unsigned v = 0; v < m_varField.size(); ++v) {
    const auto omega =  m_lambdaA[m_varField[v]];
    for (unsigned j = 0; j < dofsQp.shape(1); ++j) {
      for (size_t i = 0; i < points.size(); ++i) {
        auto arg = std::complex<double>(0.0, 1.0) * (
                          omega * time
                        - m_kVec[0]*(points[i][0] - m_origin[0]) 
                        - m_kVec[1]*(points[i][1] - m_origin[1]) 
                        - m_kVec[2]*(points[i][2] - m_origin[2]) 
                        + m_phase);
        if(arg.imag() > -M_PI && arg.imag() < M_PI) {
          dofsQp(i,j) += (R(j,m_varField[v]) * m_ampField[v] * std::exp(arg)).real();
        }
      }
    }
  }
}

void seissol::physics::ScholteWave::evaluate(double time,
					     std::vector<std::array<double, 3>> const& points,
					     const CellMaterialData& materialData,
					     yateto::DenseTensorView<2,real,unsigned>& dofsQp) const {
#ifndef USE_ANISOTROPIC
  const real omega = 2.0 * std::acos(-1);

  for (size_t i = 0; i < points.size(); ++i) {
    const auto& x = points[i];
    const bool isAcousticPart = std::abs(materialData.local.mu) < std::numeric_limits<real>::epsilon();
    const auto x_1 = x[0];
    const auto x_3 = x[2];
    const auto t = time;
    if (isAcousticPart) {
      dofsQp(i,0) = 0.35944997730200889*std::pow(omega, 2)*std::exp(-0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // sigma_xx
      dofsQp(i,1) = 0.35944997730200889*std::pow(omega, 2)*std::exp(-0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // sigma_yy
      dofsQp(i,2) = 0.35944997730200889*std::pow(omega, 2)*std::exp(-0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // sigma_zz
      dofsQp(i,3) = 0; // sigma_xy
      dofsQp(i,4) = 0; // sigma_yz
      dofsQp(i,5) = 0; // sigma_xz
      dofsQp(i,6) = -0.50555429848461109*std::pow(omega, 2)*std::exp(-0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // u
      dofsQp(i,7) = 0; // v
      dofsQp(i,8) = 0.35550086150929727*std::pow(omega, 2)*std::exp(-0.98901344820674908*omega*x_3)*std::cos(omega*t - 1.406466352506808*omega*x_1); // w
    } else {
      dofsQp(i,0) = -2.7820282741590652*std::pow(omega, 2)*std::exp(0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1) + 3.5151973269883681*std::pow(omega, 2)*std::exp(1.2825031256883821*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // sigma_xx
      dofsQp(i,1) = -6.6613381477509402e-16*std::pow(omega, 2)*std::exp(0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1) + 0.27315475753283058*std::pow(omega, 2)*std::exp(1.2825031256883821*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // sigma_yy
      dofsQp(i,2) = 2.7820282741590621*std::pow(omega, 2)*std::exp(0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1) - 2.4225782968570462*std::pow(omega, 2)*std::exp(1.2825031256883821*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // sigma_zz
      dofsQp(i,3) = 0; // sigma_xy
      dofsQp(i,4) = 0; // sigma_yz
      dofsQp(i,5) = -2.956295201467618*std::pow(omega, 2)*std::exp(0.98901344820674908*omega*x_3)*std::cos(omega*t - 1.406466352506808*omega*x_1) + 2.9562952014676029*std::pow(omega, 2)*std::exp(1.2825031256883821*omega*x_3)*std::cos(omega*t - 1.406466352506808*omega*x_1); // sigma_xz
      dofsQp(i,6) = 0.98901344820675241*std::pow(omega, 2)*std::exp(0.98901344820674908*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1) - 1.1525489264912381*std::pow(omega, 2)*std::exp(1.2825031256883821*omega*x_3)*std::sin(omega*t - 1.406466352506808*omega*x_1); // u
      dofsQp(i,7) = 0; // v
      dofsQp(i,8) = 1.406466352506812*std::pow(omega, 2)*std::exp(0.98901344820674908*omega*x_3)*std::cos(omega*t - 1.406466352506808*omega*x_1) - 1.050965490997515*std::pow(omega, 2)*std::exp(1.2825031256883821*omega*x_3)*std::cos(omega*t - 1.406466352506808*omega*x_1); // w
    }
  }
#else
  dofsQp.setZero();
#endif
}

void seissol::physics::SnellsLaw::evaluate(double time,
					   std::vector<std::array<double, 3>> const& points,
					   const CellMaterialData& materialData,
					   yateto::DenseTensorView<2,real,unsigned>& dofsQp) const {
#ifndef USE_ANISOTROPIC
  const double pi = std::acos(-1);
  const double omega = 2.0 * pi;

  for (size_t i = 0; i < points.size(); ++i) {
    const auto &x = points[i];
    const bool isAcousticPart = std::abs(materialData.local.mu) < std::numeric_limits<real>::epsilon();

    const auto x_1 = x[0];
    const auto x_3 = x[2];
    const auto t = time;
    if (isAcousticPart) {
      dofsQp(i,0) = 1.0*omega*std::sin(omega*t - omega*(0.19866933079506119*x_1 + 0.98006657784124163*x_3)) + 0.48055591432167399*omega*std::sin(omega*t - omega*(0.19866933079506149*x_1 - 0.98006657784124152*x_3)); // sigma_xx
      dofsQp(i,1) = 1.0*omega*std::sin(omega*t - omega*(0.19866933079506119*x_1 + 0.98006657784124163*x_3)) + 0.48055591432167399*omega*std::sin(omega*t - omega*(0.19866933079506149*x_1 - 0.98006657784124152*x_3)); // sigma_yy
      dofsQp(i,2) = 1.0*omega*std::sin(omega*t - omega*(0.19866933079506119*x_1 + 0.98006657784124163*x_3)) + 0.48055591432167399*omega*std::sin(omega*t - omega*(0.19866933079506149*x_1 - 0.98006657784124152*x_3)); // sigma_zz
      dofsQp(i,3) = 0; // sigma_xy
      dofsQp(i,4) = 0; // sigma_yz
      dofsQp(i,5) = 0; // sigma_xz
      dofsQp(i,6) = -0.19866933079506119*omega*std::sin(omega*t - omega*(0.19866933079506119*x_1 + 0.98006657784124163*x_3)) - 0.095471721907895893*omega*std::sin(omega*t - omega*(0.19866933079506149*x_1 - 0.98006657784124152*x_3)); // u
      dofsQp(i,7) = 0; // v
      dofsQp(i,8) = -0.98006657784124163*omega*std::sin(omega*t - omega*(0.19866933079506119*x_1 + 0.98006657784124163*x_3)) + 0.47097679041061191*omega*std::sin(omega*t - omega*(0.19866933079506149*x_1 - 0.98006657784124152*x_3)); // w
    } else {
      dofsQp(i,0) = -0.59005639909185559*omega*std::sin(omega*t - 1.0/2.0*omega*(0.39733866159012299*x_1 + 0.91767204817721759*x_3)) + 0.55554011463785213*omega*std::sin(omega*t - 1.0/3.0*omega*(0.59600799238518454*x_1 + 0.8029785009656123*x_3)); // sigma_xx
      dofsQp(i,1) = 0.14460396298676709*omega*std::sin(omega*t - 1.0/3.0*omega*(0.59600799238518454*x_1 + 0.8029785009656123*x_3)); // sigma_yy
      dofsQp(i,2) = 0.59005639909185559*omega*std::sin(omega*t - 1.0/2.0*omega*(0.39733866159012299*x_1 + 0.91767204817721759*x_3)) + 0.89049951522981918*omega*std::sin(omega*t - 1.0/3.0*omega*(0.59600799238518454*x_1 + 0.8029785009656123*x_3)); // sigma_zz
      dofsQp(i,3) = 0; // sigma_xy
      dofsQp(i,4) = 0; // sigma_yz
      dofsQp(i,5) = -0.55363837274201066*omega*std::sin(omega*t - 1.0/2.0*omega*(0.39733866159012299*x_1 + 0.91767204817721759*x_3)) + 0.55363837274201*omega*std::sin(omega*t - 1.0/3.0*omega*(0.59600799238518454*x_1 + 0.8029785009656123*x_3)); // sigma_xz
      dofsQp(i,6) = 0.37125533967075403*omega*std::sin(omega*t - 1.0/2.0*omega*(0.39733866159012299*x_1 + 0.91767204817721759*x_3)) - 0.2585553530120539*omega*std::sin(omega*t - 1.0/3.0*omega*(0.59600799238518454*x_1 + 0.8029785009656123*x_3)); // u
      dofsQp(i,7) = 0; // v
      dofsQp(i,8) = -0.16074816713222639*omega*std::sin(omega*t - 1.0/2.0*omega*(0.39733866159012299*x_1 + 0.91767204817721759*x_3)) - 0.34834162029840349*omega*std::sin(omega*t - 1.0/3.0*omega*(0.59600799238518454*x_1 + 0.8029785009656123*x_3)); // w
    }
  }
#else
  dofsQp.setZero();
#endif
}

void seissol::physics::Ocean::evaluate(double time,
                                       std::vector<std::array<double, 3>> const& points,
                                       const CellMaterialData& materialData,
                                       yateto::DenseTensorView<2,real,unsigned>& dofsQp) const {
#ifndef USE_ANISOTROPIC
  for (size_t i = 0; i < points.size(); ++i) {
    const auto x = points[i][0];
    const auto y = points[i][1];
    const auto z = points[i][2];
    const auto t = time;

    const double g = 9.81; // m/s
    const double pi = std::acos(-1);
    assert(materialData.local.mu == 0); // has to be acoustic
    const double rho = materialData.local.rho;

    const double k_x = pi/100; // 1/m
    const double k_y = pi/100; // 1/m
    constexpr double k_star = 0.0444284459948;
    constexpr double omega = 0.276857520383318;

    const auto B = g * k_star/(omega*omega);
    const auto pressure = -std::sin(k_x*x)*std::sin(k_y*y)*std::sin(omega*t)*(std::sinh(k_star*z)
        + B * std::cosh(k_star*z));

    dofsQp(i,0) = pressure;
    dofsQp(i,1) = pressure;
    dofsQp(i,2) = pressure;
    dofsQp(i,3) = 0.0;
    dofsQp(i,4) = 0.0;
    dofsQp(i,5) = 0.0;
    dofsQp(i,6) = (k_x/(omega*rho))*cos(k_x*x)*std::sin(k_y*y)*cos(omega*t)*(std::sinh(k_star*z)
        + B * std::cosh(k_star*z));
    dofsQp(i,7) = (k_y/(omega*rho))*std::sin(k_x*x)*cos(k_y*y)*cos(omega*t)*(std::sinh(k_star*z)
        + B * std::cosh(k_star*z));
    dofsQp(i,8) = (k_star/(omega*rho))*std::sin(k_x*x)*std::sin(k_y*y)*cos(omega*t)*(std::cosh(k_star*z)
        + B * std::sinh(k_star*z));
  }
#else
  dofsQp.setZero();
#endif
}
