


#include "d_topoCreator.h"
#include <cmath>

static const double Pi = 3.14159265358979323846264338327950288419717;

namespace VIBe{

 /**
  * \brief  topoRiverArea
  *
  * defines closer  area in which there is
  * a possibility for the river to meander
  *
  * linear base trend of the topology elevation
  * in river direction
  * (depending on elevSlope and elev)
  *
  * defines an quadratic trend lateral to the river
  * (depending on maxdHeight)
  *
  * \param	datalayer   	
  *
  */
void topoCreator::topoRiverArea(dataLayer **layer_)
{


	//-------------------------//
	int yDef=0;
	int yCount=0;
	int yMin=height_;
	int yMax=0;
	int yMinR=0;
	int yMaxR=0;
	int yCountR=0;

	double coeffHeight=0;

	yData = new int [4*width_];
	for (int i=0;i<4*width_;i++)
	{
		yData [i]=0;
	}
	//-------------------------//


	//evaluate attributes of main river
	//-------------------------//
	for (int x=0;x<width_;x++)
	{

		yCount=0;
		yCountR=0;
		yMin=0;
		yMax=0;
		yMinR=0;
		yMaxR=0;

		for (int y=0;y<height_;y++)
		{

			if (layer_[topoStatus]->getVal(x,y)==6) //if no river
			{
				if (yCount==0) {yMin=y;}
				if (yMax<y) {yMax=y;}
				yCount+=1;
			}
			if (layer_[topoStatus]->getVal(x,y)==10)
			{
				if (yCountR==0) {yMinR=y;}
				if (yMaxR<y) {yMaxR=y;}
				yCountR+=1;
			}


		}

		//local evaluation for each x
		//-------------------------//
		yData[x]=yCount;
		yData[1*width_+x]=yMin;
		yData[2*width_+x]=yMax;
		//-------------------------//

		//local elvation base trend in meanderWidth
		//in y direction for drainage
		//-------------------------//
		double Elev_yMin=layer_[topoElevation]->getVal(x,yMin);
		double Elev_yMax=layer_[topoElevation]->getVal(x,yMax);
		double ElevRiver=layer_[topoElevation]->getVal(x,yMaxR);

		for (int yy=yMin;yy<yMinR;yy++)
		{
			double currentdElev=(yy-yMin);
			currentdElev/=(yMinR-yMin);
			currentdElev*=(0.5*(-Elev_yMin+ElevRiver));
			layer_[topoElevation]->addVal(x,yy,currentdElev);
		}
		for (int yy=yMaxR+1;yy<yMax;yy++)
		{
			double currentdElev=(yy-yMaxR);
			currentdElev/=(yMax-yMaxR);
			currentdElev=1-currentdElev;
			currentdElev*=(0.5*(-Elev_yMax+ElevRiver));
			layer_[topoElevation]->addVal(x,yy,currentdElev);

		}


		//-------------------------//



		//reinitilize
		//-------------------------//
		yDef=height_-yMax;

		if (yDef<yMin)
		{
			yDef=yMin;
		}

		yData[3*width_+x]=yDef;
		yCount=0;
		//-------------------------//
	}

	//-------------------------//


	//global evaluation river attributes
	//-------------------------//
	for (int x=0; x<width_;x++)
	{
		if (yData[3*width_+x]>yDef) {yDef=yData[3*width_+x];}
	}

	//evaluate elevation of topology
	//determined by elevation=coeffHeight*dy*dy
	//-------------------------//
	coeffHeight=maxdHeight/(yDef*yDef);

	for (int x=0;x<width_;x++)
	{
		for (int y=0; y<height_;y++)
		{
			if (y==yData[1*width_+x])
			{
				int buffery=0;
				double newElevation=0;
				while ((y-buffery)>=0)
				{
					newElevation=layer_[topoElevation]->getVal(x,yData[1*width_+x]);
					newElevation=newElevation+coeffHeight*(buffery)*(buffery);
					layer_[topoElevation]->setVal(x,(y-buffery),newElevation);
					buffery+=1;
				}
			}

			if (y==yData[2*width_+x])
			{
				int buffery=0;
				double newElevation=0;
				while ((y+buffery)<height_)
				{
					newElevation=layer_[topoElevation]->getVal(x,yData[2*width_+x]);
					newElevation=newElevation+coeffHeight*(buffery)*(buffery);
					layer_[topoElevation]->setVal(x,(y+buffery),newElevation);
					buffery+=1;
				}
			}
		}
	}
	//-------------------------//

	delete []yData;
};




 /**
  * \brief  creates Hill
  *  
  * geometry with analytical functions
  *
  */
void topoCreator::createHill(dataLayer **layer_)
{
	double randHeight=0+(rand()%(100))*0.01*maxHilldHeight;
	int randAngle=0+(rand()%(360));

	dataLayer *Area=layer_[topoElevation]->rotatedArea(randAngle);
	int AreaHeight=Area->getheight();
	int AreaWidth=Area->getwidth();

	int riverCheck=0;

	int xStartPos=AreaWidth;
	int yStartPos=AreaHeight;
	//int xEndPos=0;
	//int yEndPos=0;
	int xEndPos=AreaWidth;
	int yEndPos=AreaHeight;

	int xPos=0+(rand()%(AreaWidth));
	int yPos=0+(rand()%(AreaHeight));

	//---

	/*    segmentOrganizer orga;
    segment seg(0,&orga);
    for ( int i=0;i<=8;i++ )
    {
        int alpha = (360/8)*i;
        int length = rand()%40+1;
        double radAlpha = alpha / 180 * Pi;
        double x_koordinate= cos( radAlpha ) * length + xPos;
        double y_koordinate= sin( radAlpha ) * length + yPos;
        seg.addPoint( QPointF(x_koordinate,y_koordinate),0.0,0 );
    }
    Interpolation natsPoint(Interpolation::NatCubic,10);
    natsPoint.addPoints(seg);
    QPolygonF ground = natsPoint.getPolygon();

    const QPolygon tempPoly =ground.toPolygon();
    QRegion regGround(tempPoly);
    bool intersection = region.intersects( regGround );
    if ( intersection ) std::cout << "ueberschneidung" << std::endl;
*/
//---

	layer_[topoElevation]->setSwitch(2);

	//check extent
	//-------------------------//
	for (int i=0;i<AreaWidth;i++)
	{
		for (int j=0;j<AreaHeight;j++)
		{
			if (Area->getVal(i,j)>0 && xStartPos>i) {xStartPos=i;}
			if (Area->getVal(i,j)>0 && xEndPos<i) {xEndPos=i;}
			if (Area->getVal(i,j)>0 && yStartPos>j) {yStartPos=j;}
			if (Area->getVal(i,j)>0 && yEndPos<j) {yEndPos=j;}
		}
	}
	//-------------------------//


	//-------------------------//
	//-------------------------//
	//if (rand()%(2)>0)
	//{
	for (int i=xPos+xStartPos;i<xPos+xEndPos;i++)
	{
		for (int j=yPos+yStartPos;j<yPos+yEndPos;j++)
		{

			if (j < height_ && i < width_)
			{
				if (layer_[topoStatus]->getVal(i,j)>5)
				{
					riverCheck+=1;
					if (layer_[topoStatus]->getVal(i,j)==10)
					{
						riverCheck+=10;
					}

				}
			}
		}
	}

	if (riverCheck<10)
	{
		for (int i=xStartPos;i<xEndPos;i++)
		{
			for (int j=yStartPos;j<yEndPos;j++)
			{
					//-------------------------//
					if (Area->getVal(i,j)>0)
					{
						if (layer_[topoElevation]->getheight()>(yPos+j) &&
							layer_[topoElevation]->getwidth()>(xPos+i))
							{
								layer_[topoElevation]->setVal(i+xPos,j+yPos,
									layer_[topoElevation]->getVal(xPos+i,yPos+j)+
									randHeight*Area->getVal(i,j));
							}
					}



			}
		}
	}
	riverCheck=0;
	for (int i=xPos-xStartPos;i>xPos-xEndPos;i--)
	{
		for (int j=yPos-yStartPos;j>yPos-yEndPos;j--)
		{

			if ((j < height_ && i < width_) && i>0 && j>0)
			{
				if (layer_[topoStatus]->getVal(i,j)>5)
				{
					riverCheck+=1;
					if (layer_[topoStatus]->getVal(i,j)==10)
					{
						riverCheck+=10;
					}
				}
			}
		}
	}

	if (riverCheck<10)
	{
		for (int i=xStartPos;i<xEndPos;i++)
		{
			for (int j=yStartPos;j<yEndPos;j++)
			{
				if ((yPos-j)>0 && (xPos-i)>0)
				{
					//-------------------------//
					if (Area->getVal(i,j)>0)
					{
						if (layer_[topoElevation]->getheight()>(yPos-j) &&
							layer_[topoElevation]->getwidth()>(xPos-i))
							{
								layer_[topoElevation]->setVal(-i+xPos,-j+yPos,
									layer_[topoElevation]->getVal(xPos-i,yPos-j)+
									randHeight*Area->getVal(i,j));
							}
					}
					//-------------------------//
				}


			}
		}
	}

};


 /**
  * \brief  paint ground polygon of hill
  *
  * \param	QPainter *p. QPolygonF poly
  *
  */
void topoCreator::paintGroundPolygon(QPainter *p,QPolygonF poly)
{
    p->setPen(Qt::NoPen);
    p->drawPolygon(poly,Qt::WindingFill);
}


 /**
  * \brief  creates Hill2
  *  
  * geometry based on a star like shape
  *
  * \param datalayer **layer datalayer basis
  * \param QImage &initialImage image to check overlapping
  * \param int maxGroundLength defines maximal lenght of a beam
  * \param int minGroundLength defines minimal lenght of a beam
  * \param int angleSegments number of beams
  * \param int amountOfHeightLevels
  * \param double hillHeight 
  *
  * \return true if succesfull (no overlapping)
  *
  */
bool topoCreator::createHill2(dataLayer **layer_, QImage &initialImage, int maxGroundLength,int minGroundLength, int angleSegments, int amountOfHeightLevels, double hillHeight)
{
    int hwidth = initialImage.width();
    int hheight = initialImage.height();
    QImage bufferImage = initialImage.copy();
    QPainter painter(&bufferImage);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setBrush(Qt::black);
    int randX = rand()%hwidth;
    int randY = rand()%hheight;
    segmentOrganizer orga;
    segment seg(0,&orga);

    int *lengthA = new int[angleSegments];

    for ( int i=0;i<angleSegments;i++ )
    {
        int alpha = ((360/angleSegments)%360)*i;
        int length = rand()%maxGroundLength+minGroundLength;
        double radAlpha = (alpha*Pi) / 180 ;
        double x_koordinate= cos( radAlpha ) * length + randX;
        double y_koordinate= sin( radAlpha ) * length + randY;
        seg.addPoint( QPointF(x_koordinate,y_koordinate),0.0,0 );
        lengthA[i] = length;
    }
    sPoint p = seg.getPoints()->at(0);
    seg.addPoint(p.getPointF(),0.0,0);
    p = seg.getPoints()->at(1);
    seg.addPoint(p.getPointF(),0.0,0);

    Interpolation natsPoint(Interpolation::NatCubic,10);
    natsPoint.addPoints(seg);
    QPolygonF ground = natsPoint.getPolygon();

    paintGroundPolygon(&painter,ground);
    if ( bufferImage != initialImage )
    {
        //std::cout << "uebrschneidung!" << std::endl;
        return false;
    }
    else
    {
       std::cout << "|";
        paintGroundPolygon(&painter,ground);
        //cnt++;

        // Ground Levels are painted from the lowest groundspline to the highest
        int heightstep = 0;
		double startAlpha=-90+rand()%90;
        for ( int step = amountOfHeightLevels-1; step > 0; step--)
        {
            heightstep++;
            segment seg(1,&orga);
            for ( int i=0;i<angleSegments;i++ )
            {
                int alpha = ((360/angleSegments)%360)*i;
                int length = (lengthA[i]/amountOfHeightLevels)*step;
                double radAlpha = (alpha*Pi) / 180 ;
                double x_koordinate= cos( radAlpha ) * length + randX;
                double y_koordinate= sin( radAlpha ) * length + randY;
                seg.addPoint( QPointF(x_koordinate,y_koordinate),0.0,0 );
            }
            sPoint p = seg.getPoints()->at(0);
            seg.addPoint(p.getPointF(),0.0,0);
            p = seg.getPoints()->at(1);
            seg.addPoint(p.getPointF(),0.0,0);

            Interpolation natsPoint(Interpolation::NatCubic,10);
            natsPoint.addPoints(seg);
            QPolygonF ground = natsPoint.getPolygon();
			paintGroundPolygon(&painter,ground);

			double height=0.0;
			double alpha=( startAlpha+((90-startAlpha)/(amountOfHeightLevels-1))*heightstep) ;
			alpha*=Pi/180.0;
			//factor for hillHeight normalized to 0-1
			double factor=(1/(1-sin(startAlpha*Pi/180)));
			height=hillHeight*( sin(alpha)-sin(startAlpha*Pi/180))*factor;

            painter.setBrush( Interpolation::encodeColor(height));
            paintGroundPolygon(&painter,ground);



        }

        //read Picture and add heigths to the layer
		//90 % of the hills are with positive height
		//10% with negative
		int sign=rand()%11; 
		if  (sign==0)  sign=-1;
		else sign=1;
        for ( int i =0; i<hwidth;i++)
        {
            for ( int j=0;j<hheight;j++ )
            {
                if ( Interpolation::decodeColor(QColor(bufferImage.pixel(i,j)))!=-1.0 )
                {
                    //std::cout << "i:j: " << i << " " << j <<  std::endl;
                    double addVal = Interpolation::decodeColor(QColor(bufferImage.pixel(i,j)));
                    layer_[topoElevation]->addVal(i,j,sign*addVal);
                }

            }
        }
    }
    delete [] lengthA;
    return true;

}


 /**
  * \brief  creates mulitple hills with createhill2
  *
  *
  * \param	dataLayer **layer_ 
  *
  */
void topoCreator::createHills(dataLayer **layer_)
{
    QVector<QRgb> colorTable;
    colorTable.append(0x000000);
    colorTable.append(0xFFFFFF);
    colorTable.append(0xFF0000);
    int hwidth = layer_[topoStatus]->getwidth();
    int hheight = layer_[topoStatus]->getheight();
    QImage image(hwidth,hheight,QImage::Format_RGB32);
    image.setColorTable(colorTable);
    QImage transfer(hwidth,hheight,QImage::Format_RGB32);

    for ( int i =0; i< hwidth;i++)
    {
        for (int j=0; j < hheight;j++ )
        {
            if (layer_[topoStatus]->getVal(i,j) >= 9 /*|| layer_[topoStatus]->getVal(i,j) == 6*/ )
            {
                image.setPixel(i,j,1);
            }
            else image.setPixel(i,j,0);
        }
    }
    //image.save("initial.png");

    QPainter picturePainter(&transfer);

    picturePainter.setPen(Interpolation::encodeColor(0));
    QBrush brush(Interpolation::encodeColor(0));
    picturePainter.setBrush(brush);


	 //make hills, amount is controlled with the condition in the while loop
    int cnt=0;
    while (cnt < maxNoHills/2)
    {
		int basis=30+rand()%30;
		int maxGroundLenght=2*basis;// + rand() % (50);
		int minGroundLenght=basis;// + rand() % (20);
		int numberStreams=8;//5+ rand () % (4);
		
		int hillHeight=int (maxHilldHeight*(25+ rand() % (75))*0.01);
		int heightLevels=basis;//hillHeight*10;//int (hillHeight*2);

        if ( createHill2(layer_,
						image,
						maxGroundLenght,
						minGroundLenght,
						numberStreams,
						heightLevels,
						hillHeight) 
						)
		{cnt++;}
    }
	cnt=0;
	while (cnt < maxNoHills/2)
    {
		int basis=10+rand()%20;
		int maxGroundLenght=2*basis;// + rand() % (50);
		int minGroundLenght=basis;// + rand() % (20);
		int numberStreams=8;//5+ rand () % (4);
		
		int hillHeight=int (maxHilldHeight*(25+ rand() % (25))*0.01);
		int heightLevels=basis;//hillHeight*10;//int (hillHeight*2);

        if ( createHill2(layer_,
						image,
						maxGroundLenght,
						minGroundLenght,
						numberStreams,
						heightLevels,
						hillHeight) 
						)
		{cnt++;}
    }

};


};
