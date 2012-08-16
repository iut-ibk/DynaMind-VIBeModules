//#include "populationgrowthmodel.h"
//#include <cstdlib>
//#include <QString>
//#include <vector>
//#include <algorithm>
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/vector_proxy.hpp>
//#include <QThread>
//#include "corine.h"

////using namespace boost::numeric::ublas;
//DM_DECLARE_NODE_NAME( PopulationGrowthModel,Vibe )

//PopulationGrowthModel::PopulationGrowthModel() {
//    param.Width = 400;
//    param.Height = 240;
//    param.CellSize = 20;
//    param.MenanderWidth = 560;
//    param.maxCPopDensity = 200;
//    param.maxDCPopDensity = 100;
//    param.maxOBPopDensity = 30;
//    param.popChangeLow_in = 95;
//    param.popCUFRand_in = 10;
//    param.popDCUFRand_in = 12;
//    param.popAGRIRand_in = 19;
//    param.populationSteps = 48;
//    param.roadInfluence = 50;


//    this->addParameter("Width", LONG, &param.Width);
//    this->addParameter("Height", LONG, &param.Height);
//    this->addParameter("CellSize", DOUBLE, &param.CellSize);
//    this->addParameter("MenanderWidth", DOUBLE, &param.MenanderWidth);
//    this->addParameter("maxCPopDensity", DOUBLE, &param.maxCPopDensity);
//    this->addParameter("maxDCPopDensity", DOUBLE, &param.maxDCPopDensity);
//    this->addParameter("maxOBPopDensity", DOUBLE, &param.maxOBPopDensity);
//    this->addParameter("popChangeLow", DOUBLE, &param.popChangeLow_in);
//    this->addParameter("popCUFRand", DOUBLE, &param.popCUFRand_in);
//    this->addParameter("popDCUFRand", DOUBLE, &param.popDCUFRand_in);
//    this->addParameter("popAGRIRand", DOUBLE, &param.popAGRIRand_in);
//    this->addParameter("PopulationSteps", INT, &param.populationSteps);
//    this->addParameter("RaodInfluence", DOUBLE, &param.roadInfluence);

//    /*this->addParameter("PopConnectivity_in", RASTERDATA_IN, &param.PopConnectivity_in);
//    this->addParameter("RoadConnecticity_in", RASTERDATA_IN, &param.RoadConnecticity_in);
//    this->addParameter("Population_in", RASTERDATA_IN, &param.Population_in);
//    this->addParameter("LandUse", RASTERDATA_IN, &param.LandUse);

//    this->addParameter("Population_out", RASTERDATA_OUT, &param.Population_out);
//    this->addParameter("PopConnectivity_out", RASTERDATA_OUT, &param.PopConnectivity_out);
//    this->addParameter("GrowPotential_out", RASTERDATA_OUT, &param.GrowPotential);
//    this->addParameter("GrowPotential_tot_out", RASTERDATA_OUT, &param.GrowPotential_tot);
//    this->addParameter("GrowPotentialStreet_out", RASTERDATA_OUT, &param.GrowPotentialStreet);*/

//}


//void PopulationGrowthModel::run() {
//    param.popChangeLow = param.popChangeLow_in * 0.01;
//    param.popCUFRand = param.popCUFRand_in+(100-param.popChangeLow*100);
//    param.popDCUFRand = param.popDCUFRand_in+(100-param.popChangeLow*100);
//    param.popAGRIRand = param.popAGRIRand_in +(100-param.popChangeLow*100);
//    param.PopConnectivity_out->setSize(param.Width, param.Height, param.CellSize);
//    param.PopConnectivity_out->clear();
//    param.GrowPotential->setSize(param.Width, param.Height, param.CellSize);
//    param.GrowPotential->clear();
//    param.GrowPotential_tot->setSize(param.Width, param.Height, param.CellSize);
//    param.GrowPotential_tot->clear();
//    param.GrowPotentialStreet->setSize(param.Width, param.Height, param.CellSize);
//    param.GrowPotentialStreet->clear();
//    param.Population_out->setSize(param.Width, param.Height, param.CellSize);
//    param.Population_out->clear();
//    double sum = 0;

//    Logger(Debug) << "Sum pop Con " << param.PopConnectivity_in->getSum();
//    for (int n = 0; n < param.populationSteps; n++) {
//        for ( unsigned long i = 0; i < param.Width; i++ ) {
//            for ( unsigned long j = 0; j < param.Height; j++ ) {
//                if ( param.LandUse->getValue(i,j) < CORINE::ForestsSemiNatural ) {
//                    double val = calcPopCon(i,j, param.PopConnectivity_in);
//                    double grothfactor = calcGrowthFactor(i,j,param.LandUse);
//                    double ro = calcRoadAccessFactor(i,j, (int)param.LandUse->getValue(i,j));
//                    val *= grothfactor;
//                    val *= calcRoadAccessFactor(i,j, (int)param.LandUse->getValue(i,j));
//                    //val = calcOverCrowding(i,j, val, param.LandUse);
//                    param.GrowPotential->setValue(i,j,grothfactor);
//                    param.GrowPotentialStreet->setValue(i,j,ro);
//                    param.GrowPotential_tot->setValue(i,j,grothfactor*ro);
//                    param.Population_out->setValue(i,j, val);
//                    param.PopConnectivity_out->setValue(i,j, val);
//                    sum+=val;
//                } else {
//                    param.GrowPotential->setValue(i,j,0);
//                    param.GrowPotentialStreet->setValue(i,j,0);
//                    param.GrowPotential_tot->setValue(i,j,0);
//                    param.Population_out->setValue(i,j, 0);
//                    param.PopConnectivity_out->setValue(i,j, 0);
//                }
//            }
//        }
//    }




//    times++;
//}


//double PopulationGrowthModel::calcPopCon(long x, long y, RasterData * rData) {
//    ublas::vector<double> vec( rData->getMoorNeighbourhood(x,y));


//    double currentVal = vec[4];
//    sort(  vec.begin(), vec.end() );
//    ublas::vector<double> vec5 (subrange(vec, 4,9));

//    double average5 = sum(vec5)/5.;

//    if ( average5 < currentVal*param.popChangeLow) {
//        average5 = currentVal*param.popChangeLow;
//    }


//    return average5;

//}
//double PopulationGrowthModel::calcOverCrowding(long x, long y, double val, RasterData * rData ) {
//    double currentDensity;
//    int currentLandUse = (int) rData->getValue(x,y);
//    if (currentLandUse == CORINE::ContUrbanFabric)
//        currentDensity=param.maxCPopDensity;

//    if (currentLandUse == CORINE::DisContUrbanFabric)
//        currentDensity=param.maxDCPopDensity*1.3; //1.0

//    if (currentLandUse > CORINE::AgriculturalAreas)
//        currentDensity=param.maxOBPopDensity;



//    if(val < param.popChangeLow*val) {
//        val = val*param.popChangeLow;
//    }
//    if (val > (1./param.popChangeLow)*param.maxCPopDensity) {
//        val = (1./param.popChangeLow)*param.maxCPopDensity;
//    }

//    return val;


//}
//double PopulationGrowthModel::calcRoadAccessFactor(long x, long y, int currentLandUse) {
//    double RoadAccessFactor=1.0;
//    double currentDensity=0;

//    if (currentLandUse == CORINE::ContUrbanFabric)
//        currentDensity=param.maxCPopDensity*1.1; //1.0

//    if (currentLandUse == CORINE::DisContUrbanFabric)
//        currentDensity=param.maxDCPopDensity*1.1; //1.0

//    if (currentLandUse > CORINE::DisContUrbanFabric)
//        currentDensity=param.maxOBPopDensity;

//    double val = param.RoadConnecticity_in->getValue(x,y);

//    //RoadAccessFactor=(1.-val/currentDensity);
//    RoadAccessFactor=(1-val/currentDensity);
//    if (RoadAccessFactor>1.3) {
//        RoadAccessFactor=1.3;
//    }
//    /*   else if (RoadAccessFactor<param.popChangeLow){
//        RoadAccessFactor=param.popChangeLow;
//    }*/
//    else if (RoadAccessFactor<param.roadInfluence/100.){
//        RoadAccessFactor=param.roadInfluence/100.;
//    }

//    return RoadAccessFactor;
//}

//double PopulationGrowthModel::calcGrowthFactor(long x, long y, RasterData * rData) {
//    int currentLandUse = (int) rData->getValue(x,y);
//    double val;
//    double para1 = 5;
//    int ra;
//    switch (currentLandUse) {
//    case CORINE::ContUrbanFabric:
//        ra = rand() % (int) param.popCUFRand;
//        val= (param.popChangeLow*100 +  ra) * 0.01;
//        break;
//    case CORINE::DisContUrbanFabric:
//        ra = rand() % (int) param.popDCUFRand;
//        val= (param.popChangeLow*100 +  ra) * 0.01;
//        break;
//    case CORINE::AgriculturalAreas:
//        ra = rand() % (int) param.popAGRIRand;
//        val= (param.popChangeLow*100 +  ra) * 0.01;
//        break;
//    default:
//        val= param.popChangeLow;
//        break;

//    }

//    return val;
//}
