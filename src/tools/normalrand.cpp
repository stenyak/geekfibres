#include "normalrand.h"
#include <stdlib.h>
double normalrand(int lowest, int highest)
{
	double temp = rand()%((highest-lowest)*100000);
	temp/=100000.0;
	temp+=lowest;
	temp+= ((highest + lowest)/2.0-temp) * ((highest + lowest)/2.0-temp) * ((highest + lowest)/2.0-temp)/lowest/highest;
	return temp;
}

