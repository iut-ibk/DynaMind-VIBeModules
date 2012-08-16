




#include "d_topoCreator.h"
#include "editor.h"


namespace VIBe{


/** @brief riverCreate_3()
  *
  * creates  main river
  * and elevation base trend for river area
  */
void topoCreator::riverCreate_3()
{
	layer_[topoStatus]->setSwitch(2);
	layer_[topoElevation]->setSwitch(2);


	//define basic river geometry based on a quadratic equation
	//basicRiver();

	/*
	based on basicRiver, creates RiverArea
	in topoStatus with value 6 where river can meander
	*/
	//defineRiverArea();
	//defineRiverArea2() instead of basicRiver() and defineRiverArea()
	defineRiverArea2();

	//defines a more complex rand randomize River area to meander
	for (int i=0; i<3; i++) defineCRiverArea();


	//creates a Elevation base trend for area
	RiverAreaElevBaseTrend();

	//creates main River in the meander Area
	createMainRiver();
}

/** @brief basicRiver()
  *
  * creates basic river based on a quadratic equation
  */
void topoCreator::basicRiver()
{
	//create mainRiver
	//-------------------------//


	//river variation attributes for each river section
	double drandSlope=0;

	//faktors for polynom 2nd order
	double a=0;
	double b=0;
	double c=0;


	//river geometry as polynon 2nd order
	//faktors for polynom 2nd order
	a=((0+(rand()%(100)))*0.04)*0.01-0.02;
	b=((0+(rand()%(100)))*3)*0.01-1.5;
	//c=((0+(rand()%(100)))*80)*0.01-160;
	c=((0+(rand()%(100)))*80)*0.01+165;

	if (c<50) //that river reaches xmax && y>ymin
	{
		if(a<0) a=-a;
		if(b>0) b=-b;
	}
	if (c>50) //that river reaches xmax && y<ymax
	{
		if(a>0) a=-a;
		if(b<0) b=-b;
	}

	//factors to scale to datalayer
	double heightFactor=(height_)*0.01;
	double widthFactor=(width_)*0.01;

	//unit normal vector for each river section
	double nVecX=0;
	double nVecY=0;
	double lVec=0;


	//for each x value one y value
	for (int h=0; h<riverPoints; h++)
	{


		//variation of Slope for each section +-5% of riverslope
		drandSlope=riverSlope*(1+0.05*(0+(rand()%(200))-100)*0.01);

		if (h>0)
		{
			nVecY=((id[h]->getY())-(id[h-1]->getY()));
			nVecX=-((id[h]->getX())-(id[h-1]->getX()));
			lVec=sqrt(nVecX*nVecX+nVecY*nVecY);
			nVecX/=lVec;
			nVecY/=lVec;
		}

		id[h]->setPoint(
			   h*widthFactor,
			   floor(0.25*(a*h*h+b*h+c)*heightFactor),
			  (elev-rrElev)-h*cellSize_*drandSlope,
			   nVecY,
			   nVecX);

		if (h==1)
		{
			h=0;
			id[h]->setPoint(
				   h*widthFactor,
				   floor(0.25*(a*h*h+b*h+c)*heightFactor),
				  (elev-rrElev)-h*cellSize_*drandSlope,
				   nVecY,
				   nVecX);
			id[h]->setfP(1);
			h=1;

		}
		if (h==riverPoints-1)
		{
			id[h]->setfP(1);
		}



	}



};
/** @brief defineRiverArea2()
  *
  * defines Area in which the main river can meander
  * as offset from basic river
  */
void topoCreator::defineRiverArea2()
{
	segmentOrganizer RiverArea;
	segment riverArea(0,&RiverArea);
	editor visArea(layer_);

	double startY=(30+rand()%40)*0.01*height_;
	double midX=(30+rand()%40)*0.01*width_;
	double midY=(30+rand()%40)*0.01*height_;
	double endY=(30+rand()%40)*0.01*height_;

	QPointF start(0,startY);
	QPointF start1(1,startY);
	QPointF mid(midX,midY);
	QPointF end1(width_-2,endY);
	QPointF end(width_-1,endY);

	riverArea.addPoint(start,1,meanderWidth*cellSize_*2);
	riverArea.addPoint(start1,1,meanderWidth*cellSize_*2);
	riverArea.addPoint(mid,1,meanderWidth*cellSize_*2);
	riverArea.addPoint(end1,1,meanderWidth*cellSize_*2);
	riverArea.addPoint(end,1,meanderWidth*cellSize_*2);

	RiverArea.addSegment(riverArea);
	visArea.paintOrga(RiverArea,topoStatus,Interpolation::NatCubic,6);

}

/** @brief defineRiverArea()
  *
  * defines Area in which the main river can meander
  * as offset from basic river
  */
void topoCreator::defineRiverArea()
{
	//
	//-------------------------//

	double dY=meanderWidth;
	int dy=floor(dY);
	layer_[topoStatus]->setmHeight(dy);
	dY=1.0*dy;
	//-------------------------//


		for (int w=0;w<width_;w++)
		{
			dy=floor(dY);
			for (int hh=0;hh<height_;hh++)
			{
				for (int h=0; h<riverPoints; h++)
				{
					if (w>=id[h]->getX()/* &&
						w<id[h+1]->getX()*/)
					{
						if ((hh<(id[h]->getY()-dy)) ||
							(hh>(id[h]->getY()+dy)))
						{
							layer_[topoStatus]->setVal(w,hh,0);

						}
						else
						{
							layer_[topoStatus]->setVal(w,hh,6);
						}




					}
				}
			}
		}

	//-------------------------//


};

/** @brief defineCRiverArea()
  *
  * defines complex Area in which the main river can meander
  * as spline in river Area with width depending on meanderWidht
  */
void topoCreator::defineCRiverArea()
{
	/*
	local segment Organizer for defining complex Area (area
	for river to meander, topoStatus == 6)
	first and last point with less width to tackle
	start and end problem of spline interpolation
	*/
	segmentOrganizer complexArea;
	segment seg(0,&complexArea);

	int currentX=0;
	double dX=0;
	int currentY=0;
	double dY=0;
	double currentZ=0.001;
	int minRiverY=height_;
	int maxRiverY=0;
	double basisLenght_=basisLenght*1.0; //[m]


	dX=basisLenght_;
	dX/=cellSize_;

	/*
	starting from x=0 until x>width
	with steps according to dX=basisLenght * 25% - 75%
	for each x evaluate minumal and maximal y within
	the area to meander (topoStatus == 6)
	*/
	for (int i=0;i<width_;i+=dX)
	{
		minRiverY=height_;
		maxRiverY=0;

		dX=basisLenght_;
		dX+=(-basisLenght_*0.25+rand()%int(basisLenght_*0.5));
		dX/=cellSize_;
		currentX=i;
		/*
		if still in dataLayer area
		evaluate y values with in the simple meander Area
		created with defineRiverArea()
		*/
		if (currentX<width_)
		{


			//find currentY according to currentX
			//-------------------------//
			for (int j=0;j<height_;j++)
			{
				if (layer_[topoStatus]->getVal(currentX,j) == 6.0 )
				{
					if (j<minRiverY) {minRiverY=j;}
					if (j>maxRiverY) {maxRiverY=j;}
				}
			}
			dY=(maxRiverY-minRiverY);
			currentY=(minRiverY+dY/4+rand()%int(dY/2));

			QPointF p(currentX,currentY);

			/*
			for the start point the areaWidth is set to 20 % of
			usual areaWidth (tackle spline interpolation start problem)
			*/
			if (currentX==0)
			{
				seg.addPoint(p,
							currentZ,
							dY*cellSize_*0.2);
			}
			else
			{
				/*
				if current X is within 40 - 60% of width of dataLayer
				the areaWidth is set to 130% of usual
				*/
				double areaWidth=dY*cellSize_;
				if (currentX> width_*0.3 && currentX<width_*0.7)
				{
					areaWidth*=1.05;
				}

				seg.addPoint(p,
							currentZ,
							areaWidth);
			}
			//-------------------------//
		}
	}
			//find currentY according to endPoint
			//-------------------------//
			//if ((currentX+basisLenght_/cellSize_)>=width_)
			{
				minRiverY=height_;
				maxRiverY=0;
				for (int j=0;j<height_;j++)
				{
					if (layer_[topoStatus]->getVal(width_-1,j) == 6.0 )
					{
						if (j<minRiverY) {minRiverY=j;}
						if (j>maxRiverY) {maxRiverY=j;}
					}
				}
				dY=(maxRiverY-minRiverY);
				currentY=(minRiverY+dY/4+rand()%int(dY/2));

				/*
				for the end point the areaWidth is set to 20 % of
				usual areaWidth (tackle spline interpolation end problem)
				*/

				QPointF p1(width_-2,currentY);
				seg.addPoint(p1,
							currentZ,
							dY*cellSize_*0.2);
				QPointF p(width_-1,currentY);
				seg.addPoint(p,
							currentZ,
							dY*cellSize_*0.2);
				//i=width_;
			}
			//-------------------------//
	//}


	complexArea.addSegment(seg);
	editor visual(layer_);
	visual.paintOrga(complexArea,topoStatus,Interpolation::NatCubic,6);
};


/** @brief createMainRiver()
  *
  * creates main river in dataLayers topoStatus and topoElevation
  * parameter
  */
void topoCreator::createMainRiver()
{
	segment seg(0,&mainRiver);

	int currentX=0;
	double dX=0;
	int currentY=0;
	double dY=0;
	double currentZ=0;
	int minRiverY=height_;
	int maxRiverY=0;
	double actualRiverWidth;


	dX=basisLenght;
	dX/=cellSize_;

	for (int i=0;i<width_;i+=dX)
	{
		minRiverY=height_;
		maxRiverY=0;

		dX=basisLenght;
		dX+=(-basisLenght*0.1+rand()%int(basisLenght*0.8));
		dX/=cellSize_;
		currentX=i;
		if (currentX<width_)
		{


			//find currentY according to currentX
			//-------------------------//
			for (int j=0;j<height_;j++)
			{
				if (layer_[topoStatus]->getVal(currentX,j) == 6.0 )
				{
					if (j<minRiverY) {minRiverY=j;}
					if (j>maxRiverY) {maxRiverY=j;}
				}
			}

			//no river outside topoStatus==6
			minRiverY+=riverWidth/cellSize_;
			maxRiverY-=riverWidth/cellSize_;


			dY=(maxRiverY-minRiverY);
			currentY=(minRiverY+dY/4+rand()%int(dY/2));

			QPointF p(currentX,currentY);
			actualRiverWidth=riverWidth;
			actualRiverWidth*=((90+rand()%(30))*0.01);
			currentZ=layer_[topoElevation]->getVal(currentX,currentY);
			currentZ-=rrElev;
			if (currentX==0)
			{
				QPointF p1(currentX+1,currentY);
				seg.addPoint(p1,
						currentZ,
						actualRiverWidth);
			}
			seg.addPoint(p,
						currentZ,
						actualRiverWidth);
			//-------------------------//
		}
			//find currentY according to endPoint
			//-------------------------//
			if ((currentX+2*basisLenght/cellSize_)>=width_)
			{
				minRiverY=height_;
				maxRiverY=0;
				for (int j=0;j<height_;j++)
				{
					if (layer_[topoStatus]->getVal(width_-1,j) == 6.0 )
					{
						if (j<minRiverY) {minRiverY=j;}
						if (j>maxRiverY) {maxRiverY=j;}
					}
				}
				dY=(maxRiverY-minRiverY);
				currentY=(minRiverY+dY/4+rand()%int(dY/2));


				QPointF p1(width_-2,currentY);
				QPointF p(width_-1,currentY);
				currentZ=layer_[topoElevation]->getVal(width_-1,currentY);
				currentZ-=rrElev;
				seg.addPoint(p1,
							currentZ,
							actualRiverWidth);
				seg.addPoint(p,
							currentZ,
							actualRiverWidth);
				i=width_;
			}
			//-------------------------//
	}


	mainRiver.addSegment(seg);
	editor visualisation(layer_);
	visualisation.paintOrga(mainRiver,topoStatus,Interpolation::NatCubic,10);
	mainRiverSpline=visualisation.paintOrga(mainRiver,topoElevation,Interpolation::NatCubic,-1,-rrElev);
};




/** @brief RiverElevationBaseTrend()
  *
  * for area where basic river can meander
  * sets an elevation base trend for hydroagents
  * parameter: elevSlope
  *
  */
void topoCreator::RiverAreaElevBaseTrend()
{

		double linearElevation=0;
		for (int i=0;i<width_;i++)
		{
			for(int j=0;j<height_;j++)
			{
				linearElevation=elev-elevSlope*i*cellSize_;
				layer_[topoElevation]->setVal(i,j,linearElevation);
			}
			//river2<<linearElevation<<"\t";
		}

};


/** @brief riverSinuosity()
  *
  * creates  a sinuosity for main river
  *
  */
void topoCreator::riverSinuosity()
{

		//sinuosities with order 1 of mainRiver
		segment sinuosity(1,&mainRiver);

		//additional segment organizer to write in topoStatus
		segmentOrganizer sinuoStatusOrg;
		segment sinuoStatus(0,&sinuoStatusOrg);


		double **sinuosityPoints;
		sinuosityPoints=new double*[30];
		for (int i=0;i<30;i++)
		{
			sinuosityPoints[i]=new double[3];
		}


		// parameter for sinuosity
		//-------------------------//
		//int noSinuosity=0;
		int randSinuosity=0+(rand()%(100));
			if (randSinuosity<75 && randSinuosity>=50)
				{randSinuosity=1;}
			if (randSinuosity>=75)
				{randSinuosity=2;}
			if (randSinuosity!=1 && randSinuosity!=2)
				{randSinuosity=3;}

		double sinuosityLength=randSinuosity*width_*0.1;
		//-------------------------//

		// init local variables
		//-------------------------//
		int endX=width_-1;
		int startY=0;
		int endY=0;
		double startElev=0;
		double endElev=0;
		int pointCount=0;
		int randSection=floor(sinuosityLength*0.1);
		int sign=(rand()%(2));
		if (sign==0) {sign=-1;}
		//-------------------------//

		layer_[topoStatus]->setSwitch(2);
		layer_[topoElevation]->setSwitch(2);


		// create points of minor river sections
		//-------------------------//
		//for (int no=0;no<noSinuosity;no++)
		{

			int randX=floor((10+(rand()%(50)))*0.01*width_);

			if (endX>(randX+sinuosityLength))
			{
				endX=randX+sinuosityLength;
			}

			if (sign==1)
			{
				for (int yy=0;yy<height_;yy++)
				{

					if((layer_[topoStatus]->getVal(randX,yy))==10)
					{
						startY=yy;
						startElev=layer_[topoElevation]->getVal(randX,yy);
						//break;
					}
					if((layer_[topoStatus]->getVal(endX,yy))==10)
					{
						endY=yy;
						endElev=layer_[topoElevation]->getVal(endX,yy);
						//break;
					}
				}
			}
			if (sign==-1)
			{
				for (int yy=height_-1;yy>0;yy--)
				{

					if((layer_[topoStatus]->getVal(randX,yy))==10)
					{
						startY=yy;
						startElev=layer_[topoElevation]->getVal(randX,yy);
						//break;
					}
					if((layer_[topoStatus]->getVal(endX,yy))==10)
					{
						endY=yy;
						endElev=layer_[topoElevation]->getVal(endX,yy);
						//break;
					}
				}
			}


			double y=0;
			for (int x=0;x<sinuosityLength;x+=randSection+(rand()%(randSection)))
			{
				y=3.1415*0.5*(1+4*x/sinuosityLength);
				//y=sign*sinuosityLength*0.1*(1-sin(y));
				y=sign*height_*0.02*(1-sin(y));
				sinuosityPoints[pointCount][0]=randX+x;
				sinuosityPoints[pointCount][1]=startY+y-(startY-endY)/sinuosityLength*x;
				sinuosityPoints[pointCount][2]=startElev-(((startElev-endElev)/sinuosityLength)*x);
				QPointF p(randX+x,
						  startY+y-(startY-endY)/sinuosityLength*x);

				sinuosity.addPoint(p,
								   startElev-(((startElev-endElev)/sinuosityLength)*x),
								   riverWidth*0.8);
				sinuoStatus.addPoint(p,
								   startElev-(((startElev-endElev)/sinuosityLength)*x),
								   riverWidth*0.8);

				pointCount+=1;
			}

				//last point
				sinuosityPoints[pointCount][0]=endX;
				sinuosityPoints[pointCount][1]=endY;
				sinuosityPoints[pointCount][2]=endElev;

				QPointF endP(endX,endY);

				sinuosity.addPoint(endP,
								   endElev,
								   riverWidth*0.6);
				sinuoStatus.addPoint(endP,
								   endElev,
								   riverWidth*0.6);


		}

		editor visualisation(layer_);

		sinuoStatusOrg.addSegment(sinuoStatus);
		visualisation.paintOrga(sinuoStatusOrg, topoStatus,Interpolation::NatCubic,10);
		sinuositiesSpline.append(visualisation.paintOrga(sinuoStatusOrg, topoElevation,Interpolation::NatCubic));

		//mainRiver.addSegment(sinuosity, topoElevation, Interpolation::NatCubic)

		//visualisation.paintOrga(mainRiver, topoElevation,Interpolation::NatCubic);



		//segmentOrganizer buffer;
		//buffer.addSegment(sinuosity);
		//sinuosities.addSegment(sinuosity);
		//editor visualisation(layer_);


		//sinuoStatusOrg.addSegment(sinuoStatus);
		//visualisation.paintOrga(sinuoStatusOrg,topoStatus,Interpolation::NatCubic,6);
		//visualisation.paintOrga(mainRiver,topoStatus,Interpolation::NatCubic,10);


		//visualisation.paintOrga(buffer,topoStatus,Interpolation::NatCubic,10);
	 //   visualisation.paintOrga(buffer,topoElevation,Interpolation::NatCubic,-1);



		//drawPoints(layer_,sinuosityPoints,pointCount,0);


		  for (int i=0; i<pointCount;i++)
		  {
			delete [] sinuosityPoints[i];
		  }
		  delete []sinuosityPoints;
};



/** @brief createminorRivers2()
  *
  * creates  minor rivers in topoStatus and topoElevation
  *
  */
void topoCreator::createminorRivers2()
{
	QList<QPointF> StartEndPoint=
		findStartEndPointMinorRiver();

	segmentOrganizer minorRiver;

	//insertr additional point way point
	QPointF WayP=(StartEndPoint.value(1)-StartEndPoint.value(0))*0.5;
	WayP=WayP+StartEndPoint.value(0);
	int dX=int(-0.025*width_)+((rand()%int(0.05*width_)));
	WayP.setX(WayP.x()+dX);

	double startZ=layer_[topoElevation]->getVal(int (StartEndPoint.value(0).x()),
												int (StartEndPoint.value(0).y()));
	double midZ=layer_[topoElevation]->getVal(int (WayP.x()),
											  int (WayP.y()));

	double endZ=layer_[topoElevation]->getVal(int (StartEndPoint.value(1).x()),
											  int (StartEndPoint.value(1).y()));
	double endendZ=layer_[topoElevation]->getVal(int (StartEndPoint.value(2).x()),
											  int (StartEndPoint.value(2).y()));


	//write minor River in topoElevation and topoStatus
	//-------------------------//
	segment _minor(0,&minorRiver);
	_minor.addPoint(StartEndPoint.value(0),startZ,riverWidth*0.2);
	_minor.addPoint(WayP,midZ,riverWidth*0.2);
	_minor.addPoint(StartEndPoint.value(1),endZ,riverWidth*0.2);
	_minor.addPoint(StartEndPoint.value(2),endendZ,riverWidth*0.2);
	editor vis(layer_);
	minorRiver.addSegment(_minor);
	vis.paintOrga(minorRiver,topoStatus,Interpolation::NatCubic,9);
	QVector<sPoint> points=vis.paintOrga(minorRiver,topoElevation,Interpolation::NatCubic,-1,-0.01);
	//-------------------------//

	//erosion model
	//-------------------------//
	double angleOfResponse=28+rand()%10;;
	int numberPoints=points.size();

		editor erosionMaster(layer_);
		segmentOrganizer erosionPlus;
		segmentOrganizer erosionMinus;
		segment erosP(0,&erosionPlus);
		segment erosM(0,&erosionMinus);


	//topoStatus == 6 start point for erosion
	StartEndPoint.value(1);
	endZ;

	//start point for minor river (y==0 || y==height)
	StartEndPoint.value(0);
	startZ;

	//max height for erosion with 60% securtity
	double maxdZ=(startZ-endZ)*0.4;
	double currentdZ=0;

	//evaluate position of start point for erosion
	//in points
	int startI=0;
	for (int i=0; i<numberPoints; i++)
	{
		if (points.value(i).getPointF()==StartEndPoint.value(1))
		{startI=i;}
	}

	//evaluate erosion
	for (int i=startI; i>=0; i--)
	{
		//getNormalVector
		double normalX=-points.value(i).getPointF().y()+
					   points.value(i+1).getPointF().y();
		double normalY=points.value(i).getPointF().x()-
					   points.value(i+1).getPointF().x();

		//get unit normal vector
		double length=normalX*normalX+normalY*normalY;
		length=sqrt(length);
		normalX/=length;
		normalY/=length;


		//set current point attributes
		int currentX=round(points.value(i).getPointF(),1).x();
		int currentY=round(points.value(i).getPointF(),1).y();
		double currentZ=layer_[topoElevation]->getVal(currentX,currentY);

		//for river points do maximal erosion
		int dirty=2;
			//linear maxdZ function
			//currentdZ=maxdZ*2*(startI-i)/startI;
			/*
			exponential maxdz function
			with maximum at mid point an cap maxdZ
			*/
			currentdZ=maxdZ*dirty*exp(log(0.01)*((1-double ((double(startI-i))/double(startI)))));
			if (currentdZ>maxdZ) currentdZ=maxdZ;
		currentZ-=currentdZ;

		double newZ=currentZ;
		double surfaceZ=newZ+1;
		double newX=currentX;
		double newY=currentY;

		int count=0;
		double dz=0;
		double distance=0;
		double bufferDistance=0;

		//dirty!!!
		if (i<startI-5)
		{
			//do erosion until natural surface is reached plus side
			QPointF p(newX,newY);
			erosP.addPoint(p,newZ,4*cellSize_);
			erosM.addPoint(p,newZ,4*cellSize_);

			while (newZ<surfaceZ)
			{
				int breaker=0;
				count+=1;
				bufferDistance=distance;
				newX=int(currentX+count*normalX);
				newY=int(currentY+count*normalY);
				if (newX>=width_ || newX<0 || newY >= height_ || newY<0)
				{
					if (newX>=width_)newX=width_-1;
					if (newX<0) newX=0;
					if (newY >= height_) newY=height_-1;
					if (newY<0) newY=0;
					breaker=1;
				}
				surfaceZ=layer_[topoElevation]->getVal(int (newX),int (newY));
				distance=(newX-currentX)*(newX-currentX)+
						 (newY-currentY)*(newY-currentY);
				distance=sqrt(distance);

				dz=distance*tan(angleOfResponse*0.0174523);
				newZ=currentZ+dz;
				if (breaker == 1) break;
			}
			dz=bufferDistance*tan(angleOfResponse*0.0174523);
			newX=int(currentX+(count-1)*normalX);
			newY=int(currentY+(count-1)*normalY);
			newZ=currentZ+dz;
			QPointF q(newX,newY);
			erosP.addPoint(q,newZ,cellSize_*4);

			/*close gap between last point and dataLayer boarder
			add additional points in zigzag form
			*/
			if(i==0)
			{
				int closeGap=abs(int(newY-p.y()));
				if (p.y()<1 && closeGap>0)
				{
					for (int k=0;k<=closeGap;k++)
					{
						erosP.addPoint(p,currentZ,cellSize_*4);
						QPointF gap(newX,p.y()+k);
						double gapZ=layer_[topoElevation]->getVal(int (gap.x()),int(gap.y()));
						erosP.addPoint(gap,gapZ,cellSize_*4);
					}
				}
				if (p.y()>height_-2 && closeGap>0)
				{
					for (int k=0;k<=closeGap;k++)
					{
						erosP.addPoint(p,currentZ,cellSize_*4);
						QPointF gap(newX,p.y()-k);
						double gapZ=layer_[topoElevation]->getVal(int (gap.x()),int(gap.y()));
						erosP.addPoint(gap,gapZ,cellSize_*4);

					}
				}
			}

			newZ=currentZ;
			surfaceZ=newZ+1;
			newX=currentX;
			newY=currentY;
			count=0;
			dz=0;
			distance=0;
			bufferDistance=0;

			//do erosion until natural surface is reached minus side
			while (newZ<surfaceZ)
			{
				int breaker=0;
				count+=1;
				bufferDistance=distance;
				newX=int(currentX-count*normalX);
				newY=int(currentY-count*normalY);
				if (newX>=width_ || newX<0 || newY >= height_ || newY<0)
				{
					if (newX>=width_)newX=width_-1;
					if (newX<0) newX=0;
					if (newY >= height_) newY=height_-1;
					if (newY<0) newY=0;
					breaker=1;

				}
				surfaceZ=layer_[topoElevation]->getVal(int (newX),int (newY));
				distance=(newX-currentX)*(newX-currentX)+
								(newY-currentY)*(newY-currentY);
				distance=sqrt(distance);

				dz=distance*tan(angleOfResponse*0.0174523);
				newZ=currentZ+dz;
				if (breaker == 1) break;
			}

			dz=bufferDistance*tan(angleOfResponse*0.0174523);
			newX=int(currentX-(count-1)*normalX);
			newY=int(currentY-(count-1)*normalY);
			newZ=currentZ+dz;
			QPointF m(newX,newY);
			erosM.addPoint(m,newZ,cellSize_*4);

			/*close gap between last point and dataLayer boarder
			add additional points in zigzag form
			*/
			if(i==0)
			{
				int closeGap=abs(int(newY-p.y()));
				if (p.y()<1 && closeGap>0)
				{
					for (int k=0;k<=closeGap;k++)
					{
						erosM.addPoint(p,currentZ,cellSize_*4);
						QPointF gap(newX,p.y()+k);
						double gapZ=layer_[topoElevation]->getVal(int (gap.x()),int(gap.y()));
						erosM.addPoint(gap,gapZ,cellSize_*4);
					}
				}
				if (p.y()>height_-2 && closeGap>0)
				{
					for (int k=0;k<=closeGap;k++)
					{
						erosM.addPoint(p,currentZ,cellSize_*4);
						QPointF gap(newX,p.y()-k);
						double gapZ=layer_[topoElevation]->getVal(int (gap.x()),int(gap.y()));
						erosM.addPoint(gap,gapZ,cellSize_*4);
					}
				}
			}
		}
	}
	erosionPlus.addSegment(erosP);
	erosionMinus.addSegment(erosM);
	erosionMaster.paintOrga(erosionPlus,topoElevation,Interpolation::Linear);
	erosionMaster.paintOrga(erosionMinus,topoElevation,Interpolation::Linear);
	minorRiverSpline.append(vis.paintOrga(minorRiver,topoElevation,Interpolation::NatCubic,-1,-rrElev*0.75));
	//erosionMaster.paintOrga(erosionPlus,Industry,Interpolation::Linear);
	//erosionMaster.paintOrga(erosionMinus,Industry,Interpolation::Linear);
	//vis.paintOrga(minorRiver,Industry,Interpolation::NatCubic,200);

};

/** @brief findStartEndPointMinorRiver()
  *
  * finds start, mid and end point of minor river
  *
  */
QList<QPointF> topoCreator::findStartEndPointMinorRiver()
{
	QList<QPointF> StartEndPoint;
	StartEndPoint.clear();

	int startX=0;
	int startY=0;
	int dX=0;
	int midX=0;
	int midY=0;

	int endX=startX+dX;
	int endY=0;
	int count=0;

	int sign=1;


	//number minor Rivers maximal 4
	//4 sectors of minor Rivers
	int quad=0;
	int whileCount=0;
while (quad==0)
{
	int i=rand()%4;

	if(i==0 && quadrant[0]==0)
	{
		startX=int(0.125*width_)+rand()%int(0.25*width_);
		startY=0;
		quadrant[i]=1;
		quad+=1;
	}
	if(i==1 &&quadrant[1]==0)
	{
		startX=int(0.625*width_)+rand()%int(0.25*width_);
		startY=0;
		quadrant[i]=1;
		quad+=1;

	}
	if(i==2 &&quadrant[2]==0)
	{
		startX=int(0.125*width_)+rand()%int(0.25*width_);
		startY=height_-1;
		quadrant[i]=1;
		quad+=1;
		sign=-1;
	}
	if(i==3 &&quadrant[3]==0)
	{
		startX=int(0.625*width_)+rand()%int(0.25*width_);
		startY=height_-1;
		quadrant[i]=1;
		quad+=1;
		sign=-1;
	}
	if (whileCount>100)
	{
		std::cout<<"error, topoCreator::findStartEndPointMinorRiver()\n";
		quad+=1;
	}
	whileCount+=1;
}

	dX=int(-0.025*width_)+((rand()%int(0.05*width_)));
	midX=startX-((rand()%int(0.05*width_)));


	endX=startX+dX;





	for (int j=startY; j*sign<(height_-startY); j+=sign)
	{
		if (layer_[topoStatus]->getVal(endX,j)==6)
		{

			if (count==0) midY=j-sign;

			count+=1;

		}


		if (layer_[topoStatus]->getVal(endX,j)==10)
		{
			endY=j-sign;
			j=(height_-startY)*sign;
		}
	}
	QPointF StartP(startX,startY);
	QPointF MidP(midX,midY);
	QPointF EndP(endX,endY);
	StartEndPoint.append(StartP);
	StartEndPoint.append(MidP);
	StartEndPoint.append(EndP);





	return StartEndPoint;


};

/** @brief createminorRivers()
  *
  * creates a minor river
  *
  */
void topoCreator::createminorRivers()
{
	segment minorRiver(2,&minorRivers);

	//-------------------------//
	int statusVal=9;
	int startPointX=0;
	int startPointY=0;
	double startPointZ=0.0;
	int endPointX=0;
	int endPointY=0;
	int dX=0;
	int dY=0;
	int randdX=0;
	int randdY=0;
	double buffer=400/cellSize_;
	int riverOverlay=0;


	int pointCount=0;
	double **Points;
		Points=new double*[20];
		for (int i=0;i<20;i++)
		{
			Points[i]=new double[3];
			for (int j=0;j<3;j++)
			{
				Points[i][j]=0;
			}
		}
	//-------------------------//



	//startPoint and endPoint
	//-------------------------//
	startPointX=0+(rand()%(width_));

		endPointX=floor(width_*0.3);
		endPointX=rand()%(endPointX);
		endPointX-=(0.1*width_);
		endPointX=startPointX-floor(width_*0.02-endPointX);
		if (endPointX<0) {endPointX=0;}

		endPointY=rand()%(2);

		if (endPointY==0)
		{
			for (int y=0; y<height_-1; y++)
			{
				if (layer_[topoStatus]->getVal(startPointX,y)==10)
				{
					startPointY=y;
					break;
				}
			}

		}
		else
		{
			endPointY=height_-1;

			for (int yy=endPointY; yy>0; yy--)
			{
				if (layer_[topoStatus]->getVal(startPointX,yy)==10)
				{
					startPointY=yy;
					break;
				}
			}

		}


		//-------------------------//

		for (int r=1; r<3*buffer;r++)
		{
			if ((endPointX+r)<width_ && (endPointX-r)>=0/* &&
				(startPointX+r)<height_ && (startPointX-r)>0 &&*/)
			{
				if (layer_[topoStatus]->getVal(endPointX+r,endPointY)>0 ||
					layer_[topoStatus]->getVal(endPointX-r,endPointY)>0 /*||
					layer_[topoStatus]->getVal(startPointX+r,startPointY)>0 ||
					layer_[topoStatus]->getVal(startPointX-r,startPointY)>0  */ )
				{
					riverOverlay+=1;
				}
			}
			else
			{
					riverOverlay+=1;
			}

		}
		if (riverOverlay<1)
		{





			//write startPoint
			//-------------------------//
				Points[0][0]=startPointX;
				Points[0][1]=startPointY;
				layer_[topoElevation]->setSwitch(2);
				startPointZ=layer_[topoElevation]->getVal(startPointX,startPointY);
				Points[0][2]=startPointZ;
				//Points[0][2]-=rrElev;
				pointCount+=1;

				QPointF p(0,0);
				p.setX(startPointX);
				p.setY(startPointY);
				minorRiver.addPoint(p,startPointZ,cellSize_);

			//-------------------------//


			//waypoints
			//-------------------------//
			int numPoints=10;
			dX=endPointX-startPointX;
			dX/=(numPoints);
			dY=endPointY-startPointY;
			dY/=(numPoints);

			for (int i=0;i<=numPoints;i++)
			{

				randdX=(dX);

				randdY=(dY);


				randdX=floor(0.35*randdX);
				if (randdX<2 && randdX>=0) {randdX=2;}
				if (randdX>-2 && randdX<=0) {randdX=-2;}
				if (randdX<0)
				{randdX-=(rand()%(randdX));}
				else {randdX+=(rand()%(randdX));}


				randdY=floor(0.35*randdY);
				if (randdY<10 && randdY>=0) {randdY=10;}
				if (randdY>10 && randdY<=0) {randdY=-10;}
				if (randdY<0) {randdY-=(rand()%(randdY));}
				else {randdY+=(rand()%(randdY));}


				Points[pointCount][0]=floor(Points[pointCount-1][0]+randdX);
				Points[pointCount][1]=floor(Points[pointCount-1][1]+randdY);
				if (Points[pointCount][1]>height_) {Points[pointCount][1]=height_-1;}
				Points[pointCount][2]=((layer_[topoElevation]->
					getVal(Points[pointCount][0],
						   Points[pointCount][1])))-(rrElev/*+rrElev*i/numPoints*/);



					//segmentOrganizer
					//-------------------------//
					p.setX(Points[pointCount][0]);
					p.setY(Points[pointCount][1]);

					minorRiver.addPoint(p,Points[pointCount][2],cellSize_);
					//-------------------------//




				pointCount+=1;
			}
			//-------------------------//


			//write endPoint
			//-------------------------//
			Points[pointCount][0]=endPointX;
			Points[pointCount][1]=endPointY;
			Points[pointCount][2]=(layer_[topoElevation]->getVal(endPointX,endPointY))-rrElev;
			//pointCount+=1;

			//segmentOrganizer
			//-------------------------//
			p.setX(endPointX);
			p.setY(endPointY);

			minorRiver.addPoint(p,Points[pointCount][2],cellSize_);
			minorRivers.addSegment(minorRiver);
			//-------------------------//

			riverWidth*=0.2;
			//-------------------------//



			//check out of bounds
			//-------------------------//
			int xmax=0;
			int xmin=width_-1;
			int ymax=0;
			int ymin=height_-1;

			for (int c=0;c<pointCount;c++)
			{
				if (Points[c][0]<xmin) {xmin=Points[c][0];}
				if (Points[c][0]<xmax) {xmax=Points[c][0];}
				if (Points[c][1]<ymin) {ymin=Points[c][1];}
				if (Points[c][1]<ymax) {ymax=Points[c][1];}
			}


			if (xmax>width_ || xmin<0 ||
				ymax>height_ || ymin<0)
			{
				std::cout<<"topoCreator::createminorRivers, out of bounds\n";
			}
			//-------------------------//

			else
			{

				//drawPoints(layer_,Points,pointCount,0);


				//write in topoStatus and topoElevation
				//-------------------------//
				for (int c=0;c<pointCount;c++)
				{
					int dx=Points[c+1][0]-Points[c][0];
					int dy=Points[c+1][1]-Points[c][1];

					if (dy==0) {dy=1;}
					if (dy>0)
					{
						for (int y=0;y<dy;y++)
						{
							double currentElevation=0;
							double currentX=Points[c][0];
							int currentY=0;

							currentY=Points[c][1];
							currentX+=(dx*y/dy);
							currentX=floor(currentX);
							currentY+=y;
							if (currentY>width_) {currentY=width_-1;}
							currentElevation=(layer_[topoElevation]->getVal(currentX,currentY));

							if (c==0 && y==0) {}
							else {currentElevation-=rrElev;}

							if (c>0 && layer_[topoStatus]->getVal(currentX,currentY)==10)
							{
								y=dy;
								c=pointCount;
							}


							layer_[topoElevation]->setVal(currentX,currentY,currentElevation);
							layer_[topoStatus]->setVal(currentX,currentY,statusVal);


							for (int i=1;i<(floor(buffer));i++)
							{
								double bufferValP=0;
								double bufferValM=0;
								if ((currentX-i)>=0 && (currentX+i)<width_)
								{
									//if no conflict with main river
									if (layer_[topoStatus]->getVal(currentX+i,currentY)!=10 &&
										layer_[topoStatus]->getVal(currentX-i,currentY)!=10 &&
										layer_[topoStatus]->getVal(currentX+i,currentY)!=6  &&
										layer_[topoStatus]->getVal(currentX-i,currentY)!=6)

									{
										bufferValP=layer_[topoStatus]->getVal(currentX+i,currentY)+5*(buffer-i)/buffer;
										if (bufferValP>5) {bufferValP=5;}
										bufferValM=layer_[topoStatus]->getVal(currentX-i,currentY)+5*(buffer-i)/buffer;
										if (bufferValM>5) {bufferValM=5;}

										layer_[topoStatus]->setVal(currentX+i,currentY,bufferValP);
										layer_[topoStatus]->setVal(currentX-i,currentY,bufferValM);

										bufferValP=layer_[topoElevation]->getVal(currentX+i,currentY)-0.5*rrElev*(buffer-i)/buffer;
										bufferValM=layer_[topoElevation]->getVal(currentX-i,currentY)-0.5*rrElev*(buffer-i)/buffer;
										layer_[topoElevation]->setVal(currentX+i,currentY,bufferValP);
										layer_[topoElevation]->setVal(currentX-i,currentY,bufferValM);
									}
								}
							}
						}
					}
					if (dy<0)
					{
						dy*=-1;
						for (int y=0;y<dy;y++)
						{
							double currentElevation=0;
							double currentX=Points[c][0];
							int currentY=Points[c][1];

							currentX+=(dx*y/dy);
							currentX=floor(currentX);
							currentY-=y;
							if (currentY>width_) {currentY=width_-1;}
							currentElevation=(layer_[topoElevation]->getVal(currentX,currentY));

							if (c==0 && y==0) {}
							else {currentElevation-=rrElev;}

							if (c>0 && layer_[topoStatus]->getVal(currentX,currentY)==10)
							{
								y=dy;
								c=pointCount;
							}

							layer_[topoStatus]->setVal(currentX,currentY,statusVal);
							layer_[topoElevation]->setVal(currentX,currentY,currentElevation);



							for (int i=1;i<(floor(buffer));i++)
							{
								double bufferValP=0;
								double bufferValM=0;

								if ((currentX-i)>=0 && (currentX+i)<width_)
								{

									//if no conflict with main river
									if (layer_[topoStatus]->getVal(currentX+i,currentY)!=10 &&
										layer_[topoStatus]->getVal(currentX-i,currentY)!=10 &&
										layer_[topoStatus]->getVal(currentX+i,currentY)!=6  &&
										layer_[topoStatus]->getVal(currentX-i,currentY)!=6)

									{
										bufferValP=layer_[topoStatus]->getVal(currentX+i,currentY)+5*(buffer-i)/buffer;
										if (bufferValP>5) {bufferValP=5;}
										bufferValM=layer_[topoStatus]->getVal(currentX-i,currentY)+5*(buffer-i)/buffer;
										if (bufferValM>5) {bufferValM=5;}
										layer_[topoStatus]->setVal(currentX+i,currentY,bufferValP);
										layer_[topoStatus]->setVal(currentX-i,currentY,bufferValM);

										bufferValP=layer_[topoElevation]->getVal(currentX+i,currentY)-0.5*rrElev*(buffer-i)/buffer;
										bufferValM=layer_[topoElevation]->getVal(currentX-i,currentY)-0.5*rrElev*(buffer-i)/buffer;
										layer_[topoElevation]->setVal(currentX+i,currentY,bufferValP);
										layer_[topoElevation]->setVal(currentX-i,currentY,bufferValM);
									}

								}
							}


						}
					}

				}
				//-------------------------//
			}

}



	delete Points;


};






/** @brief riverCreate_1()
  *
  * creates a main river
  *
  */
void topoCreator::riverCreate_1()
{
	/*
	on:	  if set outputAct==0 then pp=50
		  to activate river Variation output
		  for testing variation of riverCreator*/
	      int outputAct=0;
		  int pp=1;


//create River
//-------------------------//
	const int riverPoints=100;
	riverStruc *id[riverPoints];

//river variation attributes for each river section
	double drandSlope=0;

//faktors for polynom 2nd order
	double a=0;
	double b=0;
	double c=0;

	//test output in river.txt
	std::ofstream river;
	river.open("river.txt");
	if (outputAct==0)
	{
		pp=50;
	}
/*test riverCreator
if outputAct==1: 50 testriver output in river.txt
else: no output*/
for (int p=0;p<pp;p++)
{

//river geometry as polynon 2nd order
	//factors for polynom 2nd order
	a=((0+(rand()%(100)))*0.04)*0.01-0.02;
	b=((0+(rand()%(100)))*3)*0.01-1.5;
	c=((0+(rand()%(100)))*80)*0.01-40; //+165

	if (c<50) //that river reaches xmax && y>ymin
	{
		if(a<0) a=-a;
		if(b>0) b=-b;
	}
	if (c>50) //that river reaches xmax && y<ymax
	{
		if(a>0) a=-a;
		if(b<0) b=-b;
	}

	//factors to scale to datalayer
	double heightFactor=(height_)*0.01;
	double widthFactor=(width_)*0.01;



	//for each x value one y value
	for (int h=0; h<riverPoints; h++)
	{
		id[h]=new riverStruc;

		//variation of Slope for each section +-5% of riverslope
		drandSlope=riverSlope*(1+0.05*(0+(rand()%(200))-100)*0.01);
		id[h]->setPoint(
			h*widthFactor,
			floor(0.25*(a*h*h+b*h+c)*heightFactor),
			(elev-rrElev)-h*cellSize_*drandSlope,
			0,0);

		if (outputAct==0)
		{
			//test output in river.txt
			river<<h*widthFactor<<"\t\t"
				<<floor(0.25*(a*h*h+b*h+c)*heightFactor)
				<<"\t\t"<<(elev-rrElev)-h*cellSize_*drandSlope<<"\n";
		}
	}






}
river.close();

//adjust river
//-------------------------//
	riverWidth=floor(riverWidth/cellSize_);
	int riverHeight=floor(riverWidth/cellSize_);
	int iPuffer=0;
	//int jPuffer=0;

	for (int h=0; h<riverPoints;h++)
	{
		for (int i=0;i<width_;i++)
		{
			for(int j=0;j<height_;j++)
			{
				//adjust river to cellSize
				if (id[h]->getX()==i && id[h]->getY()==j)
				{
					iPuffer=0;
					if(h !=0)
					{
						iPuffer=id[h-1]->getX();
						//jPuffer=id[h-1]->getY()-id[h]->getY();
					}
					if(iPuffer !=0)
					{
						while((i-iPuffer)>1)
						{
							for (int l=0;l<=riverWidth;l++)
							{
								for(int m=0;m<riverHeight;m++)
								layer_[0]->setVal(iPuffer+l,j+m,3);
							}
							iPuffer +=1;

						}
					}

					for (int l=0;l<riverWidth;l++)
					{
						for(int m=0;m<=riverHeight;m++)
						layer_[0]->setVal(i+l,j+m,3);
					}
				}
			}
		}
	}

};


/** @brief drawPoints()
  *
  * function to draw pointArray with linear interpolation and smoothing
  *
  */
void topoCreator::drawPoints(dataLayer **layer_, double **pointArray, int pointCount, int smoothing_)
{
	/*
	draws pointArray in
	topoStatus, topoElevation
	*/

	smoothing=smoothing_;
	double dx=0;
	double dy=0;
	double de=0;
	double ddx=0;
	double ddy=0;
	double currentElevation=0;
	int vs=0;


		for (int s=0;s<pointCount;s++)
		{
			dx=pointArray[s+1][0]-pointArray[s][0];
			dy=pointArray[s+1][1]-pointArray[s][1];
			de=pointArray[s+1][2]-pointArray[s][2];
			ddx=pointArray[s+2][0]-pointArray[s+1][0];
			ddy=pointArray[s+2][1]-pointArray[s+1][1];
			currentElevation=0;
			vs=s;

			if (fabs(dx)>=fabs(dy))
			{

				if (dx<0) {vs+=1;}

				//smoothing if the change of angle
				//is more than alphaTol
				//-------------------------//
				if (smoothing==1)
				{
					if (s<pointCount-1)
					{

						alpha=acos(((dx*ddx)+(dy*ddy))/sqrt((dx*dx+dy*dy)*(ddx*ddx+ddy*ddy)));
						alpha*=180/PI;
						alpha=180-alpha;
						//std::cout<<"dx>dy "<<alpha<<"\n";

						if (alpha<alphaTol)
						{
							dx*=bendFac;
							dy*=bendFac;
							de*=bendFac;
							//std::cout<<"new\n";
							pointArray[s+1][0]=dx+pointArray[s][0];//->
							pointArray[s+1][1]=dy+pointArray[s][1];//Î
							pointArray[s+1][2]=de+pointArray[s][2];

						}
					}
				}
				//-------------------------//




				for (int x=0;x<=fabs(dx);x++)
				{

					double y = 0;
					if (dy==0)
					{
						y=pointArray[vs][1];
					}
					else
					{
						y= ((dy/dx)*(x))+pointArray[vs][1];
					}
					if (dx==0){currentElevation=pointArray[vs][2];}
					if (dx==0){dx=1;}
					else {currentElevation=pointArray[vs][2]+de*x/fabs(dx);}

					for (int actualRiverWidth=0;actualRiverWidth<=(riverWidth/cellSize_);actualRiverWidth++)
					{
						if ((x+pointArray[vs][0]+actualRiverWidth)<width_)
						{
							layer_[topoStatus]->setVal(x+pointArray[vs][0]
											+actualRiverWidth,floor(y),10);
							layer_[topoStatus]->setVal(x+pointArray[vs][0]
											+actualRiverWidth, ceil(y),10);

							layer_[topoElevation]->setVal(x+pointArray[vs][0]+actualRiverWidth,
											floor(y),currentElevation);
							layer_[topoElevation]->setVal(x+pointArray[vs][0]+actualRiverWidth,
											ceil(y),currentElevation);


						}
					}
				}
			}
			if ((fabs(dx)<fabs(dy)))
			{
				if (dy<0) {vs+=1;}
				//smoothing if the change of angle
				//is more than alphaTol
				//-------------------------//
				if (smoothing==1)
				{
					if (s<pointCount-1)
					{

						alpha=acos(((dx*ddx)+(dy*ddy))/sqrt((dx*dx+dy*dy)*(ddx*ddx+ddy*ddy)));
						alpha*=180/PI;
						alpha=180-alpha;
						//std::cout<<"dx<dy "<<alpha<<"\n";

						if (alpha<alphaTol)
						{
							dx*=bendFac;
							dy*=bendFac;
							de*=bendFac;
							//std::cout<<"new\n";
							pointArray[s+1][0]=dx+pointArray[s][0];//->
							pointArray[s+1][1]=dy+pointArray[s][1];//Î
							pointArray[s+1][2]=de+pointArray[s][2];

						}
					}
				}
				//-------------------------//


				for (int y=0;y<=fabs(dy);y++)
				{
					double x = 0;
					if (dx==0)
					{
						x=pointArray[vs][0];
					}
					else
					{
						x= ((dx/dy)*(y))+pointArray[vs][0];
					}

					if (dy==0) {currentElevation=pointArray[vs][2];}
					if (dy!=0) {currentElevation=pointArray[vs][2]+de*y/fabs(dy);}


					for (int actualRiverWidth=0;actualRiverWidth<=(riverWidth/cellSize_);actualRiverWidth++)
					{

							layer_[topoStatus]->setVal(floor(x),
									y+pointArray[vs][1]+actualRiverWidth,10);
							layer_[topoStatus]->setVal(ceil(x),
									y+pointArray[vs][1]+actualRiverWidth,10);

							layer_[topoElevation]->setVal(floor(x),
									y+pointArray[vs][1]+actualRiverWidth,currentElevation);
							layer_[topoElevation]->setVal(ceil(x),
									y+pointArray[vs][1]+actualRiverWidth,currentElevation);


					}
				}
			}
		}
};


/** @brief riverCreate_2()
  *
  * creates a main river
  *
  */
void topoCreator::riverCreate_2()
{
	/*
	if set outputAct==0 then pp=50
	to activate river Variation output
	for testing variation of riverCreator
	*/
	int outputAct=0;
	int pp=1;


//create mainRiver
//-------------------------//
	const int riverPoints=100;
	riverStruc *id[riverPoints];

	//river variation attributes for each river section
	double drandSlope=0;

	//faktors for polynom 2nd order
	double a=0;
	double b=0;
	double c=0;

	//test output in river.txt
	//std::ofstream river2;
	//river2.open("river2.txt");
	if (outputAct==0)
	{
		pp=50;
	}
/*test riverCreator
if outputAct==1: 50 testriver output in river.txt
else: no output*/
for (int p=0;p<pp;p++)
{

//river geometry as polynon 2nd order
	//faktors for polynom 2nd order
	a=((0+(rand()%(100)))*0.04)*0.01-0.02;
	b=((0+(rand()%(100)))*3)*0.01-1.5;
	c=((0+(rand()%(100)))*80)*0.01+165;

	if (c<50) //that river reaches xmax && y>ymin
	{
		if(a<0) a=-a;
		if(b>0) b=-b;
	}
	if (c>50) //that river reaches xmax && y<ymax
	{
		if(a>0) a=-a;
		if(b<0) b=-b;
	}

	//faktors to scale to datalayer
	double heightFactor=(height_)*0.01;
	double widthFactor=(width_)*0.01;

	//unit normal vector for each river section
	double nVecX=0;
	double nVecY=0;
	double lVec=0;


	//for each x value one y value
	for (int h=0; h<riverPoints; h++)
	{
		id[h]=new riverStruc;//main river centerline

		//variation of Slope for each section +-5% of riverslope
		drandSlope=riverSlope*(1+0.05*(0+(rand()%(200))-100)*0.01);

		if (h>0)
		{
			nVecY=((id[h]->getY())-(id[h-1]->getY()));
			nVecX=-((id[h]->getX())-(id[h-1]->getX()));
			lVec=sqrt(nVecX*nVecX+nVecY*nVecY);
			nVecX/=lVec;
			nVecY/=lVec;
		}

		id[h]->setPoint(
			   h*widthFactor,
			   floor(0.25*(a*h*h+b*h+c)*heightFactor),
			  (elev-rrElev)-h*cellSize_*drandSlope,
			   nVecX,
			   nVecY);

		if (h==1)
		{
			h=0;
			id[h]->setPoint(
				   h*widthFactor,
				   floor(0.25*(a*h*h+b*h+c)*heightFactor),
				  (elev-rrElev)-h*cellSize_*drandSlope,
				   nVecX,
				   nVecY);
			id[h]->setfP(1);
			h=1;

		}
		if (h==riverPoints-1)
		{
			id[h]->setfP(1);
		}



	}



}




	/*
	define area in dataLayer[topoStatus]
	in which the river possibly can be (meanderWidth)
	val=1
	*/
	//-------------------------//
		for (int w=0;w<width_;w++)
		{

			//int meandfac=3;
			//if (meanderWidth==1) {meandfac=5;}
			//if (meanderWidth==2) {meandfac=3;}
			//if (meanderWidth==3) {meandfac=1;}




			int dy=layer_[topoStatus]->getmHeight();
			for (int hh=0;hh<height_;hh++)
			{
				for (int h=0; h<riverPoints; h++)
				{
					if (w>=id[h]->getX() &&
						w<id[h+1]->getX())
					{
						if ((hh<(id[h]->getY()-dy)) ||
							(hh>(id[h]->getY()+dy)))
						{
							layer_[topoStatus]->setVal(w,hh,0);

						}
						else
						{
							layer_[topoStatus]->setVal(w,hh,6);
						}




					}
				}
			}
		}

	//-------------------------//


//adjust river
//-------------------------//

	//create randomize river sections
	//with randomized lenght between 8 - 32 % of total length
	int fPCount=0;
	int riverfp[50];
	for (int i=0;i<50;i++)
	{
		riverfp[i]=0;
	}
	for (int h=0; h<riverPoints; h+=floor(riverPoints*0.01*(8+(rand()%(25)))))
	{
		riverfp[fPCount]=h;
		if (h>0)
		{
			for (int i=0;i<width_;i++)
			{
				for(int j=0;j<height_;j++)
				{

					if (id[h]->getX()==i && id[h]->getY()==j)
					{
						//calc new points with meanderWidth and meander factor;
						id[h]->setfP(1);
						/*
						rSecProb river section probability
						meanderfactor between 0.5 - 2 = number of
						meander in riversection
						*/
						int rSecProb=(0+(rand()%(meanderFactor*2+1)))-1;
						for (int k=0;k<rSecProb;k++)
						{
							/*
							if there is a meander in a river section rSec
							ti is set between 15 and 85 %
							of the lenght of this river section
							*/
							int rSec=riverfp[fPCount]-riverfp[fPCount-1];
							rSec=floor(rSec*(15+(rand()%(70)))*0.01);
							double nVec=0;
							/*
							meanderWidth sets lenght of normalvector size
							2 Percent of datalayer width * 8|5|1 * 0-100%
							*/
							int mW=(height_*0.02*(50+(rand()%(50)))*0.01);

							if (meanderWidth==1) {nVec=8*mW;}
							if (meanderWidth==2) {nVec=5*mW;}
							if (meanderWidth==3) {nVec=1*mW;}



							/*
							randomly set on which river side to meander
							signum between 0,1 for - +
							*/
							int sign=(0+(rand()%(2)));
							if (sign==0) {nVec=-nVec;}
							id[h-rSec]->setPoint(id[h-rSec]->getX()+floor(nVec*id[h-rSec]->getnVec()[0]),
												 id[h-rSec]->getY()+floor(nVec*id[h-rSec]->getnVec()[1]),
												 id[h-rSec]->getElev(),
												 0,
												 0);
							id[h-rSec]->setfP(1);
						}
					}
				}
			}
		}
		fPCount+=1;
	}


	//write river supporting points
	//-------------------------//
	double **divdifVec;
	divdifVec = new double*[50];
	for (int i=0;i<50;i++)
	{
		riverfp[i]=0;
		divdifVec[i]= new double [3];
		for (int l=0;l<3;l++)
		{
			divdifVec[i][l]=0;
		}

	}
	int v=0;
	for (int q=0; q<riverPoints;q++)
	{
		if (id[q]->getfP()==1)
		{
			divdifVec[v][0]=id[q]->getX();
			divdifVec[v][1]=id[q]->getY();
			divdifVec[v][2]=id[q]->getElev();

	/*			if (outputAct==0)
				{
					river2<<divdifVec[v][0]<<"\t"
						  <<divdifVec[v][1]<<"\t"
						  <<divdifVec[v][2]<<"\n";
				}*/
				//number of supporting points v
				v+=1;
		}

	}



	//linear Interpolation
	//-------------------------//
		/*
		randFac for Section randomization of riverWidth
		randFac is increasing with flowpath
		riverWidth (riverWidth) * randFac (between 80 and 200 %) = ri
		ri actual river width set in for loop
		layer_[topoStatus] 0,1
						   0 if no river
						   1 if river
		layer_[topoElevation] for elevation
		*/

		//elevation base trend
		//-------------------------//
		double linearElevation=0;
		for (int i=0;i<width_;i++)
		{
			for(int j=0;j<height_;j++)
			{
				linearElevation=elev-elevSlope*i*cellSize_;
				layer_[topoElevation]->setVal(i,j,linearElevation);
			}
		}
		//-------------------------//

	drawPoints(layer_,divdifVec,v-1,1);

	layer_[topoStatus]->setSwitch(2);
	layer_[topoElevation]->setSwitch(2);


};


/** @brief adjustRiver()
  *
  * former function to create meandering river
  * displaced by createMainRiver()
  *
  */
void topoCreator::adjustRiver()
{
	//adjust river
	//-------------------------//
	segment basicRiver(0,&mainRiver);

	//int meandfac=3;
	//if (meanderFactor==0.5) {meandfac=8;}
	//if (meanderFactor==1) {meandfac=6;}
	//if (meanderFactor==2) {meandfac=4;}

	//create randomize river sections
	//with randomized lenght between 8 - 32 % of total length
	int fPCount=0;
	int riverfp[50];
	for (int i=0;i<50;i++)
	{
		riverfp[i]=0;
	}
	for (int h=0; h<riverPoints; h+=floor(riverPoints*0.01*(8+(rand()%(25)))))
	{
		riverfp[fPCount]=h;
		if (h>0)
		{
			for (int i=0;i<width_;i++)
			{
				for(int j=0;j<height_;j++)
				{

					if (id[h]->getX()==i && id[h]->getY()==j)
					{
						//calc new points with meanderWidth and meander factor;
						id[h]->setfP(1);
						/*
						rSecProb river section probability
						meanderfactor between 0.5 - 2 = number of
						meander in riversection
						*/
						int rSecProb=(0+(rand()%(meanderFactor*2+1)))-1;
						for (int k=0;k<rSecProb;k++)
						{
							/*
							if there is a meander in a river section rSec
							ti is set between 40 and 60 %
							of the lenght of this river section
							*/
							int rSec=riverfp[fPCount]-riverfp[fPCount-1];
							//rSec=floor(rSec*(40+(rand()%(20)))*0.01);
							rSec*=0.5;
							double nVec=0;
							/*
							meanderWidth sets lenght of normalvector size
							2 Percent of datalayer width * 8|5|1 * 0-100%
							*/
							int mW=(height_*0.02*(50+(rand()%(50)))*0.01);

							if (meanderWidth==1) {nVec=8*mW;}
							if (meanderWidth==2) {nVec=5*mW;}
							if (meanderWidth==3) {nVec=1*mW;}



							/*
							randomly set on which river side to meander
							signum between 0,1 for - +
							*/
							int sign=(0+(rand()%(2)));
							if (sign==0) {nVec=-nVec;}
							id[h-rSec]->setPoint(id[h-rSec]->getX()+floor(nVec*id[h-rSec]->getnVec()[0]),
												 id[h-rSec]->getY()+floor(nVec*id[h-rSec]->getnVec()[1]),
												 id[h-rSec]->getElev(),
												 0,
												 0);
							id[h-rSec]->setfP(1);
						}
					}
				}
			}
		}
		fPCount+=1;
	}




	//write river supporting points
	//-------------------------//
	double **divdifVec;
	divdifVec = new double*[50];
	for (int i=0;i<50;i++)
	{
		riverfp[i]=0;
		divdifVec[i]= new double [3];
		for (int l=0;l<3;l++)
		{
			divdifVec[i][l]=0;
		}

	}
	int v=0;
	double actualRiverWidth=riverWidth;
	for (int q=0; q<riverPoints;q++)
	{
		if (id[q]->getfP()==1)
		{
			QPointF p(0,0);
			p.setX(id[q]->getX());
			p.setY(id[q]->getY());
			actualRiverWidth*=((90+rand()%(30))*0.01);

			basicRiver.addPoint(p,
								id[q]->getElev(),
								actualRiverWidth);

			divdifVec[v][0]=id[q]->getX();
			divdifVec[v][1]=id[q]->getY();
			divdifVec[v][2]=id[q]->getElev();
			v+=1;

		}
		if (q==riverPoints-1)
		{
			QPointF p(0,0);
			p.setX(width_);
			p.setY(id[q]->getY());
			basicRiver.addPoint(p,
								id[q]->getElev()-0.01,
								actualRiverWidth);


		}

	}



	mainRiver.addSegment(basicRiver);
	editor visualisation(layer_);
	visualisation.paintOrga(mainRiver,topoStatus,Interpolation::NatCubic,10);
	visualisation.paintOrga(mainRiver,topoElevation,Interpolation::NatCubic,-1);


	for (int i=0;i<50;i++)
	{
		delete divdifVec[i];
	}

	delete []divdifVec;


};

};
