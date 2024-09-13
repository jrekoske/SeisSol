#include "Allocator.hpp"
#include <Solver/time_stepping/MiniSeisSol.h>

namespace seissol::proxy {

ProxyData::ProxyData(std::size_t cellCount, bool enableDR) : cellCount(cellCount) {
  initGlobalData();
  initDataStructures(enableDR);
  initDataStructuresOnDevice(enableDR);
}

void ProxyData::initGlobalData() {
  seissol::initializer::GlobalDataInitializerOnHost::init(
      globalDataOnHost, allocator, seissol::memory::Standard);

  CompoundGlobalData globalData{};
  globalData.onHost = &globalDataOnHost;
  globalData.onDevice = nullptr;
  if constexpr (seissol::isDeviceOn()) {
    seissol::initializer::GlobalDataInitializerOnDevice::init(
        globalDataOnDevice, allocator, seissol::memory::DeviceGlobalMemory);
    globalData.onDevice = &globalDataOnDevice;
  }
  timeKernel.setGlobalData(globalData);
  localKernel.setGlobalData(globalData);
  neighborKernel.setGlobalData(globalData);
  dynRupKernel.setGlobalData(globalData);

  const double timeStepWidth = static_cast<double>(seissol::miniSeisSolTimeStep);
  dynRupKernel.setTimeStepWidth(timeStepWidth);
}

void ProxyData::initDataStructures(bool enableDR) {
  // init RNG
  srand48(cellCount);
  lts.addTo(ltsTree, false); // proxy does not use plasticity
  ltsTree.setNumberOfTimeClusters(1);
  ltsTree.fixate();

  seissol::initializer::TimeCluster& cluster = ltsTree.child(0);
  cluster.child<Ghost>().setNumberOfCells(0);
  cluster.child<Copy>().setNumberOfCells(0);
  cluster.child<Interior>().setNumberOfCells(cellCount);

  seissol::initializer::Layer& layer = cluster.child<Interior>();
  layer.setBucketSize(lts.buffersDerivatives,
                      sizeof(real) * tensor::I::size() * layer.getNumberOfCells());

  ltsTree.allocateVariables();
  ltsTree.touchVariables();
  ltsTree.allocateBuckets();

  if (enableDR) {
    dynRup.addTo(dynRupTree);
    dynRupTree.setNumberOfTimeClusters(1);
    dynRupTree.fixate();

    seissol::initializer::TimeCluster& cluster = dynRupTree.child(0);
    cluster.child<Ghost>().setNumberOfCells(0);
    cluster.child<Copy>().setNumberOfCells(0);
    cluster.child<Interior>().setNumberOfCells(
        4 * cellCount); /// Every face is a potential dynamic rupture face

    dynRupTree.allocateVariables();
    dynRupTree.touchVariables();

    fakeDerivativesHost = (real*)allocator.allocateMemory(
        cellCount * yateto::computeFamilySize<tensor::dQ>() * sizeof(real),
        PagesizeHeap,
        seissol::memory::Standard);
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
    for (unsigned cell = 0; cell < cellCount; ++cell) {
      for (unsigned i = 0; i < yateto::computeFamilySize<tensor::dQ>(); i++) {
        fakeDerivativesHost[cell * yateto::computeFamilySize<tensor::dQ>() + i] = (real)drand48();
      }
    }

#ifdef ACL_DEVICE
    fakeDerivatives = (real*)allocator.allocateMemory(
        cellCount * yateto::computeFamilySize<tensor::dQ>() * sizeof(real),
        PagesizeHeap,
        seissol::memory::DeviceGlobalMemory);
    const auto& device = ::device::DeviceInstance::getInstance();
    device.api->copyTo(fakeDerivatives,
                       fakeDerivativesHost,
                       cellCount * yateto::computeFamilySize<tensor::dQ>() * sizeof(real));
#else
    fakeDerivatives = fakeDerivativesHost;
#endif
  }

  /* cell information and integration data*/
  seissol::fakeData(lts, layer, (enableDR) ? FaceType::DynamicRupture : FaceType::Regular);

  if (enableDR) {
    // From lts tree
    CellDRMapping(*drMapping)[4] = ltsTree.var(lts.drMapping);

    // From dynamic rupture tree
    seissol::initializer::Layer& interior = dynRupTree.child(0).child<Interior>();
    real(*imposedStatePlus)[seissol::tensor::QInterpolated::size()] =
        interior.var(dynRup.imposedStatePlus);
    real(*fluxSolverPlus)[seissol::tensor::fluxSolver::size()] =
        interior.var(dynRup.fluxSolverPlus);
    real** timeDerivativePlus = interior.var(dynRup.timeDerivativePlus);
    real** timeDerivativeMinus = interior.var(dynRup.timeDerivativeMinus);
    DRFaceInformation* faceInformation = interior.var(dynRup.faceInformation);

    /* init drMapping */
    for (unsigned cell = 0; cell < cellCount; ++cell) {
      for (unsigned face = 0; face < 4; ++face) {
        CellDRMapping& drm = drMapping[cell][face];
        unsigned side = (unsigned int)lrand48() % 4;
        unsigned orientation = (unsigned int)lrand48() % 3;
        unsigned drFace = (unsigned int)lrand48() % interior.getNumberOfCells();
        drm.side = side;
        drm.faceRelation = orientation;
        drm.godunov = imposedStatePlus[drFace];
        drm.fluxSolver = fluxSolverPlus[drFace];
      }
    }

    /* init dr godunov state */
    for (unsigned face = 0; face < interior.getNumberOfCells(); ++face) {
      unsigned plusCell = (unsigned int)lrand48() % cellCount;
      unsigned minusCell = (unsigned int)lrand48() % cellCount;
      timeDerivativePlus[face] =
          &fakeDerivatives[plusCell * yateto::computeFamilySize<tensor::dQ>()];
      timeDerivativeMinus[face] =
          &fakeDerivatives[minusCell * yateto::computeFamilySize<tensor::dQ>()];

      faceInformation[face].plusSide = (unsigned int)lrand48() % 4;
      faceInformation[face].minusSide = (unsigned int)lrand48() % 4;
      faceInformation[face].faceRelation = (unsigned int)lrand48() % 3;
    }
  }
}

void ProxyData::initDataStructuresOnDevice(bool enableDR) {
#ifdef ACL_DEVICE
  const auto& device = ::device::DeviceInstance::getInstance();
  ltsTree.synchronizeTo(seissol::initializer::AllocationPlace::Device,
                        device.api->getDefaultStream());
  device.api->syncDefaultStreamWithHost();

  seissol::initializer::TimeCluster& cluster = ltsTree.child(0);
  seissol::initializer::Layer& layer = cluster.child<Interior>();

  seissol::initializer::MemoryManager::deriveRequiredScratchpadMemoryForWp(ltsTree, lts);
  ltsTree.allocateScratchPads();

  seissol::initializer::recording::CompositeRecorder<seissol::initializer::LTS> recorder;
  recorder.addRecorder(new seissol::initializer::recording::LocalIntegrationRecorder);
  recorder.addRecorder(new seissol::initializer::recording::NeighIntegrationRecorder);

  recorder.addRecorder(new seissol::initializer::recording::PlasticityRecorder);
  recorder.record(lts, layer);
  if (enableDR) {
    seissol::initializer::MemoryManager::deriveRequiredScratchpadMemoryForDr(dynRupTree, dynRup);
    dynRupTree.allocateScratchPads();

    CompositeRecorder<seissol::initializer::DynamicRupture> drRecorder;
    drRecorder.addRecorder(new DynamicRuptureRecorder);

    auto& drLayer = dynRupTree.child(0).child<Interior>();
    drRecorder.record(dynRup, drLayer);
  }
#endif // ACL_DEVICE
}

} // namespace seissol::proxy
