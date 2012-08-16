#include "Interpolation.h"
#include <iostream>
#include <math.h>


namespace VIBe{


/** @brief constructor interpolaton 
  *
  * \param t interpolation type 'Linear' or 'NatCubic'
  * \param steps interpolation steps
  * \param cellSize	cellSize of dataLayer
  */
Interpolation::Interpolation(InterpolationType t, int steps, int cellsize)
{
    if ( (t <0) || (t >1) ) std::cout << "Iterpolation type could only be 'Linear' or 'NatCubic'" << std::endl;
    type = t;
    this->steps = steps;
    pts = new QPolygonF();
    npoints = 0;
    this->cellsize = cellsize;
}


/** @brief destructor interpolaton 
  *
  */
Interpolation::~Interpolation()
{
    delete pts;
}

/** @brief calculate natural cubic 
  *
  * 
  * \param n spline parameter
  * \param x spline parameter
  */
Cubic* Interpolation::calcNaturalCubic(const int n, double *x)
{


/* calculates the natural cubic spline that interpolates
y[0], y[1], ... y[n]
The first segment is returned as
C[0].a + C[0].b*u + C[0].c*u^2 + C[0].d*u^3 0<=u <1
the other segments are in C[1], C[2], ...  C[n-1] */


    float *gamma = new float[n+1];
    float *delta = new float[n+1];
    float *D = new float[n+1];
    int i;
    /* We solve the equation
       [2 1       ] [D[0]]   [3(x[1] - x[0])  ]
       |1 4 1     | |D[1]|   |3(x[2] - x[0])  |
       |  1 4 1   | | .  | = |      .         |
       |    ..... | | .  |   |      .         |
       |     1 4 1| | .  |   |3(x[n] - x[n-2])|
       [       1 2] [D[n]]   [3(x[n] - x[n-1])]

       by using row operations to convert the matrix to upper triangular
       and then back sustitution.  The D[i] are the derivatives at the knots.
       */

    gamma[0] = 1.0f/2.0f;
    for ( i = 1; i < n; i++)
    {
        gamma[i] = 1/(4-gamma[i-1]);
    }
    gamma[n] = 1/(2-gamma[n-1]);

    delta[0] = 3*(x[1]-x[0])*gamma[0];
    for ( i = 1; i < n; i++)
    {
        delta[i] = (3*(x[i+1]-x[i-1])-delta[i-1])*gamma[i];
    }
    delta[n] = (3*(x[n]-x[n-1])-delta[n-1])*gamma[n];

    D[n] = delta[n];
    for ( i = n-1; i >= 0; i--)
    {
        D[i] = delta[i] - gamma[i]*D[i+1];
    }

    /* now compute the coefficients of the cubics */
    Cubic* C = new Cubic[n];
    for ( i = 0; i < n; i++)
    {
        C[i] = Cubic((float)x[i], D[i], 3*(x[i+1] - x[i]) - 2*D[i] - D[i+1],
                     2*(x[i] - x[i+1]) + D[i] + D[i+1]);
    }
    delete [] gamma;
    delete [] delta;
    delete [] D;
    return C;
}



/** @brief draw a cubic spline
  *
  *
  *\return 0
  *\return p Polygon to paint
  */
QPolygonF Interpolation::getPolygon()
{
    interpolatedsPoints.clear();
    if ( type == Linear )
    {
        QPolygonF p;
        for (int i = 0; i < npoints-1; i++)
        {
            double gradient = 0.0;
            double valAdd = 0.0;
            double xAdd = (pts->at(i+1).x()-pts->at(i).x())/steps;
            double yAdd = (pts->at(i+1).y()-pts->at(i).y())/steps;
            //Polygon has sPoint values
            if ( sPts.size() )
            {
                gradient = (sPts.value(i+1).getZ()-sPts.value(i).getZ())/(double)steps;
                valAdd = (sPts.at(i+1).getVal()-sPts.at(i).getVal())/steps;
                if ( i == 0 ) interpolatedsPoints.append( sPoint(QPointF(sPts.at(0).x(),sPts.at(0).y()),sPts.at(0).getZ(),sPts.at(0).getVal()));

                for (int j = 1; j <= steps; j++)
                {
                    double newX = j*xAdd+sPts.value(i).x();
                    double newY = j*yAdd+sPts.value(i).y();
                    double newZ = j*gradient+sPts.value(i).getZ();
                    double newVal = j*valAdd+sPts.value(i).getVal();
                    sPoint temp = sPoint(QPointF(newX,newY),newZ,newVal);
                    if ( !interpolatedsPoints.contains( temp ) )
                        interpolatedsPoints.append( temp );
                }
            }
            p << pts->at(i);
        }
        if (npoints != 0) p << pts->at(npoints-1);
        return p;
    }
    if ( type == NatCubic )
    {
        if (npoints >= 2)
        {
            double *x = new double[npoints];
            double *y = new double[npoints];
            for ( int i = 0; i < npoints ; i++ )
            {
                x[i] = pts->at(i).x();
                y[i] = pts->at(i).y();
            }

            Cubic* X = calcNaturalCubic(npoints-1, x);
            Cubic* Y = calcNaturalCubic(npoints-1, y);

            /* very crude technique - just break each segment up into steps lines */
            QPolygonF p;
            p << QPointF(X[0].eval(0),Y[0].eval(0));
            //std::cout << "X= " << X[0].eval(0) << " Y= " << Y[0].eval(0) << std::endl;
            for (int i = 0; i < npoints-1; i++)
            {
                double gradient = 0.0;
                double valAdd = 0.0;
                if ( sPts.size() )
                {
                    gradient = (sPts.value(i+1).getZ()-sPts.value(i).getZ())/(double)steps;
                    valAdd = (sPts.at(i+1).getVal()-sPts.at(i).getVal())/steps;
                    if ( i == 0 ) interpolatedsPoints.append( sPoint(QPointF(sPts.at(0).x(),sPts.at(0).y()),sPts.at(0).getZ(),sPts.at(0).getVal()));
                }
                for (int j = 1; j <= steps; j++)
                {
                    double u = j / (double) steps;
                    p << QPointF(X[i].eval(u),Y[i].eval(u));
                    if ( sPts.size() )
                    {
                        double newVal = j*valAdd+sPts.value(i).getVal();
                        double newz = j*gradient+sPts.value(i).getZ();
                        sPoint temp = sPoint(QPointF(X[i].eval(u),Y[i].eval(u)),newz,newVal);
                        if ( !interpolatedsPoints.contains( temp ) )
                            interpolatedsPoints.append( temp );
                    }
                }
            }
            delete [] X;
            delete [] Y;
            delete [] x;
            delete [] y;
            //if ( sPts.size() )
            //    foreach( sPoint s, interpolatedsPoints )
                //std::cout << "X=" << s.x() << " Y=" << s.y() << std::endl;
            //std::cout << "SIZE p=" << p.size() << std::endl;
            return p;
        }
    }
    return 0;
}



/** @brief getOffsetPolygon
  *
  * get offset Polygon
  *
  */
QPolygonF Interpolation::getOffsetPolygon()
{
    getPolygon();

    QPolygonF top = getParallelPolygon(true);
    QPolygonF under = getParallelPolygon(false);

    QVectorIterator<QPointF> i(under);
    i.toBack();
    while (i.hasPrevious())
    {
        top << i.previous();
    }

    return top;
}

/** @brief getParallelPolygon
  *
  *  get parallel polygon
  */
QPolygonF Interpolation::getParallelPolygon(bool above)
{
    if ( !interpolatedsPoints.size() ) getPolygon();
    int multi = 0;
    if ( above ) multi = 1;
        else multi =-1;

    Interpolation *natTop;
    if ( type == Linear ) natTop = new Interpolation (Interpolation::Linear,1);
    if ( type == NatCubic ) natTop = new Interpolation (Interpolation::NatCubic,1);
    for (int i = 0; i < (interpolatedsPoints.size()); i++)
    {
        //first point
        if ( i == 0 )
        {
            QPointF mirroredFirst = (interpolatedsPoints.at(0) -interpolatedsPoints.at(1))+interpolatedsPoints.at(0);
            natTop->addPoint(getNormalVectorPoint(mirroredFirst,interpolatedsPoints.at(0),interpolatedsPoints.at(1),multi*interpolatedsPoints.at(0).getVal()/cellsize*0.5));
        }
        else
        {
            //last point
            if ( i == interpolatedsPoints.size()-1 )
            {
                QPointF mirroredLast = (interpolatedsPoints.at(i)-interpolatedsPoints.at(i-1))+interpolatedsPoints.at(i);
                natTop->addPoint(getNormalVectorPoint(interpolatedsPoints.at(i-1),interpolatedsPoints.at(i),mirroredLast,multi*interpolatedsPoints.at(i).getVal()/cellsize*0.5));
            }
            //every other
            else
                natTop->addPoint(getNormalVectorPoint(interpolatedsPoints.at(i-1),interpolatedsPoints.at(i),interpolatedsPoints.at(i+1),multi*interpolatedsPoints.at(i).getVal()/cellsize*0.5));
        }
    }
    QPolygonF top = natTop->getPolygon();
    delete natTop;
    return top;

}


/** @brief addPoint
  *
  *  
  */
void Interpolation::addPoint(double x, double y)
{
    *pts << QPointF(x,y);
    npoints++;
}

/** @brief addPoint
  *
  *  
  */
void Interpolation::addPoint(QPointF p)
{
    addPoint(p.x(),p.y());
}

/** @brief addPoint
  *
  *  
  */
void Interpolation::addPoint(sPoint p)
{
    addPoint(p.x(),p.y());
    sPts.append(p);
}

/** @brief addPoints
  *
  *  
  */
void Interpolation::addPoints(segment seg)
{
    foreach ( sPoint s, *seg.getPoints() )
    {
        addPoint(s);
    }
}

/** @brief clearPoints
  *
  *  
  */
void Interpolation::clearPoints()
{
    pts->clear();
    sPts.clear();
    interpolatedsPoints.clear();
    npoints = 0;
}

/** @brief getSteps
  *
  *  
  */
int Interpolation::getSteps() const
{
    return steps;
}

/** @brief getSamplingPoints
  *
  *  
  */
QPolygonF Interpolation::getSamplingPoints()
{
    return *pts;
}

/** @brief getInterpolatedsPoints
  *
  *  
  */
QVector<sPoint>* Interpolation::getInterpolatedsPoints()
{
    return &interpolatedsPoints;
}

/** @brief getNormalVectorPoint
  *
  *  
  */
QPointF Interpolation::getNormalVectorPoint( QPointF before, QPointF actual, QPointF after, double factor )
{
    QPointF temp;
    temp = after - before;
    double switcher;
    switcher = temp.x();
    temp.setX(temp.y());
    temp.setY(switcher);
    temp.setY( temp.y()*-1);
    double tempx = temp.x()*temp.x();
    double tempy = temp.y()*temp.y();
    if (fabs(sqrt(tempx+tempy))) temp/=sqrt(tempx+tempy);
    temp*=factor;
    temp+=actual;
    return temp;
}

/** @brief decodeColor
  *
  * returns the double value for the encoded color value
  */
double Interpolation::decodeColor(QColor c)
{
    if ( c.red() == 0 && c.green()==0 && c.blue()==0 ) return -1.0;
    else return (double)((c.red()*256+c.green())*256+c.blue())/(double)1000.0;
}

/** @brief encodeColor
  *
  * returns the encoded color value for the given double value
  */
QColor Interpolation::encodeColor(double value)
{
    if ( value < 0 && value !=-1.0 ) std::cout << "Es dürfen keine negativen Werte mit Farben codiert werden!" << std::endl;
    int intval = value*1000;
    int bValue = intval %256;
    intval -= bValue;
    intval/=256;
    int gValue = intval % 256;
    intval -= gValue;
    intval/=256;
    int rValue = intval % 256;
    //qDebug("Wert=%f -> RGB=%i,%i,%i",value,rValue,gValue, bValue);
    if (value == -1.0) return QColor(0,0,0);
    return QColor( rValue, gValue, bValue );
}

/** @brief getOptimalStepAmount
  *
  * return the stepsize set in the Interpolation-Constructor, which is optimal for the given length and cellsize
  */
int Interpolation::getOptimalStepAmount(double polylength, int cellsize)
{
    return (polylength/cellsize)*2;
}

/** @brief makeMiniPolygons
  *
  * Returns a vector of small polygons (trapez form)
  */
QVector<QPolygonF> Interpolation::makeMiniPolygons(QPolygonF poly)
{
    QVector<QPolygonF> miniPolys;
    int size = poly.size();
    int undercount=size-1;
    for ( int i=0;i<size/2;i++ )
    {
        QPolygonF p;
        p << poly.at(i);
        p << poly.at(i+1);
        p << poly.at(undercount-1);
        p << poly.at(undercount);
        undercount--;
        miniPolys.append(p);
    }
    return miniPolys;
}


};