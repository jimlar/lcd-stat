/*
 * lcd.c - the driver module for my LCD package
 * By: Jimmy Larsson 
 * 
 * 970328 - v0.0001 - Project started
 *
 */

/* intruction defines */
#define LCD_CURS_ON 2
#define LCD_ON      4
#define LCD_BLINK   1

#define LCD_SCROLL_DISPLAY  8
#define LCD_SCROLL_RIGHT    4

#define LCD_SCROLLING  1
#define LCD_INCR       2

#define LCD_8BIT       16
#define LCD_16DUTY     8
#define LCD_5x11FONT   4

/*
 * lcd_put
 * puts data/instruction byte to LCD
 *
 */

void
lcd_put (
    int   data,        /* data byte */
    int   rs);         /* register select (0: instructio, 1: data) */

/*
 * lcd_clear
 * clears and homes the cursor
 *
 */

void
lcd_clear (void);

/*
 * lcd_home
 * homes the cursor
 *
 */

void
lcd_home (void);

/*
 * lcd_on
 * turns display,cursor,blink on/off. Use LCD_CURS_ON, LCD_ON, LCD_BLINK
 *
 */

void
lcd_on (
    int   val);

/*
 * lcd_scroll
 * scrolls display or cursor. Use: LCD_SCROLL_DISPLAY, LCD_SCROLL_RIGHT
 *
 */

void
lcd_scroll (
    int   val);

/*
 * lcd_function_set
 * sets up the display functions
 * Use: LCD_8BIT, LCD_16DUTY, LCD_5x11FONT
 *
 */

void
lcd_function_set (
    int   val);

/*
 * lcd_addr_set
 * set the char-ram addr. for the next read/write
 *
 */

void
lcd_addr_set (
    int   addr);

/*
 * lcd_entry
 * sets lcd's entry mode. Use: LCD_SCROLLING, LCD_INCR
 *
 */

void
lcd_entry (
    int   val);

/*
 * lcd_init
 * initializes lcd communication and clears it.
 * returns 0 - success, otherwise the errno from ioperm.
 *
 */

int
lcd_init (
    int   lpt_addr);

/*
 * lcd_putchar
 * prints a charater on the LCD
 *
 */

void
lcd_putchar (
    int   chr);

/*
 * lcd_print
 * prints a string to the LCD
 *
 */

void
lcd_print (
    char   *str);

/*
 * lcd_pos
 * positions cursor at line and column, starting from 1,1
 * 
 */

void
lcd_pos (
    int   col,
    int   line);

/*
 * lcd_print2
 * prints two strings, one on the first line, one on the second
 *
 */

void
lcd_print2 (
    char   *line1,
    char   *line2);


/*
 * lcd_makebar
 * make a bar graph of a floating point value between 0 and 1, in terms of LCD chars.
 * Takes a char vetor and length plus the value as arguments.
 *
 */

void
lcd_makebar (
    double   value,
    char    *bar,
    int      barlen);

