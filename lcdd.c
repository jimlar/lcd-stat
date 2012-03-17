/*
 * lcdd.c - the main module for my lcd project
 *
 * Jimmy Larsson 1997
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "lcd.h"
#include "display.h"

int
main (void)
{
    FILE   *pid_file;
    int     error_num;
    int     i;
    pid_t   child_pid;

    if ((child_pid = fork ()) == -1) 
    {
	log_message ("LCDD: Couldn't create child process!");
	exit (1);
    }

    if (child_pid == 0)
    {
	error_num = lcd_init (0x378);
	if (error_num != 0)
	    log_message ("LCDD: Error opening port");
	
	display_welcome ();

	while (1)
	{
	    /* display_timeout (display_load, 4, 1);
	    display_timeout (display_mem, 4, 1); */

	    display_shortsum ();
     	}
	
	return 0;
    }
    else
    {
	printf ("initialized for 40x2 module, at PID %d\n", child_pid);
	return 0;
    }
}





