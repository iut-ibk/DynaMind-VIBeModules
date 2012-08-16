#include "dmcompilersettings.h"

#ifndef NATCUBIC_H
#define NATCUBIC_H

#include <QtGui>
#include "segmentOrganizer.h"

/*
    QImage image(size(),QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setBackgroundMode(Qt::OpaqueMode);
    QBrush background( Interpolation::encodeColor( -1 ) );
    painter.setBackground( background );
    painter.eraseRect(rect());

    segmentOrganizer orga;
    segment seg(0,&orga);
    seg.addPoint( QPointF(40,40),0.0,10 );
    seg.addPoint( QPointF(100,20),5000.0,12 );
    seg.addPoint( QPointF(180,60),10000.0,14 );
    seg.addPoint( QPointF(300,40),16000.0,25 );
    Interpolation natsPoint(Interpolation::Linear, Interpolation::getOptimalStepAmount(300,20));
	//Interpolation natsPoint(Interpolation::NatCubic, Interpolation::getOptimalStepAmount(300,cellSize_));
    natsPoint.addPoints(seg);
    QPolygonF river = natsPoint.getOffsetPolygon();

    QVector<sPoint>* interpoints = natsPoint.getInterpolatedsPoints();

    QVector<QPolygonF> minis = Interpolation::makeMiniPolygons(river);

    for ( int i=0; i< minis.size()-1; i++ )
    {
        double mean = (interpoints->at(i).getZ() + interpoints->at(i+1).getZ())/2.0;

        QPen pen;  // creates a default pen
        pen.setColor( Interpolation::encodeColor(mean) );
        painter.setPen(pen);

        painter.setBrush(Interpolation::encodeColor(mean));
        painter.drawPolygon(minis.at(i));
    }

    for( int i =0; i<size().width();i++)
    {
        for( int j=0;j<size().height();j++ )
        {
            if ( Interpolation::decodeColor(QColor(image.pixel(i,j)))!=-1 ) std::cout << Interpolation::decodeColor(image.pixel(i,j)) << std::endl;
        }
    }

    QPainter widpainter(this);
    widpainter.drawImage(0,0,image);

*/

namespace VIBe{

/** @brief coefficients of cubic spline
  *
  */
class DM_HELPER_DLL_EXPORT Cubic
{
public:
    Cubic()
    {
        this->a = 0;
        this->b = 0;
        this->c = 0;
        this->d = 0;
    }
    ;
    Cubic(float a, float b, float c, float d)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    };
    float eval( float u)

    {
        return (((d*u) + c)*u + b)*u + a;
    };
protected:

private:
    float a,b,c,d;/* a + b*u + c*u^2 +d*u^3 */

};


/** @brief interpolation class for a cubic spline interpolation
  *
  */
class DM_HELPER_DLL_EXPORT Interpolation
{
public:

    static QColor encodeColor(double value);
    static double decodeColor(QColor c);
    static int getOptimalStepAmount( double polylength, int cellsize);
    static QVector<QPolygonF> makeMiniPolygons(QPolygonF poly);

    enum InterpolationType{ NatCubic, Linear };

    Interpolation(InterpolationType t, int steps = 12, int cellsize = 1);
    ~Interpolation();
    void addPoint(double x, double y);
    void addPoint(QPointF p);
    void addPoint(sPoint p);
    void addPoints(segment seg);
    void clearPoints();
    QPolygonF getPolygon();
    QPolygonF getOffsetPolygon();
    QPolygonF getParallelPolygon( bool above );
    QVector<sPoint>* getInterpolatedsPoints();
    int getSteps() const;
    QPolygonF getSamplingPoints();
protected:

private:
    QPointF getNormalVectorPoint( QPointF before, QPointF actual, QPointF after, double factor );
    Cubic* calcNaturalCubic(const int n, double *x);

    int steps;
    QPolygonF *pts;
    QVector<sPoint> sPts;
    int npoints;
    int cellsize;
    QVector<sPoint> interpolatedsPoints;

    InterpolationType type;

};
};

#endif // NATCUBIC_H
