#include "topology.h"
#include "dmrasterdata.h"
#include "QString"
#include "d_topoCreator.h"
#include "rules.h"
#include "ca.h"
#include <QThread>
#include <QMap>
#include <boost/unordered_map.hpp>
using namespace boost;

DM_DECLARE_NODE_NAME(Topology, VIBe)

Topology::~Topology() {
}

Topology::Topology() {
    /*<!--General Parameter-->
    <parameter name ="Width" value="Integer" default="400" />
    <parameter name ="Height" value="Integer" default="240" />
    <parameter name ="CellSize" value="Double" default="20" />

    <!--River Parameter-->
    <parameter name ="RiverSlope" value="Double" default="0.001" />
    <parameter name ="RiverWidth" value="Double" default="100" />
    <parameter name ="ElevationBelow" value="Double" default="4" />
    <parameter name ="SectionBasisLength" value="Double" default="1000" />
    <parameter name ="MenanderWidth" value="Double" default="560" />
    <parameter name ="MinorRivers" value="Integer" default="2" />
    <parameter name ="NumberRiverSinuosities" value="Integer" default="1" />

    <!--Relief Parameter-->
    <parameter name ="BasicElevation" value="Double" default="100" />
    <parameter name ="BasicSlope" value="Double" default="0.0005" />
    <parameter name ="MaximalDHeight" value="Double" default="100" />
    <parameter name ="MaximalHillHeight" value="Double" default="20" />
    <parameter name ="MaximalNoOfHills" value="Double" default="40" />  */

    param.width = 400;
    param.height = 240;
    param.cellSize = 20;
    param.RiverSlope = 0.001;
    param.RiverWidth = 100;
    param.ElevationBelow = 4;
    param.SectionBasisLength = 1000;
    param.MenanderWidth = 560;
    param.MinorRivers = 2;
    param.NumberRiverSinuisities = 1;
    param.BasicElevation = 100;
    param.BasicSlope = 0.0005;
    param.MaximalDHeight = 100;
    param.MaximalHillHeight = 20;
    param.MaximalNoOfHills = 40;
    this->addParameter("Width", LONG, & param.width);
    this->addParameter("Height",LONG, & param.height);
    this->addParameter("CellSize", DOUBLE,& param.cellSize);
    this->addParameter("RiverSlope", DOUBLE ,& param.RiverSlope);
    this->addParameter("RiverWidth", DOUBLE, & param.RiverWidth);
    this->addParameter("ElevationBelow",DOUBLE, & param.ElevationBelow);
    this->addParameter("SectionBasisLength", DOUBLE,& param.SectionBasisLength);
    this->addParameter("MenanderWidth", DOUBLE, & param.MenanderWidth);
    this->addParameter("MinorRivers",LONG, & param.MinorRivers);
    this->addParameter("NumberRiverSinuosities", LONG, & param.NumberRiverSinuisities);
    this->addParameter("BasicElevation",DOUBLE, & param.BasicElevation);
    this->addParameter("BasicSlope", DOUBLE, & param.BasicSlope);
    this->addParameter("MaximalDHeight", DOUBLE, & param.MaximalDHeight);
    this->addParameter("MaximalHillHeight", DOUBLE,& param.MaximalHillHeight);
    this->addParameter("MaximalNoOfHills", LONG, & param.MaximalNoOfHills);
    //this->addParameter("topoStatus", RASTERDATA_OUT, & param.topoStatus);
    //this->addParameter("topoElevation",  RASTERDATA_OUT, & param.topoElevation);
    //this->addParameter("topoGradient", RASTERDATA_OUT, & param.topoGradient);

    std::vector<DM::View> data;
    vtopoStatus = DM::View("topoStatus", DM::RASTERDATA, WRITE);
    vtopoElevation= DM::View("topoElevation", DM::RASTERDATA, WRITE);
    vtopoGradient= DM::View("topoGradient", DM::RASTERDATA, WRITE);

    data.push_back(vtopoStatus);
    data.push_back(vtopoElevation);
    data.push_back(vtopoGradient);

    this->addData("Topology", data);

}

void Topology::run() {
    this->param.topoElevation = this->getRasterData("Topology", vtopoElevation);
    this->param.topoGradient = this->getRasterData("Topology", vtopoGradient);
    this->param.topoStatus= this->getRasterData("Topology", vtopoStatus);

    this->param.topoElevation->setSize(param.width, param.height, param.cellSize);
    this->param.topoGradient->setSize(param.width, param.height, param.cellSize);
    this->param.topoStatus->setSize(param.width, param.height, param.cellSize);

    std::cout << "Create Topology" << std::endl;

    VIBe::dataLayer **layer_;

    const int layerSize=100;
    if (100<layerSize)
    {
        //resize dataLayer
    }
    else
    {
        layer_=new VIBe::dataLayer*[layerSize];

        for (int i =0;i<layerSize;i++)
        {
            layer_[i]=NULL;
        }

    }

    for (int n=0; n<VIBe::MAX_dataLayer+1; n++)
    {
        layer_[n]=new VIBe::dataLayer(param.width,param.height,param.cellSize);
        for ( unsigned long i = 0; i < param.width; i++ ) {
            for ( unsigned long  j = 0; j < param.height; j++ ) {
                layer_[n]->setVal(i,j,0);
            }
        }
    }


    VIBe::topoCreator topoC_=VIBe::topoCreator(layer_);

    topoC_.setRiverSlope(param.RiverSlope);
    topoC_.setRiverWidth(param.RiverWidth);
    topoC_.setRiverdHeight(param.ElevationBelow);
    topoC_.setBasisLength(param.SectionBasisLength);
    topoC_.setMeanderWidth(param.MenanderWidth);
    layer_[VIBe::topoStatus]->setmHeight(param.MenanderWidth/param.cellSize);
    topoC_.setNoMinorRivers(param.MinorRivers);
    topoC_.setNoSinuosity(param.NumberRiverSinuisities);
    topoC_.setBasicElevation(param.BasicElevation);
    topoC_.setElevationSlope(param.BasicSlope);
    topoC_.setMaximaldHeight(param.MaximalDHeight);
    topoC_.setMaxHilldHeight(param.MaximalHillHeight);
    topoC_.setMaxNoHills(param.MaximalNoOfHills);


    topoC_.generateTopology(layer_);
    VIBe::rule topoStatusArtefacts(layer_);
    topoStatusArtefacts.r_topoStatus();

    //calculate gradient field
    VIBe::caTopo Grad;
    VIBe::rule *rule_;
    rule_ = new VIBe::rule(layer_);
    Grad.gradField(layer_,*rule_);


    for ( unsigned long  i = 0; i < param.width; i++ ) {
        for ( unsigned long  j = 0; j < param.height; j++ ) {
            param.topoStatus->setValue(i,j, layer_[VIBe::topoStatus]->getVal(i,j));
            param.topoElevation->setValue(i,j, layer_[VIBe::topoElevation]->getVal(i,j));
            param.topoGradient->setValue(i,j, layer_[VIBe::topoGradient]->getVal(i,j));
        }
    }

    for (int n=0; n<VIBe::MAX_dataLayer+1; n++)
    {
        delete layer_[n];

    }
}


