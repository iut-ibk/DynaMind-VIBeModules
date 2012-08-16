#ifndef __struc
#define __struc
#include "dmcompilersettings.h"
//#include <iostream>


//#include "editor.h"
//#include "dataLayer.h"
//#include "def.h"
#include <QtGui>
//#include <QLine>


namespace VIBe{

/** @brief coordinate and value administration
  *
  */
class DM_HELPER_DLL_EXPORT  point
{
private:
	double x;
	double y;
	double val;
	int pointID;

public:

	//constructors
	//-------------------------//
	point() 
	{
		x=0;
		y=0;
		val=0;

	};

	point(double x_, double y_,double val_)
	{
		x=x_;
		y=y_;
		val=val_;		
	};
	//-------------------------//

	double itsX() {return x;};
	double itsY() {return y;};
	double itsVal() {return val;};

	void setP(double x_, double y_,double val_)
	{
		x=x_;
		y=y_;
		val=val_;		
	};
	

};

/** @brief point organizer
  *
  */
class DM_HELPER_DLL_EXPORT  struc
{
private:	
	int order;
	int id;


public:
	//point array
	//-------------------------//
	//point **p;
	QList<point> p;
	//-------------------------//

	//constructor
	//-------------------------//
	struc();
	//struc(int numberPoints);
	
	//-------------------------//
	
	void addPoint(point p_);

	void setOrder(int o) {order=o;};
	void setId(int i) {id=i;};

	void setStart(struc *struc, int pointNumber);
	void setEnd(struc *struc,int pointNumber);
	
	int getOrder() {return order;};
	QList<point> getP() {return p;};
	
};

};

#endif
