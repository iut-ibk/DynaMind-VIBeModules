//#ifndef MAINSEWER_H
//#define MAINSEWER_H
//#include "dmmodule.h"
//#include "segmentOrganizer.h"
//#include "struc.h"

//using namespace DM;

//class DM_HELPER_DLL_EXPORT MainSewer : public  DM::Module {

//DM_DECLARE_NODE (MainSewer)
//        struct Parameter {
//    long Width;
//    long Height;
//    double CellSize;
//    double sewerBasisLength;
//    double sewerDistanceToRiver;
//    double sewerConnectivity;

//    double sewerBasisLength_in;
//    double sewerDistanceToRiver_in;
//    double sewerConnectivity_in;
//    RasterData * topoElevation;
//    RasterData * topoStatus;
//    RasterData* initSewerConnectivity;

//};
//public:
//MainSewer();
//virtual ~MainSewer();
//void run();

//private:
//DM::View * topoElevation;
//DM::View * topoStatus;
//DM::View * initSewerConnectivity;

//DM::View  vWWTP;
//DM::View vCityCenters;
//DM::View vMainSewerNetwork;

//Parameter param;
//void findInitSewer();
//VIBe::segmentOrganizer mainSewer;
//std::vector<Point> findWayPoints(Point startPoint, Point endPoint, RasterData * topoStatus);
//void U_drawPoints(QList<VIBe::point>& Points, int field, RasterData * drawLayer, int drawVal, int drawConnectivity);
//void drawPoints(double **pointArray, int pointCount, int field, RasterData * drawLayer, int drawVal, int drawConnectivity);
//QList<VIBe::struc> Sewer;
//QList<VIBe::struc> Sewers;

//void drawReach(double x, double y, RasterData * rdata, double radius, double maxVal);
//void findSecSewer(int num);
//};

//#endif // MAINSEWER_H
