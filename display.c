/*
 * display.c - this module creates the different screens
 *             for my LCD project
 *
 */

#include <stdio.h>
#include <time.h>
#include "display.h"
#include "lcd.h"
#include "stat.h"

/*
 * display_timeout
 * runs a function for some time (seconds)
 * funtion is a: void func (void), type of function.
 */

void
display_timeout (
    void   (*function)(void),
    int      runtime,
    int      clear)
{
    time_t   start_time;
    time_t   cur_time;

    start_time = time (NULL);
    cur_time = start_time;

    if (clear != 0)
    {
	lcd_on (0);
	function ();
	lcd_on (LCD_ON);
    }

    while (cur_time < start_time + runtime)
    {
	cur_time = time (NULL);
	function ();
    }
}

/*
 * display_shortsum
 * show a short summary of all areas
 *
 */

void
display_shortsum (void)
{
    int      users;
    double   load_i, load_f;
    double   load, dumb;
    char     line1[41], line2[41];
    char     loadbar[11] = "", membar[11] = "";
    int      tot,used,free,shared,buff,cache; 
    double   mem_free;
    long     uptime;
    long     up_d, up_h, up_m, up_s;  /* utptime (days, hours, minutes, secs)  */
    long     errs, cols;

    if (stat_getmem (&tot, &used, &free, &shared, &buff, &cache) == -1)
	return;

    mem_free = (free + buff + cache) / (double) tot;

    users = stat_getusers ();
    if (users == -1)
	return;

    if (stat_getavgload (&load, &dumb, &dumb) == -1)
	return;

    load_f = load - (int) load;
    load_i = (int) load;

    uptime = stat_getuptime ();
    if (uptime == -1)
	return;

    up_d = (long) (uptime / (24 * 3600));
    up_h = (long) (uptime / 3600) - (up_d * 24);
    up_m = (long) (uptime / 60) - (up_d * 24 * 60) - (up_h * 60);
    up_s = (long) (uptime - (up_d * 24) - (up_h * 3600) - (up_m * 60));

    if (stat_getneterrs (&errs, &cols) == -1)
	return;

    lcd_makebar (load_f, loadbar, 10);
    lcd_makebar (mem_free, membar, 10);

    /*               1234567890123456789012345678901234567890 */
    sprintf (line1, "Memfree %10s U[%02d] NE[%2d] NC[%2d] ", membar, users, errs, cols);
    sprintf (line2, "Load  %1d %10s Up%4dd %02dh %02dm %02ds ", (int) load_i, loadbar, up_d, up_h, up_m, up_s);

    lcd_home ();
    lcd_print (line1);
    lcd_pos (1, 2);
    lcd_print (line2);
}

/*
 * display_load
 * shows cpu load and users on
 *
 */

void
display_load (void)
{
    char     line1[256];
    char     line2[256];
    double   load;
    double   crap1,crap2;
    int      users;
    double   user,nice,system,idle;

    if (stat_getavgload (&load, &crap1, &crap2) == -1)
	return;

    users = stat_getusers ();
    if (users == -1)
	return;

    if (stat_getcpu (&user, &nice, &system, &idle) == -1)
	return;

    /*               1234567890123456789012345678901234567890  */
    sprintf (line1, "-CPU-    User %4.1f%% Sys %4.1f%% Idle %4.1f%%", user + nice, system, idle);
    sprintf (line2, "         Load %4.2lf  Users on system %-2d  ", load, users);

    lcd_home ();
    lcd_print (line1);
    lcd_pos (1, 2);
    lcd_print (line2);
}


/*
 * display_mem
 * show system memory summary on LCD
 *
 */

void
display_mem (void)
{
    char   line1[256];
    char   line2[256];
    int    tot,used,free,shared,buff,cache; 

    if (stat_getmem (&tot, &used, &free, &shared, &buff, &cache) == -1)
	return;

    /* convert to percent */
    used = 100 * (used / (double) tot);
    free = 100 * (free / (double) tot);
    shared = 100 * (shared / (double) tot);
    buff = 100 * (buff / (double) tot);
    cache = 100 * (cache / (double) tot);

    /* convert to megabyte */
    tot = tot / 1024 / 1024;

    /*               1234567890123456789012345678901234567890  */
    sprintf (line1, "-MEM-      Total%3dMB Used%3d%% Free%3d%% ", tot, used, free);
    sprintf (line2, "           Shared%3d%% Buff%3d%% Cache%3d%%", shared, buff, cache);

    lcd_home ();
    lcd_print (line1);
    lcd_pos (1, 2);
    lcd_print (line2);
}

/* 
 * display_welcome
 * shows a startingg screen
 *
 */

void
display_welcome (void)
{
    /*          1234567890123456789012345678901234567890    */
    lcd_print ("      ---== Welcome to lcdd ==---       ");
    lcd_pos (1 ,2);
    lcd_print ("  == Written by Jimmy Larsson 1997 ==   ");
    sleep (2);
    lcd_clear ();
}











