#include "dmcompilersettings.h"

#ifndef POPULATIONGROWTHMODEL_H
#define POPULATIONGROWTHMODEL_H

#include "dmmodule.h"
using namespace DM;
class DM_HELPER_DLL_EXPORT PopulationGrowthModel : public  Module {
DM_DECLARE_NODE( PopulationGrowthModel )
        struct Parameter {
    long Width;
    long Height;
    double CellSize;
    double MenanderWidth;
    double maxCPopDensity;
    double maxDCPopDensity;
    double maxOBPopDensity;
    double popChangeLow_in;
    double popChangeLow;
    double popCUFRand;
    double popDCUFRand;
    double popAGRIRand;
    double popCUFRand_in;
    double popDCUFRand_in;
    double popAGRIRand_in;
    double roadInfluence;
    int populationSteps;


    RasterData * PopConnectivity_in;
    RasterData * Population_in;
    RasterData * LandUse;
    RasterData * Population_out;
    RasterData * PopConnectivity_out;
    RasterData * RoadConnecticity_in;
    RasterData * GrowPotential;
    RasterData * GrowPotential_tot;
   RasterData * GrowPotentialStreet;


};
private:
int times;
Parameter param;
double calcPopCon(long x, long y, RasterData * rData);
double calcGrowthFactor(long x, long y, RasterData * rData);
double calcOverCrowding(long x, long y, double val, RasterData * rData );
double calcRoadAccessFactor(long x, long y,int currentLandUse);
public:
PopulationGrowthModel();

//virtual void init(const parameter_type &parameters);
void run();


};

#endif // POPULATIONGROWTHMODEL_H
