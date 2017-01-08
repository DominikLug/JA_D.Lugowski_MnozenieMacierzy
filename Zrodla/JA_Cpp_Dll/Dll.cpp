#include "Dll.h"
#include <iostream>

void MyClass::Foo(double ***tab , int range[2], int max[3])
{
	double *tempA, *tempB, *tempC;
	for (int i = range[0]; i < range[1]; i++)
	{
		tempA = tab[0][i];
		tempC = tab[2][i];
		for (int j = 0; j < max[2]; j++)
		{
			tempB = tab[1][j];
			for (int k = 0; k < max[1]; k++)
			{
				
				tempC[j] += tempA[k] * tempB[k];
			}
		}
	}



	/*for (int i = range[0]; i < range[1]; i++)
	{
		tempA = tab[0][i];
		tempC = tab[2][i];
		for (int j = 0; j < max[2]; j++)
		{
			for (int k = 0; k < max[1]; k++) 
			{
				tempB = tab[1][k];
				tempC[j] += tempA[k] * tempB [j];
			}
		}
	}*/

}