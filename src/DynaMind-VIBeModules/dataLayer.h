#include "dmcompilersettings.h"

#ifndef __datalayer
#define __datalayer

#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include <QtGui>
#include <QLine>
#include <QPainterPath>

#include "def.h"
//#include "vSys.h"

namespace VIBe{

class vSys;

/** @brief raster data layer
  *
  *	administrates data in a width x height array
  */
class DM_HELPER_DLL_EXPORT dataLayer
{

 public:
	dataLayer(int width_, int height_, int cellSize_);
	dataLayer();
	~dataLayer();

	void setVal(int x, int y, double val);
        double getVal(int x, int y) const;
	void addVal(int x, int y, double addVal);
        void clean();
	double randVal(double rangeLow, double rangeHigh, double median);

        double neighbour(int x, int y) const;

        int getcellSize() const {return cellSize;};
        int getheight() const {return height;};
        int getwidth() const {return width;};
	void setmHeight(int m) {mHeight=m;}
	int getmHeight() {return mHeight;}
	void setSwitch(int s) {layerswitch=s;};
	int showSwitch() {return layerswitch;};

    void saveDataToFile( QTextStream *in );
    void loadDataFromFile( QTextStream *out );


	dataLayer* createArea();
	dataLayer* rotatedArea(int alpha);

	dataLayer* Area_;
	dataLayer* rotArea_;

	 
protected:
	int width;
	int height;
	int mHeight;
	int cellSize;
	double *data;
	double *bdata;
	vSys *parentVSys;
	int layerswitch;


};

};


#endif
