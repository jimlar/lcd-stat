/*
 * stat.h - the statistics generating module for my LCD project
 * Jimmy Larsson 1997
 * 
 */

/*
 * stat_getneterrs
 * returns network errors and collisions on all devices
 * returns -1 on error
 */

int
stat_getneterrs (
    long   *errors,
    long   *collisions);

/*
 * stat_getuptime
 * returns uptime in seconds
 *
 */

long
stat_getuptime (void);

/*
 * stat_getavgload
 * returns system avg. load (processes in the run queue)
 * for 1minute, 5min, 15min (returns values in arguments)
 * returns -1 on error
 */

int
stat_getavgload (
    double   *avg1,
    double   *avg5,
    double   *avg15);


/*
 * stat_getmem
 * get memory statistics (returns values in the arguments)
 * returns -1 on error
 */

int
stat_getmem (
    int    *total, 
    int    *used,
    int    *free,
    int    *shared,
    int    *buffered,
    int    *cached);


/*
 * stat_getusers
 * returns the number of users on system
 * returns -1 on error 
 */

int 
stat_getusers (void);

/*
 * stat_getcpu
 * returns cputime spent in user,nice,system,idle modes
 * (returns in arguments, returns percentage)
 * returns -1 on error
 */

int
stat_getcpu (
    double   *user,
    double   *nice,
    double   *system,
    double   *idle);
