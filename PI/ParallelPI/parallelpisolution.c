#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h> 

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
	double  PI;
	char	processor_name[MPI_MAX_PROCESSOR_NAME];
	char	(*all_proc_names)[MPI_MAX_PROCESSOR_NAME];
	int		numprocs;
	int		MyID;
	int		namelen;
	int		proc = 0;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&MyID);
	MPI_Get_processor_name(processor_name,&namelen);

	all_proc_names = (char(*)[128]) malloc(numprocs * MPI_MAX_PROCESSOR_NAME);

	MPI_Gather(processor_name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, all_proc_names, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);
	if (MyID == 0)
	{
		for (proc=0; proc < numprocs; ++proc)
			printf("Process %d on %s\n", proc, all_proc_names[proc]);
	}

	IntervalLength = 0.0;
	if (MyID == 0)
	{ 
		if (argc > 1)
		{
			NumIntervals = atoi(argv[1]);
		}
		else
		{
			NumIntervals = 100000;
		}

		printf("NumIntervals = %i\n", NumIntervals);
	}

	MPI_Bcast(&NumIntervals, 1, MPI_INT, 0, MPI_COMM_WORLD);   /* send number of intervals to all procs */

	if (NumIntervals != 0)
	{
		//approximate the value of PI
		IntervalWidth   = 1.0 / (double) NumIntervals;           

		for (Interval = MyID + 1; Interval <= NumIntervals; Interval += numprocs){
			IntrvlMidPoint = IntervalWidth * ((double)Interval - 0.5);
			IntervalLength += (4.0 / (1.0 + IntrvlMidPoint*IntrvlMidPoint));
		}

		MyPI = IntervalWidth * IntervalLength;

		MPI_Reduce(&MyPI, &PI, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		//report approximation
		if (MyID == 0){
		printf("PI is approximately %.16f, Error is %.16f\n",
				PI, fabs(PI - ReferencePI));
		}
	}

	MPI_Finalize();
        return 0;
}
