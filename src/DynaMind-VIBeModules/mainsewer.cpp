//#include "mainsewer.h"
//#include <boost/foreach.hpp>
//#include <QList>
//#include <QVector>
//#include <sstream>
//DM_DECLARE_NODE_NAME(MainSewer, Vibe)
//MainSewer::MainSewer()
//{

//    param.Width = 400;
//    param.Height = 240;
//    param.CellSize = 20;
//    param.sewerBasisLength_in = 300;
//    param.sewerDistanceToRiver_in = 95;
//    param.sewerConnectivity_in = 300;


//    this->addParameter("Height", VIBe2::LONG, &param.Height);
//    this->addParameter("Width", VIBe2::LONG, &param.Width);
//    this->addParameter("CellSize", VIBe2::DOUBLE, &param.CellSize);
//    this->addParameter("sewerBasisLength", VIBe2::DOUBLE, &param.sewerBasisLength_in);
//    this->addParameter("topoElevation", VIBe2::RASTERDATA_IN, &param.topoElevation);
//    this->addParameter("topoStatus", VIBe2::RASTERDATA_IN, &param.topoStatus);
//    this->addParameter("CityCenters", VIBe2::VECTORDATA_IN, &param.CityCenters);
//    this->addParameter("WWTP", VIBe2::VECTORDATA_IN, &param.WWTP);
//    this->addParameter("sewerDistanceToRiver", VIBe2::DOUBLE, &param.sewerDistanceToRiver_in);
//    this->addParameter("sewerConnectivity", VIBe2::DOUBLE, &param.sewerConnectivity_in);
//    this->addParameter("initSewerConnectivity", VIBe2::RASTERDATA_OUT, &param.initSewerConnectivity);
//    this->addParameter("MainSewerNetwork", VIBe2::VECTORDATA_OUT, &param.MainSewerNetwork);


//}
//MainSewer::~MainSewer() {

//}
//std::vector<Point> MainSewer::findWayPoints(Point startPoint, Point endPoint, RasterData * topoStatus) {

//    std::vector<Point> wayPoints;

//    wayPoints.push_back(startPoint);
//    int counter = 0;
//    while (endPoint.x - wayPoints[wayPoints.size()-1].x > 2 * param.sewerBasisLength && counter < 50) {

//        Point pos;
//        pos.x = 0;
//        pos.y = 0;
//        pos.z = 360;

//        double currentDx = endPoint.x - wayPoints[wayPoints.size()-1].x;
//        double currentDy = endPoint.y - wayPoints[wayPoints.size()-1].y;

//        double currentDistance = sqrt((currentDx*currentDx)+(currentDy*currentDy));
//        double basisLength = param.sewerBasisLength * (50+rand()%50) * 0.01;

//        if (currentDistance < basisLength){
//            break;
//        }

//        double currentDistanceToRiver=param.sewerDistanceToRiver*((75+rand()%(50))*0.01);
//        double currentAngle=atan(currentDy/currentDx);
//        pos.x =  wayPoints[wayPoints.size()-1].x + basisLength * cos(currentAngle);
//        double newbufferY = wayPoints[wayPoints.size()-1].y + basisLength*sin(currentAngle);
//        pos.z = currentAngle*180/3.14+360;
//        //pos.z = -45;
//        pos.y = 0;
//        int side = 0;

//        for (long j = 0; j < param.Height; j++)
//        {
//            //check river
//            if (topoStatus->getValue(pos.x,j)== 10)
//            {
//                if (j >= newbufferY)
//                {
//                    if (j-currentDistanceToRiver-newbufferY>0)
//                    {
//                        currentDistanceToRiver+=0.5*(j-currentDistanceToRiver-newbufferY);
//                    }
//                    pos.y=j-currentDistanceToRiver;
//                    side=-1; //lower y
//                    if (topoStatus->getValue(pos.x,pos.y) < 9)
//                    {
//                        break;
//                    }
//                }
//            }


//        }
//        for (int j=param.Height-1;j>0;j--)
//        {
//            //check river
//            if (topoStatus->getValue(pos.x,j)== 10)
//            {
//                if (j<=newbufferY)
//                {
//                    if (j-currentDistanceToRiver-newbufferY>0)
//                    {
//                        currentDistanceToRiver+=0.5*(j-currentDistanceToRiver-newbufferY);
//                    }
//                    pos.y=j+currentDistanceToRiver;
//                    side=1;
//                    if (topoStatus->getValue(pos.x,pos.y) < 9)
//                    {
//                        break;
//                    }
//                }
//            }
//        }

//        wayPoints.push_back(pos);
//        counter++;

//    }
//    wayPoints.push_back(endPoint);
//    return wayPoints;
//}

//void MainSewer::run() {
//    param.sewerBasisLength = param.sewerBasisLength_in/param.CellSize;
//    param.sewerDistanceToRiver = param.sewerDistanceToRiver_in/param.CellSize;
//    param.sewerConnectivity = param.sewerConnectivity_in/param.CellSize;

//    param.initSewerConnectivity->setSize(param.Width, param.Height, param.CellSize);
//    param.initSewerConnectivity->clear();
//    param.MainSewerNetwork->clear();

//    //find sewer functions
//    //-------------------------//
//    findInitSewer();
//    std::vector<Point> vP ;
//    std::vector<std::string> names;
//    names = param.CityCenters->getPointsNames();
//    foreach(std::string name, names) {
//        foreach( Point p, param.CityCenters->getPoints(name)) {
//            vP.push_back(Point(p.x/param.CellSize, p.y/param.CellSize, p.z));
//        }
//    }
//    int numberCenters = vP.size();
//    for (int i=1;i<numberCenters;i++)
//    {
//        findSecSewer(i);
//    }
//    QVector<VIBe::segment> * segments = mainSewer.getSegments();

//    int shaft_counter = 0;
//    int edge_counter = 0;
//    for (int i = 0; i < segments->size(); i++ ) {
//        std::vector<Point> pointlist;
//        std::vector<Edge> edges;
//        VIBe::segment s = segments->at(i);
//        QVector<VIBe::sPoint> * points = s.getPoints();
//        int counter = 0;
//        for (int j = 0; j < points->size(); j++) {
//            counter++;
//            VIBe::sPoint sP = points->at(j);
//            Point p_ectract  (sP.getPointF().x()*param.CellSize+param.CellSize/2., sP.getPointF().y()*param.CellSize+param.CellSize/2., 0);
//            long x = p_ectract.x / param.CellSize;
//            long y = p_ectract.y/ param.CellSize;
//            double z = this->param.topoElevation->getValue(x,y);
//            pointlist.push_back(Point(x *param.CellSize+param.CellSize/2., y *param.CellSize+param.CellSize/2.,z));


//        }

//        for (int j = 0; j < counter-1; j++) {
//            edges.push_back(Edge(j,j+1));
//        }

//        std::stringstream ss;
//        ss << "Sewer" << i;

//        foreach (Edge e, edges) {
//            std::stringstream ss;
//            ss << "Conduit_" << edge_counter;

//            Point p1 = pointlist[e.getID1()];
//            Point p2 = pointlist[e.getID2()];

//            std::vector<Point> plist;
//            plist.push_back(p1);
//            plist.push_back(p2);

//            std::vector<Edge> elist;
//            elist.push_back(Edge(0,1));
//            param.MainSewerNetwork->setPoints(ss.str(), plist);
//            param.MainSewerNetwork->setEdges(ss.str(), elist);
//            edge_counter++;
//        }
//        for (int i = 0; i < pointlist.size(); i++) {
//            Point p = pointlist[i];
//            std::stringstream ss;
//            ss << "Shaft_" << shaft_counter;
//            std::vector<Point> plist;
//            plist.push_back(p);
//            std::vector<std::string>::const_iterator it;
//            it = std::find(param.MainSewerNetwork->getPointsNames().begin(), param.MainSewerNetwork->getPointsNames().end(), ss.str());
//            if (it != param.MainSewerNetwork->getPointsNames().end()) {
//                Logger(Error)  << "Duplicate Value";
//            }
//            param.MainSewerNetwork->setPoints(ss.str(), plist);
//            Attribute attr;
//            attr.setAttribute("Z", p.z);
//            attr.setAttribute("D", 3);
//            if (i == pointlist.size() - 1)
//                attr.setAttribute("WWTP", 1);
//            param.MainSewerNetwork->setAttributes(ss.str(), attr);



//            shaft_counter++;

//        }
//    }



//}

//void MainSewer::findInitSewer() {


//    VIBe::struc SewerSegment;
//    VIBe::segment buffer_(0,&mainSewer);

//    //layer_[cityCenters]->setSwitch(2);

//    //struc test;
//    //Sewer.append(test);


//    //connectBoarder for Sewer and WWTP
//    //with less distance
//    //for each center if different river sides
//    //culvert
//    std::vector<Point> vP ;
//    std::vector<std::string> names;
//    names = param.CityCenters->getPointsNames();
//    foreach(std::string name, names) {
//        foreach( Point p, param.CityCenters->getPoints(name)) {
//            vP.push_back(Point(p.x/param.CellSize, p.y/param.CellSize, p.z));
//        }
//    }
//    Point startSewerP(vP[0].getX(), vP[0].getY(), param.topoElevation->getValue(vP[0].getX(),vP[0].getY()));
//    std::vector<Point> wwtp;
//    foreach( Point p, param.WWTP->getPoints("WWTP")) {
//        wwtp.push_back(Point(p.x/param.CellSize, p.y/param.CellSize, p.z));
//    }
//    Point endSewerP(wwtp[0].getX(), wwtp[0].getY(), param.topoElevation->getValue(wwtp[0].getX(),wwtp[0].getY()));

//    //setBasisLength(sewerBasisLength);
//    //setDistance(sewerDistanceToRiver);

//    std::vector<Point> bufferP=(findWayPoints(startSewerP,endSewerP, param.topoStatus));
//    foreach(Point p, bufferP) {
//        VIBe::point P(p.getX(), p.getY(), p.getZ());
//        SewerSegment.addPoint(P);
//    }
//    Sewer.append(SewerSegment);
//    Sewers.append(SewerSegment);


//    //main sewer for further evalutation
//    //in sewerConnectivity layer
//    //-------------------------//
//    QList<VIBe::point> Points_;
//    VIBe::point P_;
//    VIBe::struc S_;
//    QPointF p;

//    //Points= QList<point> of struc of QList<struc> Roads
//    //drawPoint
//    //-------------------------//
//    for (int i=0;i<Sewers.size();i++)
//    {
//        S_=Sewers.value(i);
//        Points_=S_.getP();

//        for (int j=0;j<Points_.size();j++)
//        {
//            double x = Points_.value(j).itsX();
//            double y = Points_.value(j).itsY();
//            double z = param.topoElevation->getValue(x, y);
//            p.setX(Points_.value(j).itsX());
//            p.setY(Points_.value(j).itsY());
//            buffer_.addPoint(p,z,param.CellSize);
//        }
//    }
//    mainSewer.addSegment(buffer_);
//    //draw Point field
//    //-------------------------//
//    //setDrawLayer(initSewerConnectivity);
//    //setDrawVal(1);
//    //setDrawCon(sewerConnectivity);
//    //field plot
//    U_drawPoints(Points_,1, param.initSewerConnectivity, 1, param.sewerConnectivity);
//    //-------------------------//
//    //draw Point field
//    //-------------------------//
//    //setDrawLayer(SewerConnectivity);
//    //setDrawVal(1);
//    //setDrawCon(sewerConnectivity);
//    //field plot
//    U_drawPoints(Points_,1, param.initSewerConnectivity, 1, param.sewerConnectivity);
//    //-------------------------//
//    //-------------------------//

//}
//void MainSewer::findSecSewer(int num)
//{






//    //old calculation
//    VIBe::struc SewerSegment;

//    std::vector<Point> vP ;
//    std::vector<std::string> names;
//    names = param.CityCenters->getPointsNames();
//    foreach(std::string name, names) {
//        foreach( Point p, param.CityCenters->getPoints(name)) {

//            vP.push_back(Point(p.x/param.CellSize, p.y/param.CellSize, p.z));
//        }
//    }

//    Point startSewerP(vP[num].getX(), vP[num].getY(), param.topoElevation->getValue(vP[num].getX(),vP[num].getY()));
//    std::vector<Point> wwtp;
//    foreach( Point p, param.WWTP->getPoints("WWTP")) {
//        wwtp.push_back(Point(p.x/param.CellSize, p.y/param.CellSize, p.z));
//    }
//    Point endSewerP(wwtp[0].getX(), wwtp[0].getY(), param.topoElevation->getValue(wwtp[0].getX(),wwtp[0].getY()));



//    //sewer parameters set in initCity constructor
//    //setBasisLength(sewerBasisLength);
//    //setDistance(sewerDistanceToRiver);



//    //get sewer points without interaction
//    std::vector<Point> bufferP=findWayPoints(startSewerP,endSewerP, param.topoStatus);

//    //connector points
//    QList<VIBe::point> Connector;
//    double val=0.0;

//    //test the connection of city point and WWTP
//    //if there is a existing sewer close to the new one
//    for(int i = 0; i < bufferP.size(); i++) {

//        VIBe::point test;
//        test.setP(bufferP[i].getX(), bufferP[i].getY(), bufferP[i].getZ());
//        double testX=test.itsX();
//        double testY=test.itsY();
//        VIBe::point p;
//        int count=0;
//        int sewers = 0;
//        double testDistance=param.Width;

//        val=0.0;
//        val=param.initSewerConnectivity->getValue(int(testX),int(testY));

//        //if there is another sewer near
//        //search Connector point in Sewers
//        //(with lowest distance)
//        //and connect SewerSegment to Connector point
//        //-------------------------//
//        if (val>0)
//        {
//            //to SewerSegments maximum
//            for (int j=0;j<num;j++)
//            {

//                Connector=(Sewers.value(j)).getP();

//                //for every point
//                //evaluate distance to actual point
//                //-------------------------//
//                for (int c=0;c<Connector.size()-1;c++)
//                {

//                    VIBe::point connector=Connector.value(c);
//                    double connectX=connector.itsX();
//                    double connectY=connector.itsY();

//                    double distance=fabs(testX-connectX)*
//                            fabs(testX-connectX)+
//                            fabs(testY-connectY)*
//                            fabs(testY-connectY);

//                    distance = sqrt(distance);


//                    //evaluate minimum distance
//                    //-------------------------//
//                    if (testDistance>distance)
//                    {
//                        testDistance=distance;
//                        p=connector;
//                        count =  c;
//                        sewers = j;
//                    }
//                    //-------------------------//

//                }
//                //-------------------------//
//            }
//            i=bufferP.size();
//            SewerSegment.addPoint(p);

//        }
//        //-------------------------//

//        //if val == 0 new other sewer is close to new one
//        else
//        {

//            SewerSegment.addPoint(VIBe::point(bufferP[i].getX(), bufferP[i].getY(),bufferP[i].getZ()));
//        }
//    }
//    Sewer.clear();
//    Sewer.append(SewerSegment);
//    Sewers.append(SewerSegment);


//    //main sewer for further evalutation
//    //in sewerConnectivity layer
//    //-------------------------//
//    QList<VIBe::point> Points__;
//    QList<VIBe::point> Points;
//    VIBe::point P__;
//    VIBe::struc S__;
//    VIBe::struc S;
//    QPointF p(0,0);


//    //Points= QList<point> of struc of QList<struc> Roads
//    //drawPoint
//    //-------------------------//
//    //buffer segment for evaluation of close sewers

//    for (int i=0;i<Sewer.size();i++)
//    {
//        S=Sewer.value(i);
//        Points=S.getP();

//        if (Points.size()>1)
//        {
//            VIBe::segment buffer(0,&mainSewer);
//            for (int j=0;j<Points.size();j++)
//            {
//                p.setX(Points.value(j).itsX());
//                p.setY(Points.value(j).itsY());
//                buffer.addPoint(p,1,param.CellSize);
//            }
//            mainSewer.addSegment(buffer);
//        }

//    }


//    for (int i=0;i<Sewer.size();i++)
//    {
//        S__=Sewers.value(i);
//        Points__=S__.getP();
//    }




//    U_drawPoints(Points,1, param.initSewerConnectivity, 1, param.sewerConnectivity);
//    //-------------------------//
//    //draw Point field
//    //-------------------------//
//    //setDrawLayer(SewerConnectivity);
//    //setDrawVal(1);
//    //setDrawCon(sewerConnectivity);
//    //field plot
//    U_drawPoints(Points,1, param.initSewerConnectivity, 1, param.sewerConnectivity);





//};
//void MainSewer::drawPoints(double **pointArray, int pointCount, int field, RasterData * drawLayer, int drawVal, int drawConnectivity)
//{
//    double dx=0;
//    double dy=0;
//    double de=0;
//    double currentElevation=0;
//    int vs=0;
//    int riverWidth=1;
//    //layer_[drawLayer]->setSwitch(2);

//    for (int s=0;s<pointCount;s++)
//    {
//        dx=pointArray[s+1][0]-pointArray[s][0];
//        dy=pointArray[s+1][1]-pointArray[s][1];
//        de=pointArray[s+1][2]-pointArray[s][2];
//        currentElevation=0;
//        vs=s;

//        if (fabs(dx)>=fabs(dy))
//        {

//            if (dx<0) {vs+=1;}

//            for (int x=0;x<=fabs(dx);x++)
//            {

//                double y = 0;
//                if (dy==0)
//                {
//                    y=pointArray[vs][1];
//                }
//                else
//                {
//                    y= ((dy/dx)*(x))+pointArray[vs][1];
//                }
//                if (dx==0){currentElevation=pointArray[vs][2];}
//                if (dx==0){dx=1;}
//                else {currentElevation=pointArray[vs][2]+de*x/fabs(dx);}

//                for (int actualRiverWidth=0;actualRiverWidth<=(riverWidth/param.CellSize);actualRiverWidth++)
//                {
//                    if ((x+pointArray[vs][0]+actualRiverWidth)<param.Width)
//                    {
//                        drawLayer->setValue(x+pointArray[vs][0]
//                                            +actualRiverWidth,floor(y),drawVal);
//                        drawLayer->setValue(x+pointArray[vs][0]
//                                            +actualRiverWidth, ceil(y),drawVal);

//                        if (field ==1)
//                        {
//                            drawReach(x+pointArray[vs][0], y,drawLayer,
//                                      drawConnectivity, drawVal);
//                        }


//                    }
//                }
//            }
//        }
//        if ((fabs(dx)<fabs(dy)))
//        {
//            if (dy<0) {vs+=1;}

//            for (int y=0;y<=fabs(dy);y++)
//            {
//                double x = 0;
//                if (dx==0)
//                {
//                    x=pointArray[vs][0];
//                }
//                else
//                {
//                    x= ((dx/dy)*(y))+pointArray[vs][0];
//                }

//                if (dy==0) {currentElevation=pointArray[vs][2];}
//                if (dy!=0) {currentElevation=pointArray[vs][2]+de*y/fabs(dy);}


//                for (int actualRiverWidth=0;actualRiverWidth<=(riverWidth/param.CellSize);actualRiverWidth++)
//                {

//                    drawLayer->setValue(floor(x),
//                                        y+pointArray[vs][1]+actualRiverWidth,drawVal);
//                    drawLayer->setValue(ceil(x),
//                                        y+pointArray[vs][1]+actualRiverWidth,drawVal);
//                    if (field ==1)
//                    {
//                        drawReach(x, y+pointArray[vs][1],drawLayer,
//                                  drawConnectivity, drawVal);
//                    }

//                }
//            }
//        }
//    }
//};
//void MainSewer::U_drawPoints(QList<VIBe::point>& bufferP, int field, RasterData * drawLayer, int drawVal, int drawConnectivity)
//{
//    //ugly visualisation test hack
//    //-------------------------//
//    double **Points;
//    Points=new double*[bufferP.count()];
//    for (int i=0;i<bufferP.count();i++)
//    {
//        Points[i]=new double[3];
//        Points[i][0]=(bufferP.value(i)).itsX();
//        Points[i][1]=(bufferP.value(i)).itsY();
//        Points[i][2]=(bufferP.value(i)).itsVal();
//    }
//    drawPoints(Points,bufferP.count()-1,field, drawLayer, drawVal, drawConnectivity);
//    delete [] Points;
//    //-------------------------//

//};
//void MainSewer::drawReach(double x,
//                          double y, RasterData * rdata,
//                          double radius, double maxVal)
//{
//    //distance to city center function
//    //-------------------------//
//    //value = e^A*(R-r) for r>R [valMin,1]
//    //						r<R {1}
//    //value = 0.01 at r=radius
//    //minimal value = valMin = 0.01
//    double value=0;	//<=1
//    double valMin=0.001;
//    double A=0;		//parameter for exponent function
//    A=(log(0.01))/(-radius);
//    //-------------------------//




//    for (int i=-radius;i<radius;i++)
//    {
//        for (int j=-radius;j<radius;j++)
//        {
//            double distance;
//            distance=(i*i+j*j);
//            distance=sqrt(distance);
//            value=maxVal*exp(A*(- distance));
//            if ((x+i)>0 && (x+i)<rdata->getWidth() && (y+j)>0 && (y+j)<rdata->getHeight())
//            {
//                if (value>rdata->getValue(x+i,y+j))
//                {
//                    if (value>maxVal) {value = maxVal;}
//                    if (value<valMin) {value = valMin;}
//                    rdata->setValue(floor(x+i), floor(y+j), value);
//                }
//            }
//        }
//    }


//};
