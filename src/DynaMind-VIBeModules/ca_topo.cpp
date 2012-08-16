

#include "ca.h"


namespace VIBe{

/** @brief cellular automata do step topography
  *
  * \param layer_  data basis
  * \param pR rule pointer
  *
  */
void caTopo::doStep(dataLayer **layer_,rule &pR)
{
	pR.rule_04(topoElevation);
}

/** @brief cellular automata do gradient field
  *
  * \param layer_  data basis
  * \param pR rule pointer
  *
  */
void caTopo::gradField(dataLayer **layer_, rule &pR)
{
        doStep(layer_,pR);

}

/** @brief cellular automata run topography
  *
  *
  */
void caTopo::caRun()
{

}

}
