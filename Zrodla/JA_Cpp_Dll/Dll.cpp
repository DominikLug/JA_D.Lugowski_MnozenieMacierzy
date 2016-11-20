#include "Dll.h"
#include <iostream>

void MyClass::Foo(double **tabA, double **tabB, double **tabC, int startX, int stopX, int maxX, int maxY, int maxZ)
{
	double *tempA, *tempB, *tempC;
	for (int i = startX; i < stopX; i++)
	{
		tempA = tabA[i];
		tempC = tabC[i];
		for (int j = 0; j < maxZ; j++)
		{
			for (int k = 0; k < maxY; k++) 
			{
				tempB = tabB[k];
				tempC[j] += tempA[k] * tempB[j];
			}
		}
	}

}