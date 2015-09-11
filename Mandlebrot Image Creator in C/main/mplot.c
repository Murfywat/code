/* mplot.c
 * -------
 *  Author:   Murphy Cooke
 *  Purpose:  This file contains the main entry point into the Mandelbrot
 *            calculation.
 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>             /* gives us atof() */
#include <stdbool.h>            /* bool types */
#include <stdint.h>             /* uint32_t and similar types */
#include <unistd.h>             /* gives us getopt() */
#include <ctype.h>              /* gives us isprint */
#include <string.h>             /* gives us strlen() */


#include "mplot.h"              /* WIDTH and HEIGHT constants */
#include "../graph/mandel.h"    /* prototypes for mandelbrot */
#include "../graph/tga.h"       /* prototypes for image creation */

#define DEFAULT_NAME "cs450-mplot.tga"

/* Local function declarations */
void show_usage (void);
bool parse_cmd (int, char *[], char **, double *, double *, double *, double *, int *);


/* Declare global variables here */
uint8_t IMAGE[HEIGHT][WIDTH][3];  /* stores the data */
int nNum = -1;
bool rFlag = false;
bool nFlag = false;
bool pFlag = false;
bool oFlag = false;

/* main entry point */
int
main(int argc, char *argv[])
{
  /* default is to use (-2.00,-1.05) to (0.80,1.05) */
  double xstart = -2.00, xend = 0.80, ystart = -1.05, yend = 1.05;
  char * output = NULL;             /* name of output file */

  bool success = parse_cmd (argc, argv, &output, &xstart,
                            &xend, &ystart, &yend, &nNum);

  /* if we couldn't parse the command line, abort */
  if (success == false)
    {
      fprintf (stderr, "Failed to parse command line\n");
      show_usage ();
      return 1;
    }

  /* make sure a TGA file name was passed */
  if (output == NULL)
    {
      output = DEFAULT_NAME;
      printf ("No file name passed; using \"%s\"\n", output);
    }
  else if ( (strlen(output) < 5) ||
            (strcmp (output + (strlen(output)-4), ".tga")) )
    {
      fprintf (stderr, "Illegal file name: %s (only .tga files supported)\n",
               output);
      return 1;
    }

  /* sanity check on the coordinates */
  if ( (xstart >= xend) || (ystart >= yend) )
    {
      fprintf (stderr, "Illegal coordinates: (%f, %f) to (%f, %f)\n",
               xstart, ystart, xend, yend);
      return 2;
    }

  printf ("Building image file \"%s\"\n", output);
  printf ("Coordinates are (%0.2f,%0.2f) to (%0.2f,%0.2f)\n",
          xstart, ystart, xend, yend);

  calculate_mandelbrot (xstart, ystart, xend, yend);

  write_file (output);

  return 0;
}

/* show_usage
 * ----------
 *  Arguments: none
 *  Returns:   none
 *  Purpose:   prints out the help usage message
 */

void
show_usage (void)
{
  printf ("Usage: mplot [options]\n");
  printf ("Options:\n");
  printf ("  -f F.tga      Specify the .tga file to use as output\n");
  printf ("                (default name is " DEFAULT_NAME ")\n");
  printf ("  -h            Print this message and exit\n");
  printf ("  -x x          Set the lower-left x-coordinate for the image\n");
  printf ("  -y y          Set the lower-left y-coordinate for the image\n");
  printf ("  -X x          Set the upper-right x-coordinate for the image\n");
  printf ("  -Y y          Set the upper-right y-coordinate for the image\n");
  printf ("  -p N          Run with Pthreads, using N threads in parallel\n");
  printf ("  -p all        Run with Pthreads, using the maximum available CPUs\n");
  printf ("  -p row        Run with Pthreads, using one thread per row\n");
}

/* parse_command_line
 * ------------------
 *  Arguments: int argc - number of command-line arguments
 *             char * argv[] - array of argument strings
 *             char ** output - name of output file to produce
 *             double * xstart - x value of lower-left corner (by reference)
 *             double * ystart - y value of lower-left corner (by reference)
 *             double * xend - x value of upper-right corner (by reference)
 *             double * yend - y value of upper-right corner (by reference)
 *  Returns:   bool - true is successful
 *  Purpose:   It uses getopt() to parse the command line.
 */
bool
parse_cmd (int argc, char *argv[], char ** output, double * xstart,
           double * xend, double * ystart, double * yend, int * nValue)
{
  int option;

  /* option is the ASCII value of the character argument passed. See
   * http://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt for
   * more documentation on using getopt(). */
  while ((option = getopt (argc, argv, "hporf:x:y:X:Y:n:")) != -1)
    {
      switch (option)
        {
          case 'h':         /* -h */
            show_usage ();
            exit (0);
            break;

          case 'r':
        	if(nFlag == true)
        	{
        		fprintf(stderr, "n flag already passed");
        	}
        	else
        	{
        		rFlag = true;
        		printf("rFlag test\n");
        	}
        	break;
          case 'n':
        	if(rFlag == true)
        	{
        		fprintf(stderr, "r flag already passed");
        	}
        	else
        	{
        		nFlag = true;
        		*nValue = atoi(optarg);
        	}
        	break;
          case 'p':
        	  if(oFlag == true)
        	  {
        		  fprintf(stderr, "o flag already passed");
        	  }
        	  else
        	  {
        		  pFlag = true;
        	  }
        	  break;
          case 'o':
        	  if(pFlag == true)
        	  {
        		  fprintf(stderr, "p flag already passed");
        	  }
        	  else
        	  {
        		  oFlag = true;
        	  }
        	  break;
          case 'x':         /* -x -1.05 */
            *xstart = (double)(atof (optarg));
            break;
          case 'y':         /* -y -0.2 */
            *ystart = (double)(atof (optarg));
            break;
          case 'X':         /* -X 1.05 */
            *xend = (double)(atof (optarg));
            break;
          case 'Y':         /* -Y 0.2 */
            *yend = (double)(atof (optarg));
            break;
          case 'f':         /* -f output.tga */
            *output = optarg;
            break;
          default:
            /* valid flag, but the argument was missing */
            if (optopt == 'f' || optopt == 'W' || optopt == 'H' ||
                optopt == 'x' || optopt == 'y' || optopt == 'X' ||
                optopt == 'Y')
              {
                fprintf (stderr, "Option -%c requires an argument\n", optopt);
                return false;
              }
            else /* otherwise, it was an invalid flag */
              {
                fprintf (stderr, "Unknown option -%c\n", optopt);
                return false;
              }
            break;
        }
    }
  return true;
}

