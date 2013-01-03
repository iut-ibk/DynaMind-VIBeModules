#include "initialcitycenters.h"
#include <cstdlib>
#include <QString>
#include <vector>
#include <algorithm>
#include <QThread>
#include <math.h>

DM_DECLARE_NODE_NAME( InitialCityCenters, VIBe )

InitialCityCenters::InitialCityCenters() {
    param.Width = 400;
    param.Height = 240;
    param.CellSize = 20;
    param.MenanderWidth = 560;
    param.numberCenters = 3;
    this->addParameter("Width", LONG, &param.Width);
    this->addParameter("Height", LONG, &param.Height);
    this->addParameter("CellSize", DOUBLE, &param.CellSize);
    this->addParameter("MenanderWidth", DOUBLE, &param.MenanderWidth);
    this->addParameter("numberCenters", INT, &param.numberCenters);

    std::vector<DM::View> data;

    vTopoStatus = DM::View("topoStatus", DM::RASTERDATA, READ);
    vVectorDataOut = DM::View("CityCenters", DM::NODE, WRITE);
    vVectorDataOut.addAttribute("Radius");

    data.push_back(vTopoStatus);
    data.push_back(vVectorDataOut);

    this->addData("City", data);



}

void InitialCityCenters::run() {
    DM::System * city = this->getData("City");
    std::vector<Node> vCityCenters;
    std::vector<double> vCityCenteryParam;

    
    unsigned long MenanderWidthinCells = (unsigned long) param.MenanderWidth/param.CellSize;
    std::vector<unsigned long> vx;

    for (int i = 0; i < param.numberCenters; i++) {
        unsigned long x = floor(param.Width * 0.30 * (1+1.33*(rand()%(100)*0.01)));
        vx.push_back(x);
    }
    std::sort( vx.begin(), vx.end() );



    for (std::vector<unsigned long>::const_iterator it = vx.begin(); it!=vx.end(); ++it) {
        //x between 30% - 70%
        Node pos;
        pos.x = *it;
        pos.y = 0;
        pos.z = 0;
        int riverside = rand() % 2;
        long radius = 0;

        //on river to x=0 side
        if (riverside == 0) {
            for (unsigned long j = 0; j < param.Height; j++) {
                if (topoStatus->getCell( pos.x, j ) == 10) {
                    radius = floor(MenanderWidthinCells * (20+rand()%(80))*0.01 * (-1));
                    pos.y = j + radius;
                    break;
                }
            }
        }
        //on river to x=width side
        if (riverside == 1) {
            for (unsigned long j = param.Height - 1;j > 0; j--) {
                if (topoStatus->getCell(pos.x, j) == 10) {
                    radius = floor(MenanderWidthinCells * (20+rand()%(80))*0.01 );
                    pos.y = j + radius;
                    break;
                }
            }
        }

        vCityCenters.push_back(Node(pos.x*param.CellSize, pos.y*param.CellSize,radius*param.CellSize) );
        vCityCenteryParam.push_back(radius);
    }

    int counter = 0;
    foreach (Node c, vCityCenters) {
        DM::Node * n = city->addNode(c, vVectorDataOut);
        n->addAttribute("Radius", vCityCenteryParam[counter++]);
    }

}

