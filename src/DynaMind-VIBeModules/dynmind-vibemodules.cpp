//#include "../core/moduleregistry.h"
#include "topology.h"
#include "initialcitycenters.h"
#include "roadnetwork.h"
#include "dmnodefactory.h"
#include "dmmoduleregistry.h"

#include <iostream>

using namespace std;


extern "C" void DM_HELPER_DLL_EXPORT  registerModules(ModuleRegistry *registry) {
    registry->addNodeFactory(new NodeFactory<Topology>());
    registry->addNodeFactory(new NodeFactory<InitialCityCenters>());
    registry->addNodeFactory(new NodeFactory<RoadNetwork>());



}


