


#include "struc.h"

namespace VIBe{

struc::struc()
{
	order=0;
	id=0;
	//start=NULL;
	//end=NULL;
	//p=NULL;

};

//struc::struc(int numberPoints)
//{
//	order=0;
//	id=0;
//	//start=NULL;
//	//end=NULL;
//
//	//p=new point*[numberPoints];
//	//for (int i=0;i<numberPoints;i++)
//	//{
//	//	p[i]= new point(0,0,0);
//	//}	
//
//};

void struc::addPoint(point p_)
{
	p.append(p_);

};

//void struc::setStart(struc *struc, int pointNumber)
//{
//	start=struc;
//};
//
//void struc::setEnd(struc *struc,int pointNumber)
//{
//	end=struc;
//};

};