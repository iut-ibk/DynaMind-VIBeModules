


#include "rules.h"



namespace VIBe{

 /**
  * \brief  constructor rule
  *
  *
  */
/*rule::rule(dataLayer **layer_)
{	
	setLayer(layer_);
	setRuleLayer(layer_);
	width_=layer_[1]->getwidth();
	height_=layer_[1]->getheight();
	cellSize_=layer_[1]->getcellSize();

	//init pointer for 
	//neighbourhood::Moore3x3(int x, int y,dataLayer *pLayer)
	//in r_neighbourhood.cpp
	nMoore3x3 = new double[9]; 
	//-------------------------//
	for (int h=0;h<9;h++)//8 neighbours + 1 element
	{
		nMoore3x3[h]=0;	
	}
	//-------------------------//

};*/

 /**
  * \brief  constructor rule
  *
  *
  */
rule::rule(dataLayer **layer_)
{	

        //city_=city;
        dataLayer **layer=layer_;
	//layer_=city_->getLayer();
	setLayer(layer);
	setRuleLayer(layer);
	width_=layer[1]->getwidth();
	height_=layer[1]->getheight();
	cellSize_=layer[1]->getcellSize();

        /*maxCPopDensity=city_->genesis_->getmaxCPopDensity();
	maxDCPopDensity=city_->genesis_->getmaxDCPopDensity();
	maxOBPopDensity=city_->genesis_->getmaxOBPopDensity();

	PopChangeLow=city_->genesis_->popChangeLow;
	popCUFRand=city_->genesis_->popCUFRand;
	popDCUFRand=city_->genesis_->popDCUFRand;
	popAGRIRand=city_->genesis_->popAGRIRand;

	Qh=city_->genesis_->Qh;
	Qf=city_->genesis_->Qf;
	Qi=city_->genesis_->Qi;
	varMin=city_->genesis_->varMin;
        varMax=city_->genesis_->varMax;*/
	dVar=varMax-varMin;

	//init pointer for 
	//neighbourhood::Moore3x3(int x, int y,dataLayer *pLayer)
	//in r_neighbourhood.cpp
	nMoore3x3 = new double[9]; 
	//-------------------------//
	for (int h=0;h<9;h++)//8 neighbours + 1 element
	{
		nMoore3x3[h]=0;	
	}
	//-------------------------//

};

 /**
  * \brief  destructor rule
  *
  *
  */
rule::~rule()
{

	delete []nMoore3x3;
        nMoore3x3=0;
	
}

 /**
  * \brief  applySwitch_1 
  *
  * applies layer switch on layer start of rule
  *
  *\param layer data layer to apply on 
  *
  */
void rule::applySwitch_1(int layer)
{
	if (layer_[layer]->showSwitch()==2) 
	{
		layer_[layer]->setSwitch(1);
	} 
}


 /**
  * \brief  applySwitch_2 
  *
  * applies layer switch on layer end of rule
  *
  *\param layer data layer to apply on 
  *
  */
void rule::applySwitch_2(int layer)
{
	//layer switch - only one rule applicable
		if (layer__[layer]->showSwitch()==1) 
		{
			layer__[layer]->setSwitch(0);
		}  
		else if (layer__[layer]->showSwitch()==0)
		{
			layer__[layer]->setSwitch(1);
		}
		else 
		{
			std::cout<<"error layerswitch \n";
		}
	//-------------------------//	


};

double rule::sumLayer(int layer)
{
	double sum=0;
	for (int i=0; i<width_; i++)
	{
		for (int j=0; j<height_; j++)
		{
			sum+=layer_[layer]->getVal(i,j);
		}
	}
	return sum;

};

};
