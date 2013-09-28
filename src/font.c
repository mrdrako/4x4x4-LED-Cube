// ############################################
// Extension of
// 4x4x4 LED Cube project
// By Christian Moen 2008
// chr@syntaks.org
// License: GPL
//
//
// Mike Durakovich
// mrdrako87@yahoo.com
// license: GPL

// font borrowed and slightly modified from "Micro Font: 4x5"
// by Rob Chapman
//
// ############################################

// Text library for 4x5 vertically scrolling font

// making this a x6 array means that each horiz font line
// already conforms to the existing way that lines are drawn
// in frames.c, without the need to bitshift even/odd lines.
// Each value corresponds with the pixels on a horiz. line of
// the character. The final line is always 0 so there's a natural
// break when scrolling words
#include <string.h>


// need to redeclare this here, alternative is to pass it in the function call
#define cubesize 4

void scroll_char_x(const char, int, int);
void scroll_string_x(const char*, int, int);
void scroll_string_z(const char*, int, int);
void scroll_string_y(const char*, int, int);
int check_level(int);

//95 printable ascii characters counting lowercase, upper, nums, and all punct
//
const unsigned char font[95][6] PROGMEM =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },        // space
	{ 0x02, 0x02, 0x02, 0x00, 0x02, 0x00 },        //   !
	{ 0x00, 0x05, 0x00, 0x00, 0x00, 0x00 },        //   "
	{ 0x06, 0x0f, 0x06, 0x0f, 0x06, 0x00 },        //   #
	{ 0x07, 0x0a, 0x06, 0x05, 0x0e, 0x00 },        //   $
	{ 0x09, 0x01, 0x02, 0x04, 0x09, 0x00 },        //   %
	{ 0x03, 0x05, 0x04, 0x0a, 0x0d, 0x00 },        //   &
	{ 0x02, 0x04, 0x00, 0x00, 0x00, 0x00 },        //   '
	{ 0x02, 0x04, 0x04, 0x04, 0x02, 0x00 },        //   {
	{ 0x04, 0x02, 0x02, 0x02, 0x04, 0x00 },        //   }
	{ 0x00, 0x05, 0x0f, 0x05, 0x00, 0x00 },        //   *
	{ 0x00, 0x02, 0x07, 0x02, 0x00, 0x00 },        //   +
	{ 0x00, 0x00, 0x00, 0x02, 0x04, 0x00 },        //   ,
	{ 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00 },        //   -
	{ 0x00, 0x00, 0x00, 0x00, 0x08, 0x00 },        //   .
	{ 0x01, 0x01, 0x02, 0x04, 0x08, 0x00 },        //   /
	{ 0x06, 0x0b, 0x09, 0x0d, 0x06, 0x00 },        //   0
	{ 0x02, 0x06, 0x02, 0x02, 0x02, 0x00 },        //   1
	{ 0x0e, 0x01, 0x06, 0x08, 0x0f, 0x00 },        //   2
	{ 0x0e, 0x01, 0x06, 0x01, 0x0e, 0x00 },        //   3
	{ 0x02, 0x06, 0x0a, 0x0f, 0x02, 0x00 },        //   4
	{ 0x0f, 0x08, 0x0e, 0x01, 0x0e, 0x00 },        //   5
	{ 0x06, 0x08, 0x0e, 0x09, 0x06, 0x00 },        //   6
	{ 0x0f, 0x01, 0x02, 0x04, 0x08, 0x00 },        //   7
	{ 0x06, 0x09, 0x06, 0x09, 0x06, 0x00 },        //   8
	{ 0x06, 0x09, 0x07, 0x01, 0x06, 0x00 },        //   9
	{ 0x00, 0x04, 0x00, 0x04, 0x00, 0x00 },        //   :
	{ 0x00, 0x04, 0x00, 0x04, 0x08, 0x00 },        //   ;
	{ 0x02, 0x04, 0x08, 0x04, 0x02, 0x00 },        //   <
	{ 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00 },        //   =
	{ 0x08, 0x04, 0x02, 0x04, 0x08, 0x00 },        //   >
	{ 0x06, 0x09, 0x02, 0x00, 0x02, 0x00 },        //   ?
	{ 0x06, 0x0d, 0x0b, 0x08, 0x07, 0x00 },        //   @
	{ 0x06, 0x09, 0x0f, 0x09, 0x09, 0x00 },        //   A
	{ 0x0e, 0x09, 0x0e, 0x09, 0x0e, 0x00 },        //   B
	{ 0x06, 0x09, 0x08, 0x09, 0x06, 0x00 },        //   C
	{ 0x0e, 0x09, 0x09, 0x09, 0x0e, 0x00 },        //   D
	{ 0x0f, 0x08, 0x0e, 0x08, 0x0f, 0x00 },        //   E
	{ 0x0f, 0x08, 0x0e, 0x08, 0x08, 0x00 },        //   F
	{ 0x06, 0x08, 0x0b, 0x09, 0x06, 0x00 },        //   G
	{ 0x09, 0x09, 0x0f, 0x09, 0x09, 0x00 },        //   H
	{ 0x0e, 0x04, 0x04, 0x04, 0x0e, 0x00 },        //   I
	{ 0x01, 0x01, 0x01, 0x09, 0x06, 0x00 },        //   J
	{ 0x09, 0x0a, 0x0c, 0x0a, 0x09, 0x00 },        //   K
	{ 0x08, 0x08, 0x08, 0x08, 0x0f, 0x00 },        //   L
	{ 0x09, 0x0f, 0x0f, 0x09, 0x09, 0x00 },        //   M
	{ 0x09, 0x0d, 0x0e, 0x0b, 0x09, 0x00 },        //   N
	{ 0x06, 0x09, 0x09, 0x09, 0x06, 0x00 },        //   O
	{ 0x0e, 0x09, 0x0e, 0x08, 0x08, 0x00 },        //   P
	{ 0x06, 0x90, 0x09, 0x0b, 0x07, 0x00 },        //   Q
	{ 0x0e, 0x09, 0x0e, 0x0a, 0x09, 0x00 },        //   R
	{ 0x07, 0x08, 0x06, 0x01, 0x0e, 0x00 },        //   S
	{ 0x0e, 0x04, 0x04, 0x04, 0x04, 0x00 },        //   T
	{ 0x09, 0x09, 0x09, 0x09, 0x06, 0x00 },        //   U
	{ 0x0a, 0x0a, 0x0a, 0x0a, 0x04, 0x00 },        //   V
	{ 0x09, 0x09, 0x0f, 0x0f, 0x09, 0x00 },        //   W
	{ 0x09, 0x09, 0x06, 0x09, 0x09, 0x00 },        //   X
	{ 0x0a, 0x0a, 0x04, 0x04, 0x04, 0x00 },        //   Y
	{ 0x0f, 0x02, 0x04, 0x08, 0x0f, 0x00 },        //   Z
	{ 0x06, 0x04, 0x04, 0x04, 0x06, 0x00 },        //   [
	{ 0x08, 0x08, 0x04, 0x02, 0x01, 0x00 },        /*   \    */
	{ 0x06, 0x02, 0x02, 0x02, 0x06, 0x00 },        //   ]
	{ 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00 },        //   ^
	{ 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00 },        //   _
	{ 0x04, 0x02, 0x00, 0x00, 0x00, 0x00 },        //   `
	{ 0x00, 0x05, 0x0b, 0x0b, 0x05, 0x00 },        //   a
	{ 0x08, 0x08, 0x0e, 0x09, 0x0e, 0x00 },        //   b
	{ 0x00, 0x04, 0x0a, 0x08, 0x06, 0x00 },        //   c
	{ 0x01, 0x01, 0x07, 0x09, 0x07, 0x00 },        //   d
	{ 0x04, 0x0a, 0x0c, 0x08, 0x06, 0x00 },        //   e
	{ 0x06, 0x04, 0x0e, 0x04, 0x04, 0x00 },        //   f
	{ 0x07, 0x09, 0x07, 0x01, 0x07, 0x00 },        //   g
	{ 0x08, 0x08, 0x0e, 0x09, 0x09, 0x00 },        //   h
	{ 0x00, 0x04, 0x00, 0x04, 0x04, 0x00 },        //   i
	{ 0x01, 0x00, 0x01, 0x01, 0x06, 0x00 },        //   j
	{ 0x08, 0x0a, 0x0c, 0x0a, 0x09, 0x00 },        //   k
	{ 0x0c, 0x04, 0x04, 0x04, 0x0e, 0x00 },        //   l
	{ 0x00, 0x09, 0x0f, 0x0f, 0x09, 0x00 },        //   m
	{ 0x0e, 0x09, 0x09, 0x09, 0x09, 0x00 },        //   n
	{ 0x00, 0x06, 0x09, 0x09, 0x06, 0x00 },        //   o
	{ 0x00, 0x0e, 0x09, 0x0e, 0x08, 0x00 },        //   p
	{ 0x00, 0x06, 0x09, 0x07, 0x01, 0x00 },        //   q
	{ 0x00, 0x0a, 0x0d, 0x08, 0x08, 0x00 },        //   r
	{ 0x00, 0x07, 0x04, 0x02, 0x0e, 0x00 },        //   s
	{ 0x04, 0x0e, 0x04, 0x04, 0x03, 0x00 },        //   t
	{ 0x00, 0x09, 0x09, 0x09, 0x06, 0x00 },        //   u
	{ 0x00, 0x0a, 0x0a, 0x0a, 0x04, 0x00 },        //   v
	{ 0x00, 0x09, 0x0f, 0x0f, 0x06, 0x00 },        //   w
	{ 0x00, 0x09, 0x06, 0x06, 0x09, 0x00 },        //   x
	{ 0x00, 0x09, 0x07, 0x01, 0x06, 0x00 },        //   y
	{ 0x00, 0x0f, 0x02, 0x04, 0x0f, 0x00 },        //   z
	{ 0x06, 0x04, 0x0c, 0x04, 0x06, 0x00 },        //   {
	{ 0x04, 0x04, 0x00, 0x04, 0x04, 0x00 },        //   |
	{ 0x0c, 0x04, 0x06, 0x04, 0x0c, 0x00 },        //   }
	{ 0x00, 0x05, 0x0a, 0x00, 0x00, 0x00 },        //   ~

};

int check_level(int level)
{
    if (level < 0 || level > 3)
        return 0;
    else
        return 1;
}

//scrolls a character along the x axis
void scroll_char_x(const char mychar, int delay, int level)
{
    int fontchar;
    fontchar = (int)mychar - 32;
    
    if (!check_level(level))
        level = 0;
    
    //default to a space if outside the range of the array
    if (fontchar < 0 || fontchar > 94)
        fontchar = 0;

    cube[level][0] = pgm_read_byte(&font[fontchar][0]);
    delay_ms(delay);
    cube[level][1] = pgm_read_byte(&font[fontchar][0]);
    cube[level][0] = pgm_read_byte(&font[fontchar][1]);
    delay_ms(delay);
    cube[level][2] = pgm_read_byte(&font[fontchar][0]);
    cube[level][1] = pgm_read_byte(&font[fontchar][1]);
    cube[level][0] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][0]);
    cube[level][2] = pgm_read_byte(&font[fontchar][1]);
    cube[level][1] = pgm_read_byte(&font[fontchar][2]);
    cube[level][0] = pgm_read_byte(&font[fontchar][3]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][1]);
    cube[level][2] = pgm_read_byte(&font[fontchar][2]);
    cube[level][1] = pgm_read_byte(&font[fontchar][3]);
    cube[level][0] = pgm_read_byte(&font[fontchar][4]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][2]);
    cube[level][2] = pgm_read_byte(&font[fontchar][3]);
    cube[level][1] = pgm_read_byte(&font[fontchar][4]);
    cube[level][0] = pgm_read_byte(&font[fontchar][5]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][3]);
    cube[level][2] = pgm_read_byte(&font[fontchar][4]);
    cube[level][1] = pgm_read_byte(&font[fontchar][5]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][4]);
    cube[level][2] = pgm_read_byte(&font[fontchar][5]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][5]);
    delay_ms(delay);

}

// this will scroll a text string along the x axis
// remember to add an extra space at the front or the back if it will repeat
// or else the string will jam together
//
void scroll_string_x(const char* mystring, int delay, int level)
{
    int size;
    int y;
    size = strlen(mystring);
    
    if (!check_level(level))
        level = 0;
    
    //use array
    clear_x_plane(level);
    for (y = 0; y < size; y++)
    {
        cube[level][3] = cube[level][2];
        cube[level][2] = cube[level][1];
        cube[level][1] = cube[level][0];
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        delay_ms(delay);
        cube[level][3] = cube[level][2];
        cube[level][2] = cube[level][1];
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        delay_ms(delay);
        cube[level][3] = cube[level][2];
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        delay_ms(delay);
        cube[level][3] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][3]);
        delay_ms(delay);
        cube[level][3] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][3]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][4]);
        delay_ms(delay);
        cube[level][3] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][3]);
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][4]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][5]);
        delay_ms(delay);
    }

}


// this will scroll a text string along the z axis
// remember to add an extra space at the front or the back if it will repeat
// or else the string will jam together
//
void scroll_string_z(const char* mystring, int delay, int level)
{
    int size;
    int y;
    size = strlen(mystring);
    
    if (!check_level(level))
        level = 0;
    
    //use array
    clear_z_plane(level);
    for (y = 0; y < size; y++)
    {
        cube[3][level] = cube[2][level];
        cube[2][level] = cube[1][level];
        cube[1][level] = cube[0][level];
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        delay_ms(delay);
        cube[3][level] = cube[2][level];
        cube[2][level] = cube[1][level];
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        delay_ms(delay);
        cube[3][level] = cube[2][level];
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        delay_ms(delay);
        cube[3][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][3]);
        delay_ms(delay);
        cube[3][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][3]);
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][4]);
        delay_ms(delay);
        cube[3][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][3]);
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][4]);
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][5]);
        delay_ms(delay);
    }

}

// this will scroll a text string along the y axis
// remember to add an extra space at the front or the back if it will repeat
// or else the string will jam together
//
void scroll_string_y(const char* mystring, int delay, int level)
{

    int size;
    int y;
    size = strlen(mystring);
    
    if (!check_level(level))
        level = 0;
    
    //use array
    clear_y_plane(level);
    for (y = 0; y < size; y++)
    {
        set_y_line_pattern(3, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][3]));  //scroll previous letter if exists
        set_y_line_pattern(2, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][4]));
        set_y_line_pattern(1, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][5]));
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][4]));
        set_y_line_pattern(2, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][5]));
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));  
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));  
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][5]));
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));  
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));  
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2]));  
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));  
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));  
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2]));  
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][3]));  
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));  
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2]));  
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][3]));  
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][4]));  
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2]));  
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][3]));  
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][4]));  
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][5]));
        delay_ms(delay);
    }

}
