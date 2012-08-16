#include "rules.h"


namespace VIBe{

/** @brief r_topoStatus()
  *
  *  clear artefacts derived from editor::paintOrga(...)
  *  only for topoStatus
  */
void rule::r_topoStatus()
{
	applySwitch_1(topoStatus);

	//current cell value
	int cellVal=0;

	int countVal=0;
	int bufferVal=0;

	//initialize nMoore3x3
	for (int i=0; i<9;i++) nMoore3x3[i]=NULL;

	for (int i=0; i<width_;i++)
	{
		for (int j=0; j<height_;j++)
		{
			nMoore3x3=Moore3x3(i,j,layer_[topoStatus]);
			//actual cell
			cellVal =  int (nMoore3x3[4]);

			//only change from 6 to 10 or from 0 to 6
			if (cellVal==0 || cellVal==6)
			{
				countVal=0;
				for (int k=0; k<9; k++)
				{
					//count neightbour cells which 
					//differ from current cell
					if ((int (nMoore3x3[k])) != cellVal)
					{
						//save differing neighbour value
						bufferVal=k;
						countVal+=1;
					}
				}
				//if more than 3 cells differ
				//and the changing value is not 0
				if (countVal>=4 && nMoore3x3[3]>1)
				{
					layer_[topoStatus]->setVal(i,j,
						nMoore3x3[3]);

					//eliminate artefacts in topoElevation
					if (nMoore3x3[3]==10 && countVal >5)
					{
						nMoore3x3=Moore3x3(i,j,layer_[topoElevation]);
						double newVal=0;
						for (int l=0;l<9;l++)
						{
							newVal+=nMoore3x3[l];
						}
						newVal/=9;
						layer_[topoElevation]->setVal(i,j,newVal);
					}
				}
				// layer switch, fill other datafield
				else
					layer_[topoStatus]->setVal(i,j,cellVal);

			}
			// layer switch, fill other datafield
			else
				layer_[topoStatus]->setVal(i,j,cellVal);
		}
	}
	applySwitch_2(topoStatus);
	for (int i=0; i<width_;i++)
	{
		for (int j=0; j<height_;j++)
		{
			layer_[topoStatus]->setVal(i,j,
				layer_[topoStatus]->getVal(i,j));
		}
	}
	layer_[topoStatus]->setSwitch(2);
};



};