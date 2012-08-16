#include "roadnetwork.h"
#include <QString>
#include <QThread>
#include <QLineF>
#include <math.h>

DM_DECLARE_NODE_NAME( RoadNetwork, VIBe )

        RoadNetwork::RoadNetwork()
{
    param.Height = 240;
    param.Width = 400;
    param.CellSize = 20;
    param.roadDistanceToRiver = 140;
    param.roadBasisLength = 300;
    param.dAlpha = 15.;
    param.roadConnectivity = 15.;

    this->addParameter("Height", LONG, &param.Height);
    this->addParameter("Width", LONG, &param.Width);
    this->addParameter("CellSize", DOUBLE, &param.CellSize);
    this->addParameter("roadDistanceToRiver", DOUBLE, &param.roadDistanceToRiver_in);
    this->addParameter("roadBasisLength", DOUBLE, &param.roadBasisLength_in);
    this->addParameter("dAlpha", DOUBLE, &param.dAlpha_in);
    this->addParameter("roadConnectivity", DOUBLE, &param.roadConnectivity_in);
    /*this->addParameter("topoStatus", RASTERDATA_IN, &param.topoStatus);
    this->addParameter("RoadNetworkRa", VIBe2::RASTERDATA_OUT, &param.RoadNetworkRa);
    this->addParameter("CityCenters", VIBe2::VECTORDATA_IN, &param.CityCenters);
    this->addParameter("RoadNetworkVec", VIBe2::VECTORDATA_OUT, &param.RoadNetworkVec);*/

    vTopoStatus = DM::View("TopoStatus", DM::RASTERDATA, DM::READ);
    vRoadNetworkRa = DM::View("RoadNetworkRa", DM::RASTERDATA, DM::WRITE);

    vCityCenters = DM::View("CityCenters", DM::NODE, DM::READ);
    vRoadNetworkVec = DM::View("RoadNetworkVec", DM::EDGE, DM::WRITE);

    std::vector<DM::View> data;

    data.push_back(vTopoStatus);
    data.push_back(vRoadNetworkRa);
    data.push_back(vCityCenters);
    data.push_back(vRoadNetworkVec);

    this->addData("City", data);

}


void RoadNetwork::run() {

    city = this->getData("City");
    param.RoadNetworkRa = this->getRasterData("City",vRoadNetworkRa);
    param.RoadNetworkRa->setSize(param.Width, param.Height, param.CellSize);
    param.RoadNetworkRa->clear();
    param.roadDistanceToRiver =  param.roadDistanceToRiver_in / param.CellSize;
    param.roadBasisLength = param.roadBasisLength_in / param.CellSize;
    param.dAlpha = param.dAlpha_in / 0.01745329;
    param.roadConnectivity = param.dAlpha_in / param.CellSize;


    Node StartPoint;
    StartPoint.x = 0;
    StartPoint.y = this->findPointNearRiver(StartPoint.x, param.roadDistanceToRiver,  param.topoStatus);

    param.RoadNetworkRa->setValue(StartPoint.x, StartPoint.y, 1);

    Node EndPoint;
    EndPoint.x = param.Width -1;
    EndPoint.y = this->findPointNearRiver(EndPoint.x, param.roadDistanceToRiver,  param.topoStatus);


    param.RoadNetworkRa->setValue(EndPoint.x, EndPoint.y, 1);

    std::vector<Node> vP ;

    std::vector<std::string> centers = city->getUUIDsOfComponentsInView(vCityCenters);
    foreach(std::string c , centers) {
        Node * p = this->city->getNode(c);
        vP.push_back(Node(p->getX()/param.CellSize, p->getY()/param.CellSize, p->getZ()));
    }
    //std::cout << "huhu1" << std::endl;
    if (vP.size() < 1) {
         Logger(Error) << "No City Centers";
        return;
    }
    Node StartPointCity = vP[0];
    Node EndPointCity =vP[vP.size()-1];


    std::vector<Node> wayPoints = this->findWayPoints(StartPoint, EndPoint, param.topoStatus);

    //build up connections
    std::vector<Edge> edges;

    //Create Main Road

    //param.RoadNetworkVec = VectorData();


    for (long i = 0; i < wayPoints.size()-1; i++ ) {
        Node n1(wayPoints[i].getX()*param.CellSize+param.CellSize/2.,wayPoints[i].getY()*param.CellSize+param.CellSize/2.,wayPoints[i].getZ()*param.CellSize+param.CellSize/2.);
        Node n2(wayPoints[i+1].getX()*param.CellSize+param.CellSize/2.,wayPoints[i+1].getY()*param.CellSize+param.CellSize/2.,wayPoints[i+1].getZ()*param.CellSize+param.CellSize/2.);
        this->city->addEdge(this->city->addNode(n1), this->city->addNode(n2), vRoadNetworkVec);
    }
    for ( int i = 0; i < vP.size(); i++ ) {
        long id = this->findNearestPoint(vP[i], wayPoints);
        double l = (vP[i].x - wayPoints[id].x) * (vP[i].x - wayPoints[id].x) + (vP[i].y - wayPoints[id].y)*(vP[i].y - wayPoints[id].y);
        if (l > 100) {
            Node n1(wayPoints[id].getX()*param.CellSize+param.CellSize/2.,wayPoints[id].getY()*param.CellSize+param.CellSize/2., wayPoints[id].getZ()*param.CellSize+param.CellSize/2.);
            Node n2(vP[i].getX()*param.CellSize+param.CellSize/2.,vP[i].getY()*param.CellSize+param.CellSize/2.,vP[i].getZ()*param.CellSize+param.CellSize/2.);
            this->city->addEdge(this->city->addNode(n1), this->city->addNode(n2), vRoadNetworkVec);
        }

    }
    int counter = 0;
    int counter1 = 0;
    for ( std::vector<Node>::const_iterator it = wayPoints.begin(); it != wayPoints.end(); ++it) {
        Node p(*it);
        counter1++;
        if ( counter1 > 8 && counter > 0 ) {
            param.RoadNetworkRa->setValue(p.x, p.y, p.z);
            counter1 = 0;
        }
        counter++;
    }
}
long RoadNetwork::findNearestPoint(const Node p,const std::vector<Node>  & points) {

    long id = 0;
    if ( points.size() < 1 ) {
        return -1;
    }
    double lref = (points[0].x - p.x) * (points[0].x - p.x) + (points[0].y - p.y) * (points[0].y - p.y);
    for (  long i = 1; i < points.size(); i++ ) {
        double l =  (points[i].x - p.x) * (points[i].x - p.x) + (points[i].y - p.y) * (points[i].y - p.y);
        if ( l < lref ) {
            id = i;
            lref = l;
        }
    }
    return id;
}


std::vector<Node> RoadNetwork::findWayPoints(Node startPoint, Node endPoint, RasterData * topoStatus) {

    std::vector<Node> wayPoints;

    wayPoints.push_back(startPoint);
    int counter = 0;
    while (endPoint.x - wayPoints[wayPoints.size()-1].x > 2 * param.roadBasisLength && counter < 50) {

        Node pos;
        pos.x = 0;
        pos.y = 0;
        pos.z = 360;

        double currentDx = endPoint.x - wayPoints[wayPoints.size()-1].x;
        double currentDy = endPoint.y - wayPoints[wayPoints.size()-1].y;

        double currentDistance = sqrt((currentDx*currentDx)+(currentDy*currentDy));
        double basisLength = param.roadBasisLength * (50+rand()%50) * 0.01;

        if (currentDistance < basisLength){
            break;
        }

        double currentDistanceToRiver=param.roadDistanceToRiver*((75+rand()%(50))*0.01);
        double currentAngle=atan(currentDy/currentDx);
        pos.x =  wayPoints[wayPoints.size()-1].x + basisLength * cos(currentAngle);
        double newbufferY = wayPoints[wayPoints.size()-1].y + basisLength*sin(currentAngle);
        pos.z = currentAngle*180/3.14+360;
        //pos.z = -45;
        pos.y = 0;
        int side = 0;

        for (long j = 0; j < param.Height; j++)
        {
            //check river
            if (topoStatus->getValue(pos.x,j)== 10)
            {
                if (j >= newbufferY)
                {
                    if (j-currentDistanceToRiver-newbufferY>0)
                    {
                        currentDistanceToRiver+=0.5*(j-currentDistanceToRiver-newbufferY);
                    }
                    pos.y=j-currentDistanceToRiver;
                    side=-1; //lower y
                    if (topoStatus->getValue(pos.x,pos.y) < 9)
                    {
                        break;
                    }
                }
            }


        }
        for (int j=param.Height-1;j>0;j--)
        {
            //check river
            if (topoStatus->getValue(pos.x,j)== 10)
            {
                if (j<=newbufferY)
                {
                    if (j-currentDistanceToRiver-newbufferY>0)
                    {
                        currentDistanceToRiver+=0.5*(j-currentDistanceToRiver-newbufferY);
                    }
                    pos.y=j+currentDistanceToRiver;
                    side=1;
                    if (topoStatus->getValue(pos.x,pos.y) < 9)
                    {
                        break;
                    }
                }
            }
        }

        wayPoints.push_back(pos);
        counter++;

    }
    wayPoints.push_back(endPoint);
    return wayPoints;
}



long RoadNetwork::findPointNearRiver(long x, long offest, RasterData * rData) {
    int riverside = (int) rand() % 2;
    long y = 0;

    //on river to x=0 side
    if (riverside == 0) {
        for (unsigned long j = 0; j < param.Height; j++) {
            if (rData->getValue( x, j ) > 8) {
                y = j - offest;
                break;
            }
        }
    }
    //on river to x=width side
    if (riverside == 1) {
        for (unsigned long j = param.Height - 1;j > 0; j--) {
            if (rData->getValue(x, j) > 8) {
                y = j + offest;
                break;
            }
        }
    }
    return y;
}

