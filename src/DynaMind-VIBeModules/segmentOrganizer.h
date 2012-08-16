#include "dmcompilersettings.h"

#ifndef STRUCORGANIZER_H
#define STRUCORGANIZER_H

#include <QtCore>
#include <iostream>

/* Example:
    segmentOrganizer orga;
    segment neu(1,&orga);
    neu.addPoint( QPointF(20.3547,12.3333),0.0,0.0);
    segment seg(0,&orga);
    seg.addPoint( QPointF(20.3555,12.3333),0.0,0.0);
    seg.addPoint( QPointF(20.3545,12.3333),0.0,0.0);
    seg.addPoint( QPointF(20.3555,12.6333),0.0,0.0);
    seg.addPoint( QPointF(21.5555,13.6333),0.0,0.0);

    QVector<sPoint>* points = seg.getPoints();
    foreach ( sPoint s, *points )
    {
        std::cout << s.getPointID() << std::endl;
    }
*/

namespace VIBe{

class sPoint;
class segment;
class segmentOrganizer;


/** @brief coordinate, z-value, value, id administration
  *
  */
class DM_HELPER_DLL_EXPORT sPoint : public QPointF
{
public:
    sPoint();
    sPoint(QPointF p, double z=0, double val = 1, int id = -1);
    QPointF getPointF() const;
    double getVal() const;
    int getPointID() const;
    double getZ() const;
    void setVal( double v );
    void setPointID( double id );
    void setZ( double z );
protected:
private:
    double z_;
    double val_;
    int pointID_;
};


/** @brief sPoint organizer
  *
  */
class DM_HELPER_DLL_EXPORT segment
{
public:
    segment();
    segment(int ord, segmentOrganizer* org);
    void addPoint( QPointF p, double z, double val );
    QVector<sPoint>* getPoints();
        inline int getOrder() {return order;}
	sPoint getsPointFromId(int id);

protected:

private:
    QVector<sPoint> points;
    int order;
    segmentOrganizer *organizer;
};

/** @brief segmentOrganizer
  *
  * administrates segments containing sPoints
  * containing a dublicate point control
  */
class DM_HELPER_DLL_EXPORT segmentOrganizer
{
public:
    	segmentOrganizer();
    	void addSegment( segment s );
    	int getPointID( QPointF p );
	int getPointID();
    	QVector<segment>* getSegments();
	QVector<QPointF>* getPointPool();
	QPointF getPointFFromID(int id);
	sPoint getSPointFromID(int id);
	void removeSegment(int id);
	void replaceSegment(int id, segment s);

protected:
private:
    int getNextPointID();

    QVector<segment> segments;
    QVector<QPointF> points;
    int pointCounter;
    int precision;
};


QPointF round( QPointF p, int precision = 1000 );
/*{
    QPointF temp = p*precision;
    double x = temp.x();
    x=qRound(x);
    x/=precision;
    double y = temp.y();
    y=qRound(y);
    y/=precision;
    temp.setX( x );
    temp.setY( y );
    return temp;
}*/
}

#endif // STRUCORGANIZER_H
