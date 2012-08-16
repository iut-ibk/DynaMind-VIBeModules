#include "dmcompilersettings.h"

#ifndef __editor
#define __editor

/*
creatorBase.h
20080210 initall (H. Kinzel)
*/

#include <QtGui>
#include "dataLayer.h"
#include "Interpolation.h"


namespace VIBe{
class dataLayer;

/** @brief visualisation class for segmentorganizer
  *
  */
class DM_HELPER_DLL_EXPORT editor
{

 private:
	 dataLayer **layer_;



 public:

  QVector<sPoint> paintOrga(segmentOrganizer s,
					int layer,
					Interpolation::InterpolationType interpolType,
					double paintVal = -1, double addVal =0.0);
  QVector<sPoint> paintSeg(segment s,
					int layer,
					Interpolation::InterpolationType interpolType,
					double paintVal = -1, double addVal =0.0);



  enum HeightInterpolation {linear, sinus, exponential};
  bool createArea(QPointF p,int layer, int maxGroundLength,int minGroundLength, int angleSegments, int amountOfHeightLevels, double areaHeight, HeightInterpolation hInterp);
  void paintGroundPolygon(QPainter *p,QPolygonF poly);



  editor();
  editor(dataLayer **layer);





};

};
#endif

