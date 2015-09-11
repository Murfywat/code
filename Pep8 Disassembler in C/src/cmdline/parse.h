#ifndef __PARSE_COMMAND_LINE__
#define __PARSE_COMMAND_LINE__

/* ************************************************************************* *
 * parse.h                                                                   *
 * -------                                                                   *
 *  Author:   Murphy Cooke                                                   *
 *  Purpose:  Header file for parse.c.                                       *
 * ************************************************************************* */

/* ************************************************************************* *
 * Function prototypes here. Note that variable names are often omitted.     *
 * ************************************************************************* */
int parse_command_line (int, char *[], const char **, const char **, int *);

void usage();
/* ************************************************************************* *
 * Externally defined variables here. An externally defined variable is a    *
 * global variable in parse.c that is made to be accessible to parts of the  *
 * program that include this header file. For instance, if you have a global *
 * variable defined as:                                                      *
 *                                                                           *
 *   bool some_global_boolean;                                               *
 *                                                                           *
 * In this file, you would put:                                              *
 *                                                                           *
 *   extern bool some_global_boolean;                                        *
 *                                                                           *
 * This comment should be deleted.                                           *
 * ************************************************************************* */
    extern bool symStatus;

#endif
