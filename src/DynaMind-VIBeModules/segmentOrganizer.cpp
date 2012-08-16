#include "segmentOrganizer.h"

//sPoint
namespace VIBe{

sPoint::sPoint()
{
    val_ = -1.0;
    z_ = -1.0;
    pointID_ = -1;
};
sPoint::sPoint(QPointF p, double z, double val, int id) : QPointF(p)
{
    val_ = val;
    z_ = z;
    pointID_ = id;
};
QPointF sPoint::getPointF() const
{
    return (QPointF) *this;
}
double sPoint::getVal() const
{
    return val_;
};
int sPoint::getPointID() const
{
    return pointID_;
};
double sPoint::getZ() const
{
    return z_;
};
void sPoint::setVal( double v )
{
    val_ = v;
};
void sPoint::setPointID( double id )
{
    pointID_ = id;
};
void sPoint::setZ( double z )
{
    z_ = z;
};

//segment

segment::segment()
{
    order = -1;
    organizer = 0;
};
segment::segment(int order, segmentOrganizer* org)
{
    this->order = order;
    if ( org ) organizer = org;
    else std::cout << "organizer darf nicht null sein" << std::endl;
};
void segment::addPoint( QPointF p, double z, double val )
{
    points.append( sPoint( round(p), z, val, organizer->getPointID(round(p)) ) );
};
QVector<sPoint>* segment::getPoints()
{
    return &points;
};

sPoint segment::getsPointFromId(int id)
{
	sPoint returnP;
	for(int i=0; i<points.size(); i++)
	{
		if(id==points.value(i).getPointID())
		{
			returnP=points.value(i);
		}
	}
	//if (returnP.getPointID()==-1)
	//{
	//	std::cout<<"error segmentorganizer, sPoint point "<<id<<" not found\n";
	//}
	return returnP;

};


QPointF segmentOrganizer::getPointFFromID(int id)
{
	sPoint returnsP;
	QPointF returnP;
	foreach (segment seg, segments)
	{
		if (returnsP.getPointID()==-1)
		{
			returnsP=seg.getsPointFromId(id);
		}
	}
	returnP=returnsP.getPointF();
	return returnP;
};

/**
*@brief remove Segment
*@author Chrisitan Urich
*/
void segmentOrganizer::removeSegment(int id) {
	segments.remove(id);
}

sPoint segmentOrganizer::getSPointFromID(int id)
{
	sPoint returnsP;
	foreach (segment seg, segments)
	{
		if (returnsP.getPointID()==-1)
		{
			returnsP=seg.getsPointFromId(id);
		}
	}
	return returnsP;
};

// segmentOrganizer

segmentOrganizer::segmentOrganizer()
{
    pointCounter = 0;
};
void segmentOrganizer::addSegment( segment s )
{
    segments.append( s );
};
void segmentOrganizer::replaceSegment( int id, segment s )
{
    segments.replace(id, s );
};
int segmentOrganizer::getPointID( QPointF p )
{
    if ( points.contains( p ) ) return points.indexOf( p );
    else
    {
        points.append(p);
        return points.count()-1;
    }
};
int segmentOrganizer::getPointID()
{
        return points.count();
};

int segmentOrganizer::getNextPointID()
{
    return pointCounter++;
};


QVector<segment>* segmentOrganizer::getSegments()
{
    return &segments;
}

QVector<QPointF>* segmentOrganizer::getPointPool()
{
	return &points;
};

QPointF round( QPointF p, int precision) {
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
}


};
