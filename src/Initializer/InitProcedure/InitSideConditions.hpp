#ifndef INITPROC_SIDECONDITIONS_H
#define INITPROC_SIDECONDITIONS_H

#include "Initializer/InitProcedure/Init.hpp"

namespace seissol {
  class SeisSol;
  namespace initializers::initprocedure {
void initSideConditions(seissol::SeisSol& seissolInstance);
}
}

#endif
