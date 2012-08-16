#include "dmcompilersettings.h"

#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include "dmmodule.h"
#include "dm.h"
using namespace DM;
class DM_HELPER_DLL_EXPORT RoadNetwork : public  DM::Module {
DM_DECLARE_NODE( RoadNetwork )

    struct Parameter {
        long Width;
        long Height;
        double CellSize;

        double roadDistanceToRiver_in;
        double roadBasisLength_in;
        double dAlpha_in;
        double roadConnectivity_in;

        double roadDistanceToRiver;
        double roadBasisLength;
        double dAlpha;
        double roadConnectivity;

        RasterData * topoStatus;
        RasterData * RoadNetworkRa;
    };

public:
    RoadNetwork();
    void run();
private:
    Parameter param;
    long findPointNearRiver(long x, long offset, RasterData * rData);
    std::vector<Node> findWayPoints(Node startPoint, Node endPoint, RasterData * topoStatus);
    long findNearestPoint(const Node  p, const std::vector<Node> &  points);
    DM::System * city;

    DM::View vTopoStatus;
    DM::View vRoadNetworkRa;

    DM::View vCityCenters;
    DM::View vRoadNetworkVec;

};

#endif // ROADNETWORK_H
