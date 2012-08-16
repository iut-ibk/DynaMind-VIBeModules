#include "dmcompilersettings.h"

#ifndef INITIALCITY_H
#define INITIALCITY_H

#include "dmmodule.h"
#include "dm.h"
using namespace DM;
class DM_HELPER_DLL_EXPORT InitialCityCenters : public  Module {

 DM_DECLARE_NODE( InitialCityCenters)

    struct Parameter {
        unsigned long Width;
        unsigned long Height;
        double CellSize;
        double MenanderWidth;
        int numberCenters;

    };

 RasterData * topoStatus;
private:
    Parameter param;
    DM::View vTopoStatus;
    DM::View vVectorDataOut;
public:
    InitialCityCenters();
    void run();


};

#endif // INITIALCITY_H
