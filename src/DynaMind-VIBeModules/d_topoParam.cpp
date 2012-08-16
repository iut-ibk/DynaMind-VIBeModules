





#include "d_topoCreator.h"

namespace VIBe{


 /**
  * \brief  constructor topoCreator
  *
  *  initialize all
  *
  */
topoCreator::topoCreator(dataLayer **layer) //: dataLayer()
{
	//topology lattice attributes
	//-------------------------//

	layer_=layer;
	cellSize_=layer_[topoStatus]->getcellSize();
	width_=layer_[topoStatus]->getwidth();
	height_=layer_[topoStatus]->getheight();
	inputFile="NULL";
	

	//riverPoints=100;
	for (int i=0;i<riverPoints;i++)
	{
		id[i]=new riverStruc();

	}
	for (int i=0;i<4;i++)quadrant[i]=0;


	//init river parameters
	riverSlope=0;
	riverWidth=0;
	rrElev=0;
	meanderFactor=0;
	meanderWidth=0;
	basisLenght=0;

	smoothing=0;
	alpha=0;
	alphaTol=0;
	bendFac=0;

	elev=0;
	elevSlope=0;
	maxdHeight=0;
	maxHilldHeight=0;
	maxNoHills=0;
	yData=0;



	elev=0;
	elevSlope=0;
	riverSlope=0;
	riverWidth=0;
	rrElev=0;
	meanderFactor=0;
	meanderWidth=0;
	smoothing=OFF;





}


/**
  * \brief  destructor topoCreator
  *
  *
  */
topoCreator::~topoCreator()
{
	for (int i=0;i<riverPoints;i++)
	{
		delete id[i];

	}
}

/**
  * \brief  load river parameter
  *
  *
  * \param int kind 	0 : load from inputFile
  *						1 : alpine river
  *						2 : foreland river
  *						3 : lowland river
  */
void topoCreator::loadRiverParam(int kind)
{
/* input arguement: kind of river
	0 : load from inputFile
	1 : alpine river
	2 : foreland river
	3 : lowland river
*/

	//probability for meander factor
	int rm=(0+(rand()%(100)));
	int meandfac=3;

	switch (kind)
	{
	case 0:
		//loadRiverParamFromFile();
		break;

	case 1:
	/*fast flowing river for alpin cities
	with appropiate slope and elevation
	riverSlope between 0.0005 to 0.0025 promille
	drandSlope as fraction of riverSlope for
	variation of riverSlope in subject of flow path*/
		riverSlope=(0+(rand()%(100)))*0.01*0.002+0.0005;

		//river width between 40 - 140 m
			riverWidth=((0+(rand()%(100)))*0.01*100+40);

		//randomRiverElevation below bank between 2 - 7 m
			rrElev=((0+(rand()%(100)))*0.01*5+2);

		//meander ability
		//length between two meander points
			if (rm<=20){ meanderFactor=0.5;}
			else if (rm>20 && rm<=60) {meanderFactor=1;}
			else if (rm>60 && rm<=100) {meanderFactor=2;}
			else {std::cout<<"error meanderFactor, topoCreator::loadRiverParam(int)\n";}
			meanderFactor=2;
			basisLenght=1000; //m


			//meanderWidth sets length
			//of normal vector size
			//2 Percent of datalayer height * 5|3|1 * 0-100%
			//in d_genRiver.cpp
			meanderWidth=1+rand()%(3);
				if (meanderWidth==1) {meanderWidth=6;}
				if (meanderWidth==2) {meanderWidth=5;}
				if (meanderWidth==3) {meanderWidth=4;}


				if (meanderFactor==0.5) {meandfac=8;}
				if (meanderFactor==1) {meandfac=6;}
				if (meanderFactor==2) {meandfac=4;}
				meanderWidth*=(height_*0.02);
				//int dy=floor(height_*0.02);

				//if (meanderWidth==1) {dy*=8;}
				//if (meanderWidth==2) {dy*=5;}
				//if (meanderWidth==3) {dy*=1;}
				//layer_[topoStatus]->setmHeight(dy);
				layer_[topoStatus]->setmHeight(meanderWidth);

		//sinuosity
			noSinuosity=1+rand()%(3);
			noMinorRivers=1+rand()%(4);


		//smoothing
			smoothing=ON;
			alpha=0;
			alphaTol=90;
			bendFac=0.5;
		break;

	case 2:
		riverWidth=0;
		rrElev=0;
		meanderFactor=0;
		break;
	case 3:
		riverWidth=0;
		rrElev=0;
		meanderFactor=0;
		break;

	default:
		std::cout<<"error void topoCreator::loadRiverParam(int kind)\n";




	}
};



/**
  * \brief  load topography parameter
  *
  *
  * \param int kind 	0 : load from inputFile
  *						1 : alpine river
  *						2 : foreland river
  *						3 : lowland river
  */
void topoCreator::loadTopoParam(int kind)
{
	/* input arguement: kind of topology
	0 : load from inputFile
	1 : alpine city
	2 : foreland city
	3 : lowland city
	*/
	switch (kind)
	{
	case 0:
		//loadReliefParamFromFile();
		break;

	case 1:
		elev=100;
		elevSlope=0.0005;
		maxdHeight=100;
		maxHilldHeight=20;
		maxNoHills=40;
		break;
	case 2:
		elev=0;
		elevSlope=0;
		maxdHeight=0;
		maxHilldHeight=0;
		maxNoHills=0;
		break;
	case 3:
		elev=0;
		elevSlope=0;
		maxdHeight=0;
		maxHilldHeight=0;
		maxNoHills=0;
		break;
	default:
		std::cout<<"error void topoCreator::loadTopoParam(int kind)\n";
	}
};



double topoCreator::getrrElev(){return rrElev;};

void topoCreator::setRiverSlope(double r) {riverSlope=r;};
void topoCreator::setRiverWidth(double r) {riverWidth=r;};
void topoCreator::setRiverdHeight(double r) {rrElev=r;};
void topoCreator::setBasisLength(double r) {basisLenght=r;};
void topoCreator::setMeanderWidth(double r) {meanderWidth=int (r/cellSize_);};
void topoCreator::setNoMinorRivers(int r) {noMinorRivers=r;};
void topoCreator::setNoSinuosity(int r) {noSinuosity=r;};
void topoCreator::setBasicElevation(double r) {elev=r;};
void topoCreator::setElevationSlope(double r) {elevSlope=r;};
void topoCreator::setMaximaldHeight(double r) {maxdHeight=r;};
void topoCreator::setMaxHilldHeight(double r) {maxHilldHeight=r;};
void topoCreator::setMaxNoHills(int r) {maxNoHills=r;};

};
