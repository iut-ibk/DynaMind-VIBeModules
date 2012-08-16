#include "dmcompilersettings.h"

#ifndef __neighbourhood
#define __neighbourhood


#include "dataLayer.h"

namespace VIBe{

static const double Pi = 3.14159265358979323846264338327950288419717;

/** @brief defines neighbourhoods
  *
  */
class DM_HELPER_DLL_EXPORT neighbourhood
{
	private:

	protected:
//cell neighbourhoods
//-------------------------//
		double* Moore3x3(int x, int y, dataLayer *pL);
		///double nMoore3x3[9];

		void Moore5x5(int x, int y);
		double* Neumann1(int x, int y, dataLayer *pL);
		double* Neumann3(int x, int y, dataLayer *pL);
		void iNeumann(int x, int y);
//-------------------------//

//cell neighbourhoodcells
//-------------------------//
		void setMoore3x3(int x, int y, int h, int layer, double val);



//agents
//-------------------------//
		void Agent_01(/*dataLayer **layer_*/);
		void Agent_02(/*dataLayer **layer_*/);
		void Agent_03(/*dataLayer **layer_*/);
		void Agent_04(/*dataLayer **layer_*/);
		void Agent_05(/*dataLayer **layer_*/);
		void Agent_06(/*dataLayer **layer_*/);

		void a_LEA_routing();
		void a_LEA_routingModified();
//-------------------------//
	public:
		neighbourhood();
		dataLayer **layer__;
		double calcCell();
		void setLayer(dataLayer **layer_) {layer__=layer_;};


		double *nMoore3x3;

};
};

#endif //__neighbourhood
