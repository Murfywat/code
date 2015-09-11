#ifndef __CS450_MANDELBROT__
#define __CS450_MANDELBROT__

struct thread_data{
	int startRow;
	int endRow;
	int numRows;
	int threadNum;
	double startX;
	double startY;
	double xStep;
	double yStep;
};
void calculate_mandelbrot (double, double, double, double);

void * calc_mandel_thread(void *);

extern bool nFlag;
extern bool rFlag;
extern bool pFlag;
extern bool oFlag;
extern int nNum;
#endif
