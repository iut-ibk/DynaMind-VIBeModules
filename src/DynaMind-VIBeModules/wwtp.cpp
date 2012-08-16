//#include "wwtp.h"

//VIBe_DECLARE_NODE_NAME( WWTP, Vibe )

//WWTP::~WWTP()
//{
//}
//void WWTP::run()
//{
//    param.WWTP->clear();
//    Point wwtp = findWWTP();
//    param.WWTPlant->setSize(param.Width, param.Height, param.CellSize);
//    param.WWTPlant->clear();

//    drawReach(wwtp.getX()/param.CellSize,wwtp.getY()/param.CellSize,param.WWTPlant ,param.WWTPAction/param.CellSize,1);

//    std::vector<Point> vWWTP;
//    vWWTP.push_back(wwtp);
//    param.WWTP->setPoints("WWTP", vWWTP);

//}
//void WWTP::drawReach(double x,
//                     double y, RasterData  * rdata,
//                     double radius, double maxVal)
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

//Point WWTP::findWWTP()
//{
//    double minX=0;
//    double Y=0;
//    int sign=rand()%(2);
//    double x=0;
//    double WWTPRadius = param.WWTPRadius/param.CellSize;
//    //layer_[cityCenters]->setSwitch(2);
//    int numberCenters = 0;
//    foreach( Point p, param.CityCenters->getPoints("Positions")) {
//        minX = (p.getX()+p.getZ())/param.CellSize;
//        numberCenters++;
//    }

//    //minX=Centers[numberCenters-1]->itsX+
//    //Centers[numberCenters-1]->itsmHeight;
//    if (minX>param.Width){
//        //minX=width-0.5*Centers[numberCenters-1]->itsmHeight;
//        Point p = param.CityCenters->getPoints("Positions")[numberCenters-1];
//        minX = param.Width-0.5*p.getZ()/param.CellSize;
//    }

//    x=(param.Width-minX)*0.5;
//    int fx=floor(x);
//    x=(minX+fx+rand()%(fx));
//    if (x>=param.Width)
//    {
//        x=param.Width-WWTPRadius;
//    }


//    if (sign==0)
//    {
//        for (int i=0;i<param.Height;i++)
//        {
//            //param.topoStatus.getValue()
//            if (param.topoStatus->getValue(floor(x),i)==10)
//            {
//                Y=i-param.distanceToRiver/param.CellSize;
//                i=param.Height;
//            }
//        }
//    }
//    else
//    {
//        for (int i=param.Height-1;i>0;i--)
//        {
//            if (param.topoStatus->getValue(floor(x),i)==10)
//            {
//                Y=i+param.distanceToRiver/param.CellSize;
//                i=0;
//            }
//        }
//    }
//    Point p;
//    p.x = x*param.CellSize+param.CellSize/2.;
//    p.y = Y*param.CellSize+param.CellSize/2.;
//    p.z = -1;
//    return p;

//};


//WWTP::WWTP() {

//    param.Width = 400;
//    param.Height = 240;
//    param.CellSize = 20;
//    param.distanceToRiver = 140;
//    param.WWTPRadius = 2000;
//    param.WWTPAction = 500;
//    this->addParameter("Width", VIBe2::LONG, &param.Width);
//    this->addParameter( "Height", VIBe2::LONG, &param.Height );
//    this->addParameter( "CellSize", VIBe2::DOUBLE, & param.CellSize);
//    this->addParameter( "distanceToRiver", VIBe2::DOUBLE, &param.distanceToRiver);
//    this->addParameter( "WWTPRadius", VIBe2::DOUBLE, &param.WWTPRadius);
//    this->addParameter( "WWTPAction", VIBe2::DOUBLE, &param.WWTPAction);
//    this->addParameter("CityCenters", VIBe2::VECTORDATA_IN, &param.CityCenters);
//    this->addParameter("topoStatus", VIBe2::RASTERDATA_IN, &param.topoStatus);
//    this->addParameter("WWTPlant", VIBe2::RASTERDATA_OUT, &param.WWTPlant);
//    this->addParameter("WWTP",  VIBe2::VECTORDATA_OUT, &param.WWTP);

//}
