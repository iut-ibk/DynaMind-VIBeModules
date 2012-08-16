
#include "rules.h"
//#include "dataLayer.h"
//#include <iostream>
namespace VIBe{


 /**
  * \brief rule_04
  *
  * calculates gradient field
  *
  */
void rule::rule_04(int layerName)
{
        layer_[topoGradient]->setSwitch(2);
	//double *nMoore3x3;
	double bufferVal=0;
	double gradVal=0;
	int whichCell=4;
	double cellVal=0;
	double cellSize=layer_[layerName]->getcellSize();
	double currentVal=0;

	for (int i=0;i<layer_[layerName]->getwidth();i++)
	{
		for (int j=0;j<layer_[layerName]->getheight();j++)
		{
			nMoore3x3=Moore3x3(i,j,layer_[layerName]);
			bufferVal=0;
			gradVal=0;
			whichCell=4;
			cellVal=nMoore3x3[4];

			for (int k=0; k<9;k++)
			{
				currentVal=nMoore3x3[k];


				if (k==1 || k==3 || k ==5 || k==7)
				{
					bufferVal=(cellVal-currentVal)/cellSize;
					if (fabs(gradVal) < fabs (bufferVal))
					{
						gradVal=bufferVal;
						whichCell=k;

					}
				}
				else
				{
					bufferVal=(cellVal-currentVal)/(sqrt(2.0)*cellSize);
					if (fabs(gradVal) < fabs (bufferVal))
					{
						gradVal=bufferVal;
						whichCell=k;

					}
				}

			}
			//whichCell code
			layer_[topoGradient]->setVal(i,j,gradVal);
		}
	}

};

 /**
  * \brief r_initLandUse
  *
  * applies landuse smoothing
  *
  */
void rule::r_initLandUse()
{
	applySwitch_1(initLandUse);

	int cellVal=0;
	int currentVal=0;
	int countCurrent=0;

	int ForestbufferCount=0;


	int AgribufferCount=0;


	for (int i=0; i<9;i++)
	{
		nMoore3x3[i]=NULL;
	}

	for (int i=0;i<width_;i++)
	{
		for (int j=0;j<height_;j++)
		{
			nMoore3x3=Moore3x3(i,j,layer__[initLandUse]);
			cellVal=int(nMoore3x3[4]);
			currentVal=0;
			countCurrent=0;
			//count forests
			ForestbufferCount=0;

			//count agriculturalAreas
			AgribufferCount=0;

			for (int k=0; k<9;k++)
			{
				if (k==4) {k+=1;}
				currentVal=int(nMoore3x3[k]);

				if (currentVal==Forests)
					ForestbufferCount+=1;

				if (currentVal==AgriculturalAreas)
					AgribufferCount+=1;
			}
			
			//if (ForestbufferCount<4 )
			//no changes at boarder
			if (i>0 && j>0 && i<width_-1 && j < height_-1)
			{
				if (ForestbufferCount>5 )
				{
					if ((cellVal ==AgriculturalAreas ))
						layer__[initLandUse]->setVal(i,j,Forests);
					else
						layer__[initLandUse]->setVal(i,j,cellVal);
				}
				else
				{
					if(AgribufferCount>2 && cellVal==Forests)
						layer__[initLandUse]->setVal(i,j,AgriculturalAreas);
					else
						layer__[initLandUse]->setVal(i,j,cellVal);
				}
			}
			else
				layer__[initLandUse]->setVal(i,j,cellVal);
		}
	}
	applySwitch_2(initLandUse);

};

 /**
  * \brief r_initToLandUse
  *
  * calculates initial landuse to landuse
  *
  */
void rule::r_initToLandUse()
{
	applySwitch_1(LandUse);
	for (int i=0;i<layer__[initLandUse]->getwidth();i++)
	{
		for (int j=0;j<layer__[initLandUse]->getheight();j++)
		{
			layer__[LandUse]->setVal(i,j,
			layer__[initLandUse]->getVal(i,j));
		}
	}
	applySwitch_2(LandUse);
	
};

};
