#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
double countPI(double n)
{
	int i = 0;
	int count = 0;
	double pi = 0;
	double x = 0;
	double y = 0;
	srand((unsigned)time(NULL));
	while (i < n)
	{
		x = rand() / (double)(RAND_MAX);
		y = rand() / (double)(RAND_MAX);
		if ((x * x) + (y * y) < 1)
		{
			count++;
		}
		i++;
	}
	pi = 4 * (count / n);
	return pi;
}
 
int main(){
	double pi = 0;
	pi = countPI(2000);
	printf("%lf\n",pi);
	return 0;
}