/*
editor.cpp
20080210 inital (H. Kinzel)
*/

#include "editor.h"
#include "Interpolation.h"


namespace VIBe{


 /**
  * \brief  constructor editor
  *
  * initialize layer_ with NULL
  *
  */
editor::editor()
{
	layer_=NULL;

};

 /**
  * \brief  constructor editor
  *
  * initialize layer_ parameter
  *
  * \param layer_ data basis
  */
editor::editor(dataLayer **layer)
{
    layer_=layer;
};

 /**
  * \brief  paint segment
  *
  * paint segment with interpolation method with addVal or setVal
  *
  * \param seg segment to paint
  * \param layer layer to paint on
  * \param interpolType Interpolation method natCubic or linear
  * \param paintVal value to paint with
  * \param addVal value to add
  *
  * \return points array of interpolated points
  */
QVector<sPoint> editor::paintSeg(segment seg, int layer,Interpolation::InterpolationType interpolType, double paintVal, double addVal) {
    double cellSize_=layer_[layer]->getcellSize();
    double width_=layer_[layer]->getwidth();
    double height_=layer_[layer]->getheight();

    QImage image(width_,height_,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setBackgroundMode(Qt::OpaqueMode);
    QBrush background( Interpolation::encodeColor( -1 ) );
    painter.setBackground( background );
    painter.eraseRect(image.rect());



	QVector<sPoint> points;


        Interpolation natsPoint(interpolType, Interpolation::getOptimalStepAmount(width_,cellSize_),cellSize_);
        natsPoint.addPoints(seg);
        QPolygonF river = natsPoint.getOffsetPolygon();
        QVector<sPoint>* interpoints = natsPoint.getInterpolatedsPoints();

        QVector<QPolygonF> minis = Interpolation::makeMiniPolygons(river);

        QColor c = Interpolation::encodeColor(-1);
        for ( int i=0; i< minis.size()-1; i++ )
        {
            double mean = (interpoints->at(i).getZ() + interpoints->at(i+1).getZ())/2.0;

            QPen pen;  // creates a default pen
            pen.setColor( Interpolation::encodeColor(mean) );
            painter.setPen(pen);

            painter.setBrush(Interpolation::encodeColor(mean));
            painter.drawPolygon(minis.at(i));
        }
        //natsPoint.clearPoints();
		points+=*(natsPoint.getInterpolatedsPoints());

    for ( int i =0; i<width_;i++)
    {
        for ( int j=0;j<height_;j++ )
        {
            if ( paintVal == -1 )
            {
                if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1.0 ) 
				{
					if (addVal==0.0) layer_[layer]->setVal(i,j,Interpolation::decodeColor(image.pixel(i,j)));
					else if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1.0 ) layer_[layer]->addVal(i,j,addVal);
				}
            }
            else if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1.0 )
				layer_[layer]->setVal(i,j,paintVal);

        }
    }
	return points;

}


 /**
  * \brief  paint segment organizer consisting of segments
  *
  * paint segment organizer  with interpolation method with addVal or setVal
  *
  * \param s segmentOrganizer to paint
  * \param layer layer to paint on
  * \param interpolType Interpolation method natCubic or linear
  * \param paintVal value to paint with
  * \param addVal value to add
  *
  * \return points array of interpolated points
  */
QVector<sPoint> editor::paintOrga(segmentOrganizer s, int layer,Interpolation::InterpolationType interpolType, double paintVal, double addVal)
{
    double cellSize_=layer_[layer]->getcellSize();
    double width_=layer_[layer]->getwidth();
    double height_=layer_[layer]->getheight();

    QImage image(width_,height_,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setBackgroundMode(Qt::OpaqueMode);
    QBrush background( Interpolation::encodeColor( -1 ) );
    painter.setBackground( background );
    painter.eraseRect(image.rect());

    QVector<segment>* segments = s.getSegments();

	QVector<sPoint> points;


    foreach( segment seg, *segments )
    {
        Interpolation natsPoint(interpolType, Interpolation::getOptimalStepAmount(width_,cellSize_),cellSize_);
        natsPoint.addPoints(seg);
        QPolygonF river = natsPoint.getOffsetPolygon();
        QVector<sPoint>* interpoints = natsPoint.getInterpolatedsPoints();

        QVector<QPolygonF> minis = Interpolation::makeMiniPolygons(river);

        QColor c = Interpolation::encodeColor(-1);
        for ( int i=0; i< minis.size()-1; i++ )
        {
            double mean = (interpoints->at(i).getZ() + interpoints->at(i+1).getZ())/2.0;

            QPen pen;  // creates a default pen
            pen.setColor( Interpolation::encodeColor(mean) );
            painter.setPen(pen);

            painter.setBrush(Interpolation::encodeColor(mean));
            painter.drawPolygon(minis.at(i));
        }
        //natsPoint.clearPoints();
		points+=*(natsPoint.getInterpolatedsPoints());
    }

    for ( int i =0; i<width_;i++)
    {
        for ( int j=0;j<height_;j++ )
        {
            if ( paintVal == -1 )
            {
                if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1.0 ) 
				{
					if (addVal==0.0) layer_[layer]->setVal(i,j,Interpolation::decodeColor(image.pixel(i,j)));
					else if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1.0 ) layer_[layer]->addVal(i,j,addVal);
				}
            }
            else if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1.0 )
				layer_[layer]->setVal(i,j,paintVal);

        }
    }
	
	


	return points;

//	Interpolation i(interpolType,20,cellSize);
//	foreach ( sPoint s, s.getPoints )
//	{
//	    i.addPoint( s );
//	}
    //QPolygonF Poly= i.getOffsetPolygon();


    //QImage image(width,height,rgb);
    //QPainter p(&image);
    //QPen pen(Qt::darkBlue, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    //p.setPen(pen);
    //p.drawPolygon(Poly);
    //


    //layer_[layer]->setVal(1,1,paintVal);

    //for x
    //    for y

	//editor e(layer_);
	//segmentOrganizer o;
	//segment s(0,&o);
	//e.paintSegment(s,1,1,1);

};
	//editor aMaster(layer_);
	//	int maxGroundLenght=50;// + rand() % (50);
	//	int minGroundLenght=40;// + rand() % (20);
	//	int numberStreams=8;//5+ rand () % (4);
	//	
	//	double areaHeight=11;
	//	int heightLevels=20;//areaHeight*5;
	//	QPointF p(200,125);

	// aMaster.createArea(p,
	//					Industry,
	//					maxGroundLenght,
	//					minGroundLenght,
	//					numberStreams,
	//					heightLevels,
	//					areaHeight,editor::exponential);



 /**
  * \brief  creates area
  *  
  * geometry based on a star like shape
  *
  * \param qwer mid point to start
  * \param QImage &initialImage image to check overlapping
  * \param int maxGroundLength defines maximal lenght of a beam
  * \param int minGroundLength defines minimal lenght of a beam
  * \param int angleSegments number of beams
  * \param int amountOfHeightLevels
  * \param double Area Height 
  * \param hInterpolType interpolation typ linear, natCubic, exponetial, sinus
  *
  * \return true if succesfull (no overlapping)
  *
  */
bool editor::createArea(QPointF qwer, int layer, int maxGroundLength,int minGroundLength, int angleSegments, int amountOfHeightLevels, double areaHeight, editor::HeightInterpolation hInterpolType)
{
	int hwidth=layer_[topoStatus]->getwidth();
	int hheight=layer_[topoStatus]->getheight();
	QImage bufferImage(hwidth,hheight,QImage::Format_RGB32);
	bufferImage.fill(0);

    QPainter painter(&bufferImage);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setBrush(Qt::black);
	int randX = qwer.x();//int (hwidth/2);
    int randY = qwer.y();//int (hheight/2);
    segmentOrganizer orga;
    segment seg(0,&orga);

    int *lengthA = new int[angleSegments];

    for ( int i=0;i<angleSegments;i++ )
    {
        int alpha = ((360/angleSegments)%360)*i;
        //int length = minGroundLength + rand()%(maxGroundLength-minGroundLength);
		int length = rand()%maxGroundLength+minGroundLength;
        double radAlpha = (alpha*3.141592654) / 180 ;
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
                double radAlpha = (alpha*3.141592654) / 180 ;
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

			double height=0.0;

			
            //sets the color to draw height
			if (hInterpolType==editor::sinus)
			{
			//sets the color to draw a sinus-like mountain
			/*
				angle for sinus wave from startAlpha to 90 degree
				startAlpha ranges from -90 to -1 degrees
				=> at the boarder of a hill,
				the bend at the start is between (45� for startAlpha == -1)
				and (0� for startAlpha == -90)			
			*/
				double alpha=( startAlpha+((90-startAlpha)/(amountOfHeightLevels-1))*heightstep) ;
                                alpha*=3.141592654/180.0;
				//factor for hillHeight normalized to 0-1
                                double factor=(1/(1-sin(startAlpha*3.141592654/180)));
                                height=areaHeight*( sin(alpha)-sin(startAlpha*3.141592654/180))*factor;
	          
				//height=areaHeight*( sin( ( (90.0/(amountOfHeightLevels-1))*heightstep) *Pi/180.0));
				//height=areaHeight*( sin( ( (-90+180.0/(amountOfHeightLevels-1))*heightstep) *Pi/180.0));
			}

			if (hInterpolType==editor::exponential)
			{
				double A=log(0.01)*(double (heightstep))/double (amountOfHeightLevels);
				height=areaHeight*(1-exp(A));
				
			}

			if (hInterpolType==editor::linear)
			{
                                height=areaHeight*( sin( ( (90.0/(amountOfHeightLevels-1))*heightstep) *3.141592654/180.0));
			}


            
            painter.setBrush( Interpolation::encodeColor(height));
            paintGroundPolygon(&painter,ground);
        }

        //read Picture and add heigths to the layer
        for ( int i =0; i<hwidth;i++)
        {
            for ( int j=0;j<hheight;j++ )
            {
                if ( Interpolation::decodeColor(QColor(bufferImage.pixel(i,j)))!=-1.0 )
                {
                    //std::cout << "i:j: " << i << " " << j <<  std::endl;
                    double addVal = Interpolation::decodeColor(QColor(bufferImage.pixel(i,j)));
					addVal=addVal;
					//addVal*=0.85;
					//if (addVal>1) addVal=1;
                    layer_[layer]->addVal(i,j,addVal);
                }
            }
        }
	//}
    delete [] lengthA;
    return true;

};


 /**
  * \brief  paint ground polygon 
  *
  * \param	p   painter
  * \param poly polygon to paint
  *
  */
void editor::paintGroundPolygon(QPainter *p,QPolygonF poly)
{
    p->setPen(Qt::NoPen);
    p->drawPolygon(poly,Qt::WindingFill);
};










};
