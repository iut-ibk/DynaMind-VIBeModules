#include "ca.h"


namespace VIBe{

    /** @brief constructor cellular automata
  *
  *
  */
    ca::ca()
    {
        layer_=0;
        //city_=NULL;


    }

    /** @brief constructor cellular automata
  *
  *
  */
    /*ca::ca(dataLayer **layer)
{
        layer_=layer;
        city_=NULL;
};*/

    /** @brief constructor cellular automata
  *
  *
  */
    ca::ca(dataLayer **layer)
    {

        //city_=city;
        layer_=layer;
    }



    /** @brief cellular automata run
  *
  *
  */
    void ca::caRun()
    {
        std::cout<<"error, ca::caRun() - missing methode in subclass\n";
    }


    /** @brief cellular automata do step
  *
  *
  */
    void ca::doStep(dataLayer **layer_, rule &pR)
    {
        (void) layer_;
        (void) pR;
        std::cout<<"error, ca::doStep() - missing methode in subclass\n";
    }

}
