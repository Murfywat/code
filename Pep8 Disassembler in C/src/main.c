/* ************************************************************************* *
 * main.c                                                                    *
 * ------                                                                    *
 *  Author:   Murphy Cooke                                                   *
 *  Purpose:  A disassembler and interpreter for Pep/8 System                *
 *  This code abides by the JMU Honor code                                   *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.  For documentation of standard C library           *
 * functions, see the list at:                                               *
 *   http://pubs.opengroup.org/onlinepubs/009695399/functions/contents.html  *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <stdbool.h>            /* bool types */
#include <stdint.h>             /* uint32_t, uint8_t, and similar types */
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "cmdline/parse.h"      /* command line parser */
#include "disasm/disasm.h"



/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */
    const char * filename;
    const char * symfilename;
    int mainmode = 0;
    mem_loc * firstAddress = NULL;
    const char * disasmOutput;
    bool symStatus = false;


/* ************************************************************************* *
 * main -- main entry point into the program.                                *
 *                                                                           *
 * Parameters                                                                *
 *   argc -- the number of command-line arguments                            *
 *   argv -- the array of command-line arguments (array of pointers to char) *
 *                                                                           *
 * Returns                                                                   *
 *   Program exit status. The standard is to return 0 for normal exit (no    *
 *   errors occurred). If something unusual happens, return some number      *
 *   other than 0, which can then indicate to the user what happened.        *
 *                                                                           *
 * Notes                                                                     *
 *   This function is similar to the "public static void Main" method used   *
 *   in Java classes.                                                        *
 * ************************************************************************* */

int
main (int argc, char * argv[])
{
    int return_code = 0;

    return_code = parse_command_line (argc, argv, &filename, &symfilename,  &mainmode);

    if(filename != NULL)
    {
        return_code = disassemble_data(filename, &firstAddress, symfilename);

        format_disasm_output(&disasmOutput, &firstAddress);
        free_program();
    }
    else
    {
        fprintf(stderr,"No file passed");
    }

    if(return_code == 1)
    {
        fprintf(stderr, "No file name passed");
    }
    free(firstAddress);
    return return_code;
}
