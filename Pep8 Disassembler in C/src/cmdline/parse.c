/* ************************************************************************* *
 * parse.c                                                                   *
 * -------                                                                   *
 *  Author:   Murphy Cooke                                                   *
 *  Purpose:  This code is used to parse the command-line arguments for      *
 *            CS 350 projects 0 and 1.                                       *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.                                                    *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <unistd.h>             /* gives us getopt() */
#include <ctype.h>              /* gives us isprint() */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"              /* prototypes for exported functions */



/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */
    int iflag = 0;
    int sflag = 0;

/* ************************************************************************* *
 * parse_command_line -- parses the command-line arguments using getopt()    *
 *                                                                           *
 * Parameters                                                                *
 *   argc -- the number of command-line arguments                            *
 *   argv -- the array of command-line arguments (array of pointers to char) *
 *                                                                           *
 * Returns                                                                   *
 *   Parsing success status. If the command-line arguments are successfully  *
 *   parsed with no errors, we return the value 0. If any errors occur (such *
 *   as passing an invalid flag or passing a file that does not exist), we   *
 *   return 1 back to the calling function and stop processing               *
 *                                                                           *
 * Notes                                                                     *
 *   This function is designed to be extensible for multiple command-line    *
 *   options. In C, though, you can only return a single value. We get       *
 *   around this restriction by using call-by-reference parameters. For      *
 *   instance, if we want to set a boolean value, the parameter list would   *
 *   be modified as:                                                         *
 *                                                                           *
 *     parse_command_line (..., bool * boolval)                              *
 *                                                                           *
 *   Then, the function would be called from somewhere else like:            *
 *                                                                           *
 *     bool my_bool_value;                                                   *
 *     int return_code;                                                      *
 *     return_code = parse_command_line (..., &my_bool_value);               *
 * ************************************************************************* */

int
parse_command_line (int argc, char * argv[],const char **filename,
                    const char ** symfile, int * mainmode)
{
    int returncode = 0;
    char * cvalue = NULL;
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "s:i")) != -1)
    {
        switch (c)
        {
            case 'i':
                iflag = 1;
                break;

            case 's':
                sflag = 1;
                cvalue = optarg;
                *symfile = cvalue;
                symStatus = true;
                break;

            case '?':
                if (optopt == 's')
                {
                    fprintf(stderr, "Hexdump: Failed to pass file name to -s flag\n");
                    usage();
                    exit(1);

                }
                else
                {
                    fprintf(stderr, "Hexdump: Unknown Option Char -%c\n", optopt);
                    abort();

                }


            default:
                abort ();
        }
    }

    *filename = argv[optind];
    if (*filename == NULL)
    {
        return 1;
    }

    if(sflag == 0)
    {
        *symfile = NULL;
    }
    return returncode;
}

void
usage()
{
    printf("usage: hexdump [-v] [-f filename]\n");

}
