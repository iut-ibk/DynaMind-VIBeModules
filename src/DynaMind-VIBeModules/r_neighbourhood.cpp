//#include "rules.h"
#include "dataLayer.h"
#include "neighbourhood.h"
//#include <iostream>


namespace VIBe{

 /**
  * \brief  constructor neighbourhood
  *
  *
  */
neighbourhood::neighbourhood()
{

};

 /**
  * \brief  Moore3x3
  *
  *  evaluated moore neighbourhood of speciefied cell
  *  Neighbour definition:
  *		6 7 8
  *		3(4)5
  *		0 1 2
  *
  * \param	x, y   position
  * \param pLayer dataLayer to evaluate
  *
  * \return	 nMoore3x3 vector of neighbourhood 0-8
  *
  */
double* neighbourhood::Moore3x3(int x, int y,dataLayer *pLayer)
{
		
	int i=0;	
	for(int j=0;j<3;j++)
	{
		for(int k=0;k<3;k++)
		{
			i=j*3+k;
			{				
				nMoore3x3[i]=pLayer->neighbour(((x-1)+k),(y-1)+j);
				//Neighbour definition:
				//6 7 8
				//3(4)5
				//0 1 2
			}
		}	
	}
	return nMoore3x3;
};



 /**
  * \brief  Neumann1
  *
  *  evaluated Neumann neighbourhood of speciefied cell
  *  Neighbour definition:
  * 			3
  *			0	1	2
  *				4
  *
  * \param	x, y   position
  * \param pLayer dataLayer to evaluate
  *
  * \return	 nNeumann1 vector of neighbourhood 0-4
  *
  */
double* neighbourhood:: Neumann1(int x, int y, dataLayer *pL)
{
	double *nNeumann1=NULL;
	nNeumann1 = new double[5];

	nNeumann1[0]=pL->neighbour(x-1,y);
	nNeumann1[1]=pL->neighbour(x,y);
	nNeumann1[2]=pL->neighbour(x+1,y);
	nNeumann1[3]=pL->neighbour(x,y-1);
	nNeumann1[4]=pL->neighbour(x,y+1);


	/*
				3
			0	1	2
				4
	*/

	return nNeumann1;



};


 /**
  * \brief  Neumann3
  *
  *  evaluated extended Neumann neighbourhood of speciefied cell
  *  Neighbour definition:
  *				7
  *				8
  *				9
  *		0 1 2  3/10 4 5 6
  *				11
  *				12
  *				13
  *
  * \param	x, y   position
  * \param pLayer dataLayer to evaluate
  *
  * \return	 nNeumann3 vector of neighbourhood 0-13
  *
  */
double* neighbourhood:: Neumann3(int x, int y,dataLayer *pLayer)
{
	double *nNeumann3;
	nNeumann3 = new double[15];
	for (int h=0;h<15;h++)//13 neighbours + 2 element
	{		 
		nNeumann3[h]=0;	
	}
	int i=0;
	for(int j=0;j<7;j++)
	{
		nNeumann3[i]=pLayer->neighbour(x-3+j,y);
		i+=1;
	}
	for(int k=0;k<7;k++)
	{
		nNeumann3[i]=pLayer->neighbour(x,y-3+k);
		i+=1;
	}
		/*Neighbour definition:
				7
				8
				9
		0 1 2  3/10 4 5 6
				11
				12
				13
		
		*/


	return nNeumann3;

};


 /**
  * \brief  iNeumann
  *
  *  evaluated inverted Neumann neighbourhood of speciefied cell
  *
  * \param	x, y   position
  *
  */
void neighbourhood:: iNeumann(int x, int y)
{
	(void) x;
	(void) y;
};

 /**
  * \brief  calcCell
  *
  */
double neighbourhood:: calcCell()
{
	return 0;
};


 /**
  * \brief  setMoore3x3
  *
  *  manipulates cell in moore neighbourhood
  *
  * \param	x, y   position
  * \param  h	specified cell in moore3x3 neighbourhood 
  * \param layer layer to apply on 
  * \param val value to set
  *
  */
void neighbourhood::setMoore3x3(int x,int y,int h, int layer, double val)
{
	int xPosition=x;
	int yPosition=y;
	switch (h)
	{
		case 0:
			xPosition-=1;
			yPosition-=1;
			break;
		case 1:
			
			yPosition-=1;
			break;
		case 2:
			xPosition+=1;
			yPosition-=1;
			break;
		case 3:
			xPosition-=1;
			
			break;
		case 5:
			xPosition+=1;
			
			break;
		case 6:
			xPosition-=1;
			yPosition+=1;
			break;
		case 7:
			
			yPosition+=1;
			break;
		case 8:
			xPosition+=1;
			yPosition+=1;
			break;
	}

	layer__[layer]->setVal(xPosition,yPosition,val);



};

};