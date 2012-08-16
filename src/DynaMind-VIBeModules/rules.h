#include "dmcompilersettings.h"

#ifndef __rules
#define __rules



//#include "vSys.h"
#include "dataLayer.h"
//#include "d_initCity.h"
#include "neighbourhood.h"



namespace VIBe{


//class initCity;
//class vSys;
class dataLayer;

/** @brief cellular automata rule 
  *
  * administrates datalayer data and bdata access
  */
class DM_HELPER_DLL_EXPORT rule : public neighbourhood
{

private:
	void applySwitch_1(int layer);
	void applySwitch_2(int layer);

        //vSys *city_;
	dataLayer **layer_;
        //initCity *genesis_;
	int width_;
	int height_;
	int cellSize_;

public:
        rule(dataLayer **layer_);
        //rule (vSys *city_);
	~rule();

        void setRuleLayer(dataLayer **l) {layer_=l;}
        //void setInitCity(initCity *Genesis) {genesis_=Genesis;}
        //bool isInitCity() {if (genesis_) return 1; else return 0;}


	double sumLayer(int layer);

	//several rules
	//-------------------------//
		//game of life
		void rule_01();
		//game of life modified
		void rule_02();
		//apply topology agents
		void rule_03();		
		//gradient field rule
		void rule_04(int layer);
	//-------------------------//


	//clear topoStatus artefacts
	//-------------------------//		
		void r_topoStatus();
	//-------------------------//	


	//landuse rules
	//-------------------------//
		void r_initLandUse();
			void r_initToLandUse();
		void r_landUseChange01();
			void r_LandUseSmoothing();
	//-------------------------//

	//load the initial population if saved
	//-------------------------//
		void loadinitPop();
	//-------------------------//

	//population rules
	//-------------------------//
		void r_loadGrowthparameter();
		void r_popGrowthmodel01();
		void r_popGrowthmodel02();
		void r_popGrowthmodel03();
			double calcVal(int x, int y);
			double calcMeanVal(int x, int y);
			double calcGrowthFactor(int currentLandUse);
			double calcRoadAccessFactor(int x, int y, int currentLandUse);
			double calcOverCrowding(double val,int currentLandUse);
		void r_calcRoadPopConnect();

	//population parameter
	//-------------------------//
	double maxCPopDensity;
	double maxDCPopDensity;
	double maxOBPopDensity;

	double PopChangeLow;
	int popCUFRand;
	int popDCUFRand;
	int popAGRIRand;
	//-------------------------//
	//-------------------------//



	//industry rules
	//-------------------------//
		void r_indGrowthmodel01();
			void initIndToInd();
			void loadIndParameter();
	//-------------------------//

	//industry parameter
	//-------------------------//
		double PE_IndperCell;
	//-------------------------//

	//percentage of impervious area
	//-------------------------//
		void r_calcPercImpArea();
			void r_PIMPSmoothing();
	//-------------------------//


	//PE rules
	//-------------------------//
		void r_calcPopEquivalents();
			void r_sumPE();
			void r_calcPopToPE();
			void r_calcIndToPE();
			void r_calcComToPE();
	//-------------------------//

	//DWF rules
	//-------------------------//
		void r_loadDWFparameter();
		void r_calcDWF();
		void r_calcDWFpollutants();
	//-------------------------//	

	//DWF parameter
	//-------------------------//
		double Qh; //l/sEW residential and commercial DWF
		double Qf; //l/sEW parasite water DWF
		double Qi; //l/sha industrial water DWF
		int varMin;//(%)
		int varMax;//(%)
		int dVar; //(%)
	//-------------------------//




	//WDS rules
	//-------------------------//
		void r_calcWaterDemand();
			void r_sumWaterDemand();
			void r_calcPopToWD();
			void r_calcIndToWD();
			void r_calcComToWD();
			void r_calcAgriToWD();



};
};


#endif
