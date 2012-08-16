#ifndef CORINE_H
#define CORINE_H

namespace CORINE {
    enum CORINE
    {
        /*
            CORINE land cover nomenclature
            in extracts
    */
        //LEVEL 1
        //LEVEL 2
        //LEVEL 3
        //-------------------------//
        ArtificialSurfaces=0,				//0 1.
        UrbanFabric,				 //1 1.1
        ContUrbanFabric,		  //2 1.1.1
        DisContUrbanFabric,		  //3 1.1.2
        //IndustrialCommercialTrans,	 //1.2
        //IndustrialCommercial,	  //1.2.1
        RoadRailNetwork,		  //4 1.2.2
        //MineDumpConstruction,		 //1.3
        //ArtificialVegetated,		 //1.4
        //NonArtificialSurface,			//Control
        //-------------------------//

        //-------------------------//
        AgriculturalAreas,				//5 2.
        //ArableLand,					 //2.1
        //PermanentCrops,				 //2.2
        //Pastures,					 //2.3
        //HetAgriculture,				 //2.4
        //NonAgriculturalAreas,			//ControlValue
        //-------------------------//

        //-------------------------//
        ForestsSemiNatural,				//c 3.
        //Forests,					 //3.1
        //Shrub,						 //3.2
        //littleVeg,					 //3.3
        //NonForest,						//ControlValue
        //-------------------------//

        //-------------------------//
        //Wetlands,						//4.
        //InlandWetLand,				 //4.1
        //CoastalWetLand,				 //4.2
        //NoWetlands,						//ControlValue
        //-------------------------//

        //-------------------------//
        WaterBodies,					//7 5.
        InlandWaters,				 //8 5.1
        MarineWaters,				 //5.2
        NoWaterBodies,					//ControlValue
        //-------------------------//
        LandUseEnd

    };
}
#endif // CORINE_H
