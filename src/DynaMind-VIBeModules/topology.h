

#ifndef TOPOLOGY_H
#define TOPOLOGY_H
#include "dmcompilersettings.h"
#include "dmmodule.h"
#include "dm.h"
#include <QMap>
#include <QVariant>
#include <QString>
using namespace DM;
class DM_HELPER_DLL_EXPORT Topology : public  Module {
DM_DECLARE_NODE( Topology)

    struct Parameter {
        long width;
        long height;
        double cellSize;
        double RiverSlope;
        double RiverWidth;
        double ElevationBelow;
        double SectionBasisLength;
        double MenanderWidth;
        long MinorRivers;
        long NumberRiverSinuisities;
        double BasicElevation;
        double BasicSlope;
        double MaximalDHeight;
        double MaximalHillHeight;
        long MaximalNoOfHills;


        RasterData *topoStatus;
        RasterData *topoElevation;
        RasterData *topoGradient;


    };


public:
    Topology();
    virtual ~Topology();
    void run();
    Parameter param;

private:
    DM::View vtopoStatus;
    DM::View vtopoElevation;
    DM::View vtopoGradient;

};

#endif // TOPOLOGY_H

