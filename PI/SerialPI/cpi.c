#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	int		NumIntervals	= 0;	//num intervals in the domain [0,1] of F(x)= 4 / (1 + x*x)
	double	IntervalWidth	= 0.0;	//width of intervals
	double  IntervalLength  = 0.0;	//length of intervals
	double	IntrvlMidPoint	= 0.0;	//x mid point of interval
	int		Interval		= 0;	//loop counter
	int		done			= 0;	//flag
	double	MyPI			= 0.0;	//storage for PI approximation results
	double	ReferencePI		= 3.141592653589793238462643; //ref value of PI for comparison
	
	IntervalLength = 0.0;
	if (argc > 1)
	{
		NumIntervals = atoi(argv[1]);
	}
	else
	{
		NumIntervals = 100000;
	}

	printf("NumIntervals = %i\n", NumIntervals);

	if (NumIntervals == 0)
		done = 1;   //exit if number of intervals = 0  
	else
	{
		IntervalWidth   = 1.0 / (double) NumIntervals;           
		for (Interval = 1; Interval <= NumIntervals; Interval++)
		{
		  IntrvlMidPoint = IntervalWidth * ((double)Interval - 0.5);
		  IntervalLength += (4.0 / (1.0 + IntrvlMidPoint*IntrvlMidPoint));
	   }
	   MyPI = IntervalWidth * IntervalLength;

	   printf("PI is approximately %.16f, Error is %.16f\n",
			   MyPI, fabs(MyPI - ReferencePI));
	}
	return 0;

}
