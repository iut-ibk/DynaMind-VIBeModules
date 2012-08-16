#include "d_topoCreator.h"


namespace VIBe{

/*
d_topoParam.cpp
	Parameter variation
	topoCreator::topoCreator(dataLayer **layer_) : dataLayer()
	void topoCreator::loadRiverParam(int kind)
	void topoCreator::loadTopoParam(int kind)

d_genRiver.cpp
	generate river
	topoCreator::riverCreate_2(dataLayer **layer_)
	topoCreator::riverSinuosity(dataLayer **layer_)
	topoCreator::createminorRivers(dataLayer **layer_)
	topoCreator::drawPoints(dataLayer **layer_,
					   double **pointArray,
					   int pointCount,
					   int smoothing_)

d_genRelief.cpp
	generate relief
	void topoCreator::topoRiverArea(dataLayer **layer_)
*/


 /**
  * \brief  generates virtual environment
  *
  *  generates river
  *  river meander area
  *  river sinuosities
  *  topography inclusive hills
  *
  *
  */
void topoCreator::generateTopology(dataLayer **layer_)
{
//generate topology parameters
//-------------------------//
srand(time(NULL)); //init rand()

	/*
	0 : load from input file
	1 : alpine river
	2 : foreland river
	3 : lowland river
	*/
	loadRiverParam(0);

	/*
	0 : load from input file
	1 : alpine city
	2 : foreland city
	3 : lowland city
	*/
	loadTopoParam(0);


	
	//create main river
	std::cout<<"\t"<<" creating main river\n";
	//-------------------------//
		riverCreate_3();
		//riverCreate_2();
		topoRiverArea(layer_);
	//-------------------------//
	

	//create sinuosity of main river
	//-------------------------//	
		for (int i=0; i<noSinuosity; i++)
		{
			riverSinuosity();
		}

	//-------------------------//	


	////create minor rivers
	////-------------------------//
	std::cout<<"\t"<<" creating minor rivers ";
	for (int i=0; i<noMinorRivers; i++)
	{
		createminorRivers2();
		std::cout<<"|";
		//createminorRivers();
	}
	////-------------------------//
	std::cout<<"\n";

	std::cout<<"\t"<<" creating hills";	
	createHills(layer_);
	std::cout<<"\n";	




};













};
