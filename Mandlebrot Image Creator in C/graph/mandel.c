/* mandel.c
 * --------
 *  Author:   Murphy Cooke
 *  Purpose:  This file contains the code for calculating the Mandelbrot set.
 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>
#include <stdbool.h>            /* bool types */
#include <stdint.h>             /* uint32_t and similar types */
#include <inttypes.h>           /* PRI* format strings */
#include <pthread.h>
#include <omp.h>

#include "../main/mplot.h"      /* WIDTH and HEIGHT constants */
#include "mandel.h"             /* prototypes for mandelbrot */

#define NUM_ITERATIONS 10000

/* Local function declarations */
static uint32_t calc_pixel (double, double, uint8_t *, uint8_t *, uint8_t *);

/* Declare global variables here */

double currentY = 0.0;
double currentX = 0.0;

uint64_t total_iters = 0;
uint32_t iters = 0;
double x,y;
/* Keeps track of current Row value for multithreading purposes*/
int currentRow = 0;
/* Pointer for array of threads individual iterator numbers to be summed */
int * iterArray;

/* calculate_mandelbrot
 * --------------------
 *  Arguments: double xstart - the x-coord of the lower-left corner
 *             double ystart - the y-coord of the lower-left corner
 *             double xend - the x-coord of the upper-right corner
 *             double yend - the y-coord of the upper-right corner
 *  Returns:   none
 *  Purpose:   iterates through all rows and pixels to determine if points are
 *             in the Mandelbrot set; sets RGB values in a global array
 */
void
calculate_mandelbrot (double xstart, double ystart, double xend, double yend)
{
  /* pixelize the image */
  double xstep = (xend - xstart)/WIDTH;
  double ystep = (yend - ystart)/HEIGHT;
  uint8_t r,g,b;
  uint32_t i, j;
  int numThreads = 0;
  int rowsPerThread = 0;



  /* start in the lower-left corner */
  x = xstart;
  y = ystart;
  currentY = y; /* Current Y for Threads*/

  if(pFlag == true && oFlag == false)
  {
	  printf ("PThreads multithreading will be used\n");
  }
  else if (pFlag == false && oFlag == true)
  {
	  printf ("OpenMP multithreading will be used\n");
  }
  else
  {
	  printf ("No multithreading will be used\n");
  }

  if (pFlag == false && oFlag == false)
  {
	  /* For each row */
	  for (i=0; i<HEIGHT; i++)
	  {
		/* For each pixel in the row */
		for (j=0; j<WIDTH; j++)
		{
		  /* determine if the (x,y) coordinates are in the set */
		  iters = calc_pixel (x, y, &r, &g, &b);
		  total_iters += iters;

		  /* color the pixel accordingly */
		  IMAGE[i][j][0] = b;
		  IMAGE[i][j][1] = g;
		  IMAGE[i][j][2] = r;

		  /* move to the next pixel */
		  x += xstep;
		}

		/* move to the next row */
		y += ystep;
		x = xstart;
	  }
  }
  else if(oFlag == true)
  {
	  if(nFlag == true)
	  {
		  numThreads = nNum;
	  }
	  else
	  {
		  if(rFlag == false)
		  {
			  numThreads = 10;
		  }
		  else
		  {
			  numThreads = HEIGHT;
		  }
	  }

	  omp_set_num_threads(numThreads);
	  rowsPerThread = HEIGHT/numThreads;
	  x = xstart;
	  y = ystart;
	  double yStarts[numThreads];
	  j = 0;
	  for(i = 0; i < HEIGHT; i++)
	  {
		  if(i % rowsPerThread == 0)
		  {
			  yStarts[j] = (double)i*ystep + ystart;
			  j++;
		  }
	  }

      #pragma omp parallel private (currentY)
	  /* For each Thread */
	  {
		  currentY = yStarts[omp_get_thread_num()];

          #pragma omp for reduction(+:total_iters) private (i, j, y, x, r, g, b)
		  for (i = 0; i < HEIGHT; i++)
		  {
			x = xstart;
			y = currentY;
			/* For each pixel in the row */
			for (j=0; j<WIDTH; j++)
			{
			  /* determine if the (x,y) coordinates are in the set */
			  iters = calc_pixel (x, y, &r, &g, &b);
			  total_iters += iters;

			  /* color the pixel accordingly */
			  IMAGE[i][j][0] = b;
			  IMAGE[i][j][1] = g;
			  IMAGE[i][j][2] = r;

			  /* move to the next pixel */
			  x = (double)j*xstep + xstart;
			}
			/* move to the next row */
			currentY = (double)i * ystep + ystart;
		  }
	  }

  }
  else
  {
	  if(pFlag == true)
	  {
		  pthread_attr_t attr;
		  int rc;
		  long t;
		  void * status;
		  pthread_attr_init(&attr);
		  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		  if(nFlag == true)
		  {
			  numThreads = nNum;
		  }
		  else
		  {
			  if(rFlag == true)
			  {
				  numThreads = HEIGHT;
			  }
			  else
			  {
				  /* Default Thread Amount */
				  numThreads = 10;
			  }
		  }
		  iterArray = malloc(numThreads * sizeof(int));
		  pthread_t thread[numThreads];
		  struct thread_data args[numThreads];

		  rowsPerThread = HEIGHT/numThreads;
		  for(t = 0; t < numThreads; t++)
		  {
			  /*Set initial thread_data struct values*/
			  if(t == (numThreads - 1) && (HEIGHT % numThreads != 0))
			  {
				  args[t].numRows = rowsPerThread + (HEIGHT%numThreads);
				  args[t].startRow = currentRow;
			  }
			  else
			  {
				  args[t].numRows = rowsPerThread;
				  args[t].startRow = currentRow;
			  }
			  args[t].xStep = xstep;
			  args[t].yStep = ystep;
			  args[t].startX = x;
			  args[t].startY = currentY;
			  args[t].threadNum = t;

			  /* Add Pixel Difference to Y value*/
			  for(i = 0; i < rowsPerThread; i++)
			  {
				  currentY += ystep;
			  }
			  currentRow += rowsPerThread;

			  rc = pthread_create(&thread[t], &attr, calc_mandel_thread, (void *)&args[t]);
			  if(rc)
			  {
				  printf("ERROR; return code from pthread_create is %d\n", rc);
				  exit(-1);
			  }
		  }

		  pthread_attr_destroy(&attr);

		  for(t = 0; t < numThreads; t++)
		  {
			  rc = pthread_join(thread[t], &status);
			  if(rc){
				  printf("ERROR; return code from pthread_join() is %d\n", rc);
				  exit(-1);
			  }
		  }
	  }
  }
  if(pFlag == true)
  {
	  total_iters = 0;
	  for(i = 0; i < numThreads; i++)
	  {
		  total_iters += iterArray[i];
	  }

  }
  printf ("Total iterations: %" PRIu64 "\n", total_iters);
}


void *
calc_mandel_thread(void * _args)
{

	struct thread_data * args = (struct thread_data *)_args;
	double tempX = args->startX;
	double tempY = args->startY;
	uint8_t r,g,b;
	int i, j;
	  /* For each row in the thread */
	  for (i=args->startRow; i< args->startRow + args->numRows; i++)
	  {
		/* For each pixel in the row */
		for (j=0; j<WIDTH; j++)
		{
		  /* determine if the (x,y) coordinates are in the set */
		  iters = calc_pixel (tempX, tempY, &r, &g, &b);
		  iterArray[args->threadNum] += iters;

		  /* color the pixel accordingly */
		  IMAGE[i][j][0] = b;
		  IMAGE[i][j][1] = g;
		  IMAGE[i][j][2] = r;

		  /* move to the next pixel */
		  tempX += args->xStep;
		}

		/* move to the next row */
		tempY += args->yStep;
		tempX = args->startX;
	  }
	pthread_exit(NULL);
}
/* calc_pixel
 * ----------
 *  Arguments: double x - the x-coordinate of the point to calculate
 *             double y - the y-coordinate of the point to calculate
 *             uint8_t * red - pointer to the red value to set
 *             uint8_t * green - pointer to the green value to set
 *             uint8_t * blue - pointer to the blue value to set
 *  Returns:   uint32_t - the number of iterations for this particular point
 *  Purpose:   determines whether or not (x,y) is a point in the set
 */

static uint32_t
calc_pixel (double x, double y, uint8_t * red, uint8_t * green, uint8_t * blue)
{
  /* We are looking at x+yi as a complex number and determining if it is in the
   * Mandelbrot set. If we can repeat the calculation below and not exceed a
   * threshold value after a fixed number of iterations, then it is in the set.
   * We use z = a+bi to refer to the complex number z, where a is the "real"
   * part and b is the "imaginary" part that gets multiplied by the square root
   * of -1 (the constant i). */
  double a = 0, b = 0;      /* z = a + bi */
  double newa, newb;        /* used for updating a and b */
  double color;             /* pixel color */
  bool inset = true;        /* is the point in the Mandelbrot set? */
  uint32_t num_iters = 0;   /* how many iterations do we make it through? */

  /* Iterate through the Mandelbrot calculation NUM_ITERATIONS times. If we ever
   * exceed the threshold, then break out of the loop; we know that point is not
   * in the set. */
  for (num_iters=0; num_iters < NUM_ITERATIONS; num_iters++)
    {
      /* The calculation is as follows:
       *   z^2 + (x+yi) = (a+bi)(a+bi) + (x+yi)
       *                = (a^2 -b^2 + x) + (2ab + y)i
       * As such, our update is:
       *   newa = a^2 - b^2 + x
       *   newb = 2ab + y
       */
        newa = (a*a)-(b*b) + x;   /* a^2 - b^2 + x */
        newb = 2*a*b + y;         /* 2ab + y */
        a = newa;
        b = newb;

        /* If a^2 + b^2 > 100, the number is not in the set */
        if(((a*a)+(b*b)) > 100)
          {
            inset = false;
            color = (double)num_iters;
            break;
          }
    }

  if (inset)
    {
      /* points in the set are a blue-ish color */
      *blue = 230;
      *green = 50;
      *red = 70;
    }
  else
    {
      /* points out of the set are green-ish, lighter as they get closer */
      if (color < 50) color += 50;
      else if (color < 100) color += 55;
      else if (color < 350) color += 80;
      *blue = color / NUM_ITERATIONS * 180;
      *green = color / NUM_ITERATIONS * 255;
      *red = color / NUM_ITERATIONS * 120;
    }
  return num_iters;
}

