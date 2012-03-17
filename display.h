/*
 * display.h - this module creates the different screens
 *             for my LCD project
 *
 */

/*
 * display_timeout
 * runs a function for some time (seconds)
 *
 */

void
display_timeout (
    void   (*function)(void),
    int      runtime,
    int      clear);

/*
 * display_shortsum
 * show a short summary of all areas
 *
 */

void
display_shortsum (void);

/*
 * display_load
 * shows the system load screen on the LCD
 *
 */

void
display_load (void);

/*
 * display_mem
 * show system memory summary on LCD
 *
 */

void
display_mem (void);


/* 
 * display_welcome
 * shows a startingg screen
 *
 */

void
display_welcome (void);

