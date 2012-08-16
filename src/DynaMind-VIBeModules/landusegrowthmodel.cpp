//#include "landusegrowthmodel.h"
//#include "vectordata.h"
//#include <cstdlib>
//#include <QString>
//#include <vector>
//#include <algorithm>
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/vector_proxy.hpp>
//#include <QThread>
//#include "corine.h"


////using namespace boost::numeric::ublas;
//VIBe_DECLARE_NODE_NAME( LandUseGrowthModel, Vibe )

//LandUseGrowthModel::LandUseGrowthModel() {

//   /* <parameter name ="Width" value="Integer" default="400" />
//    <parameter name ="Height" value="Integer" default="240" />
//    <parameter name ="CellSize" value="Double" default="20" />
//    <parameter name ="MenanderWidth" value="Double" default="560" />
//    <parameter name ="maxCPopDensity" value="Double" default="200" />
//    <parameter name ="maxDCPopDensity" value="Double" default="100" />
//    <parameter name ="maxOBPopDensity" value="Double" default="30" />
//    <parameter name ="popCUFRand" value="Double" default="10" />
//    <parameter name ="popDCUFRand" value="Double" default="12" />
//    <parameter name ="popAGRIRand" value="Double" default="19" />
//    <parameter name ="popChangeLow" value="Double" default="95" />
//    <parameter name ="LandSteps" value="Integer" default="1" />  */
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
//    param.LandSteps = 1;


//    this->addParameter("Width", VIBe2::LONG, &param.Width);
//    this->addParameter("Height", VIBe2::LONG, &param.Height);
//    this->addParameter("CellSize", VIBe2::DOUBLE, &param.CellSize);
//    this->addParameter("MenanderWidth", VIBe2::DOUBLE, &param.MenanderWidth);
//    this->addParameter("maxCPopDensity", VIBe2::DOUBLE, &param.maxCPopDensity);
//    this->addParameter("maxDCPopDensity", VIBe2::DOUBLE, &param.maxDCPopDensity);
//    this->addParameter("maxOBPopDensity", VIBe2::DOUBLE, &param.maxOBPopDensity);
//    this->addParameter("popChangeLow", VIBe2::DOUBLE, &param.popChangeLow_in);
//    this->addParameter("popCUFRand", VIBe2::DOUBLE, &param.popCUFRand_in);
//    this->addParameter("popDCUFRand", VIBe2::DOUBLE, &param.popDCUFRand_in);
//    this->addParameter("popAGRIRand", VIBe2::DOUBLE, &param.popAGRIRand_in);
//    this->addParameter("LandSteps", VIBe2::INT, &param.LandSteps);

//    //this->addParameter("PopConnectivity_in", VIBe2::RASTERDATA_IN, &param.PopConnectivity_in);
//    this->addParameter("Population_in", VIBe2::RASTERDATA_IN, &param.Population_in);
//    this->addParameter("LandUse", VIBe2::RASTERDATA_IN, &param.LandUse);
//    this->addParameter("LandUse_out", VIBe2::RASTERDATA_OUT, &param.LandUse_out);

//}




//void LandUseGrowthModel::run() {
//     param.popChangeLow = param.popChangeLow_in * 0.01;
//     param.popCUFRand = param.popCUFRand_in+(100-param.popChangeLow*100);
//     param.popDCUFRand = param.popDCUFRand_in+(100-param.popChangeLow*100);
//     param.popAGRIRand = param.popAGRIRand_in +(100-param.popChangeLow*100);


//    param.LandUse_out->setSize(param.Width, param.Height, param.CellSize);

//    for (int n = 0; n < param.LandSteps; n++) {
//        for ( long i = 0; i < param.Width; i++ ) {
//            for ( long j = 0; j < param.Height; j++ ) {
//                int val = calculateLanduse(i,j,  param.Population_in);
//                if ( val > 0 && param.LandUse->getValue(i,j) != CORINE::RoadRailNetwork) {
//                    param.LandUse_out->setValue(i,j, val);
//                } else {
//                    param.LandUse_out->setValue(i,j, param.LandUse->getValue(i,j));
//                }
//            }
//        }
//    }
//    times++;
//}

//int LandUseGrowthModel::calculateLanduse(long x, long y, RasterData * rData) {
//    double pop = rData->getValue(x,y);
//    int landuse = 0;
//    if ( pop >= param.maxDCPopDensity*0.98) {
//        landuse = CORINE::ContUrbanFabric;
//    }
//    if ( pop > param.maxOBPopDensity && pop <= param.maxDCPopDensity) {
//        landuse =CORINE::DisContUrbanFabric;
//    }
//    return landuse;

//}
