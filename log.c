/*
 * log.c - the module to handle error loging for my LCD project
 * Jimmy Larsson 1997
 *
 */

#include <stdio.h>
#include "log.h"

/* 
 * log_message
 * puts an entry into the log
 *
 */

void
log_message (
    char   *err_msg)
{
    fprintf (stderr, "LCDD: %s\n", err_msg);
}
