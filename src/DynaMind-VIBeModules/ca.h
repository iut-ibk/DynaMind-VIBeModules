#include "dmcompilersettings.h"

#ifndef __ca
#define __ca

#include <iostream>


//#include "vSys.h"
#include "dataLayer.h"
#include "rules.h"
#include "def.h"

namespace VIBe{

class rule;
class vSys;
class dataLayer;



/** @brief cellular automata basic class
  *
  */
class DM_HELPER_DLL_EXPORT ca
{



public:
	ca();
	ca(dataLayer **layer_);
        //ca(vSys *city);
        //vSys *city_;
	dataLayer **layer_;
	virtual void caRun();
	virtual void doStep(dataLayer **layer_,rule&);

//protected:
	long time;
	int height_;
	int width_;
	int cellSize_;


};

/** @brief population cellular automata class
  *
  */
/*class VIBE_HELPER_DLL_EXPORT caPop: public ca
{


public:
	caPop();
	caPop(vSys *city);

	void doStep();
	void caRun();
	void caBatchRun(bool isLastStep);
	void loadInitPop();

private:
	bool parameterLoaded;
	rule *popR_;

};*/

/** @brief industry cellular automata class
  *
  */
/*class VIBE_HELPER_DLL_EXPORT caInd: public ca
{

public:
	caInd(vSys *city);
        virtual ~caInd();
	void doStep();
	void caRun();

private:
	bool parameterLoaded;
	rule *indR_;

};*/


/** @brief road cellular automata class
  *
  */
/*class VIBE_HELPER_DLL_EXPORT caRoad: public ca
{
private:

	void doStep(rule *rule);
	void caRun();
	void evalRoadConnect();

public:
	caRoad(dataLayer **layer);

};*/



/** @brief topography cellular automata class
  *
  */
class DM_HELPER_DLL_EXPORT caTopo: public ca
{
public:
	void caRun();
	void doStep(dataLayer **layer_, rule &pR);
	void gradField(dataLayer **layer_, rule &pR);

};


/** @brief landuse cellular automata class
  *
  */
/*class VIBE_HELPER_DLL_EXPORT caLandUse: public ca
{
public:
	caLandUse(vSys *city);
	caLandUse();
        virtual ~caLandUse();
	void caRun();
	void dointitLandUseSmoothing();
	void doStep();

private:
	rule *landR_;
	double PE_IndperCell;
	double maxCPopDensity;
	double maxDCPopDensity;
	double maxOBPopDensity;



};*/


/** @brief agents cellular automata class
  *
  */
class DM_HELPER_DLL_EXPORT caAgents: public ca
{
public: 
	caAgents(vSys *city);
	void agentsRun();

private:
	rule *agentR_;

};

}


#endif
