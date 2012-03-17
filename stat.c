/*
 * stat.c - the statistics generating module for my LCD project
 * Jimmy Larsson 1997
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include "stat.h"
#include "log.h"

/*
 * stat_getneterrs
 * returns network errors and collisions on all devices
 *
 */

int
stat_getneterrs (
    long   *errors,
    long   *collisions)
{
    FILE   *netfile;
    char    inbuf[256];
    char    indev[256] = "";
    long    errs_i, errs_o, cols;
    long    junk;
    char   *status = "";

    *errors = 0;
    *collisions = 0;

    netfile = fopen ("/proc/net/dev", "r");
    if (netfile == NULL)
    {
	log_message ("GETNETERRS: error opening /proc/net/dev");
	return -1;
    }

    while (status != NULL)
    {
	status = fgets (inbuf, 256, netfile);
	sscanf (inbuf, "%s %d %d %d %d %d %d %d %d %d %d %d", indev, &junk, &errs_i, &junk, &junk, &junk, &junk, &errs_o, &junk, &junk, &cols, &junk);
	if (indev[strlen (indev) - 1] == ':' && status != NULL)
	{
	    *errors = *errors + errs_i + errs_o;
	    *collisions = *collisions + cols;
	}
    }

    fclose (netfile);
    return 0;
}


/*
 * stat_getuptime
 * returns uptime in seconds
 *
 */

long
stat_getuptime (void)
{
    FILE    *uptime_file;
    double   ut;            /* uptime    */
    double   it;            /* idle time */

    uptime_file = fopen ("/proc/uptime", "r");
    if (uptime_file == NULL)
    {
	log_message ("GETUPTIME: error opening /proc/uptime");
	return -1;
    }

    fscanf (uptime_file, "%lf %lf", &ut, &it);

    fclose (uptime_file);

    return (long) ut;
}


/*
 * stat_getavgload
 * returns average load for 1min, 5min, 15min (in arguments)
 *
 */

int
stat_getavgload (
    double   *avg1,
    double   *avg5,
    double   *avg15)
{
    FILE     *statfile;
    char      inbuf[256];
   
    statfile = fopen ("/proc/loadavg", "r");
    if (statfile == NULL)
    {
	log_message ("GETAVGLOAD: error opening /proc/loadavg");
	return -1;
    }

    fgets (inbuf, 256, statfile);
    sscanf (inbuf, "%lf %lf %lf", avg1, avg5, avg15);

    fclose (statfile);
    return 0;
}

/*
 * stat_getmem
 * get memory statistics (return values in the arguments)
 * 
 */

int
stat_getmem (
    int    *total, 
    int    *used,
    int    *free,
    int    *shared,
    int    *buffered,
    int    *cached)
{
    FILE   *memfile;
    char    line[256];
    char    type[256] = "";
    
    memfile = fopen ("/proc/meminfo", "r");
    if (memfile == NULL)
    {
	log_message ("GETMEM: error opening /proc/meminfo");
	return -1;
    }
    
    fgets (line, 256, memfile);    /* flush the topline */
    fgets (line, 256, memfile);
    sscanf (line, "%s %d %d %d %d %d %d", type, total, used, free, shared, buffered, cached);

    fclose (memfile);
    return 0;
}

/*
 * stat_getusers
 * returns the number of users on system
 * 
 */

int 
stat_getusers (void)
{
    FILE         *utmpfile;
    struct utmp   ut;
    int           num = 0;
    
    utmpfile = fopen (UTMP_FILE, "rb");
    if (utmpfile == NULL)
    {
	log_message ("GETUSERS: error opening UTMP_FILE");
	return -1;
    }
 
    do 
    {
	fread (&ut, sizeof(struct utmp), 1, utmpfile);

	if ((ut.ut_type == USER_PROCESS) && (ut.ut_user[0] != 0))
	    num++;

    } while (!feof (utmpfile));

    fclose (utmpfile);
    return (num);
}

/*
 * stat_getcpu
 * returns cputime spent in user,nice,system,idle modes
 * (returns in arguments, returns percentage)
 *
 */

int
stat_getcpu (
    double   *user,
    double   *nice,
    double   *system,
    double   *idle)
{
    FILE     *cpufile;
    
    char      inbuf[256] = "";
    char      desq[256] = "";
    long      total;
    
    cpufile = fopen ("/proc/stat", "r");
    if (cpufile == NULL)
    {
	log_message ("GETCPU: error opening /proc/stat");
	return -1;
    }
    
    while (strcmp (desq, "cpu") != 0)
    {
	if (fgets (inbuf, 256, cpufile) == NULL)
	{
	    log_message ("GETCPU: error reading /proc/stat");
	    fclose (cpufile);
	    return -1;
	}

	sscanf (inbuf, "%s %lf %lf %lf %lf", desq, user, nice, system, idle);
    }
    
    total = *user + *nice + *system + *idle;
    
    *user = 100 * (*user / total);
    *nice = 100 * (*nice / total);
    *system = 100 * (*system / total);
    *idle = 100 * (*idle / total);
    
    fclose (cpufile);
    return 0;
}
