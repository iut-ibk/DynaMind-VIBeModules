#include "dmcompilersettings.h"

#ifndef __d_topoCreator
#define __d_topoCreator




#include "def.h"
#include "dataLayer.h"
#include "segmentOrganizer.h"
#include "Interpolation.h"


namespace VIBe{

class riverStruc;


/** @brief topology generator
  *
  */
class DM_HELPER_DLL_EXPORT topoCreator
{
	
public:
	topoCreator(dataLayer **pL);
    ~topoCreator();
	void generateTopology(dataLayer **pL);

	double getrrElev();

	//setter river Parameter 
	//-------------------------//
	void setRiverSlope(double r);
	void setRiverWidth(double r);
	void setRiverdHeight(double r);
	void setBasisLength(double r);
	void setMeanderWidth(double r);
	void setNoMinorRivers(int r);
	void setNoSinuosity(int r);

	//setter relief Parameter
	//-------------------------//
	void setBasicElevation(double r);
	void setElevationSlope(double r);
	void setMaximaldHeight(double r);
	void setMaxHilldHeight(double r);
	void setMaxNoHills(int r);


	//points container
	//-------------------------//
	segmentOrganizer mainRiver;
		QVector <sPoint> mainRiverSpline;
	segmentOrganizer sinuosities;
		QVector<QVector <sPoint> > sinuositiesSpline;
	segmentOrganizer minorRivers;
		QVector<QVector <sPoint> > minorRiverSpline;

  private:
	//topology lattice attributes
	//set in Constructor topoCreator::topoCreator
	//or loaded with input file
	//-------------------------//
	int cellSize_;
	int width_;
	int height_;
	dataLayer **layer_;
	QString inputFile;
	//-------------------------//


	//riverCreator Parameters
    //set in topoCreator::loadRiverParam(int kind)
	//-------------------------//
	static const int riverPoints=100;
	riverStruc *id[riverPoints];

	double riverSlope;
	double riverWidth;
	double rrElev;
	int meanderFactor;
	int meanderWidth;
	double basisLenght;

	//sinuosity
	int noSinuosity;
	int noMinorRivers;
	int quadrant[4];


	//for linear interpolation
	int smoothing;
	double alpha;
	double alphaTol;
	double bendFac;
	//-------------------------//

	//topoCreator Parameters
	//set in topoCreator::loadTopoParam(int kind)
	//-------------------------//
	double elev;
	double elevSlope;
	double maxdHeight;
	double maxHilldHeight;
	int maxNoHills;
	int *yData;

	//function generateTopology
	//-------------------------//
	void loadRiverParam(int kind);//kind of river

	void loadTopoParam(int kind);

   	void riverCreate_1();
	void riverCreate_2();
	void riverCreate_3();
		void basicRiver();
		void defineRiverArea();
		void defineRiverArea2();
		void defineCRiverArea();
		void adjustRiver();
		void RiverAreaElevBaseTrend();
		void createMainRiver();
	void riverSinuosity();
	void drawPoints(dataLayer **pL,double **pointsArray,int pointCount, int smoothing_);
	//QPainterPath drawRiver(/*dataLayer **pL, double **pointsArray, int pointCount*/);
	void topoRiverArea(dataLayer **pL);
	void createminorRivers();
		void createminorRivers2();
		QList<QPointF> findStartEndPointMinorRiver();
	void createHill(dataLayer **pL);
	bool createHill2(dataLayer **pL, QImage &initialImage, int maxGroundLength,int minGroundLength, int angleSegments, int amountOfHeightLevels, double hillHeight);
	void createHills(dataLayer **pL);
    void paintGroundPolygon(QPainter *p,QPolygonF poly);

	//-------------------------//
};


/** @brief river container
  *
  */
class riverStruc
{
	private:
		int x;
		int y;
		double elevation;
		double nVector[2]; //normal unit vector
		int fP; //fixed point
	public:
		riverStruc()
		{
			x=0;
			y=0;
			fP=0;
			elevation=0;
			for (int i=0;i<2;i++)
			{
				nVector[i]=0;
			}
		};
		void setPoint(int _x, int _y, double _elevation, double nX, double nY)
		{
			x=_x;
			y=_y;
			nVector[0]=nX;
			nVector[1]=nY;
			elevation=_elevation;
		};

		void setfP(int fP_)
		{
			fP=fP_;
		};

		int getX(){return x;};
		int getY(){return y;};
		double getElev() {return elevation;};
		double *getnVec() {return nVector;};
		int getfP() {return fP;};


};
};

#endif
