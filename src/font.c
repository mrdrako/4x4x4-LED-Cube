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
// can be stored in half of a byte, reducing memory space required.
// Bitshifting the upper byte down retrieves the pattern.
// The final line is always 0 so there's a natural
// break when scrolling words

// NOTE - much of this programming assumes a 4x4x4 cube. 
// Might need to be modified for your use.

#include <string.h>


// need to redeclare this here, alternative is to pass it in the function call
#define cubesize 4

void scroll_char_x(const char, int, int);
void scroll_char_y(const char, int, int);
void scroll_char_z(const char, int, int);
void scroll_string_x(const char*, int, int);
void scroll_string_z(const char*, int, int);
void scroll_string_y(const char*, int, int);
int check_level(int);

//95 printable ascii characters counting lowercase, upper, nums, and all punct
//
const unsigned char font[95][3] PROGMEM =
{
	{ 0x00, 0x00, 0x00 },        // space
	{ 0x22, 0x20, 0x20 },        //   !
	{ 0x05, 0x00, 0x00 },        //   "
	{ 0x6f, 0x6f, 0x60 },        //   #
	{ 0x7a, 0x65, 0xe0 },        //   $
	{ 0x91, 0x24, 0x90 },        //   %
	{ 0x35, 0x4a, 0xd0 },        //   &
	{ 0x24, 0x00, 0x00 },        //   '
	{ 0x24, 0x44, 0x20 },        //   {
	{ 0x42, 0x22, 0x40 },        //   }
	{ 0x05, 0xf5, 0x00 },        //   *
	{ 0x02, 0x72, 0x00 },        //   +
	{ 0x00, 0x02, 0x40 },        //   ,
	{ 0x00, 0xf0, 0x00 },        //   -
	{ 0x00, 0x00, 0x80 },        //   .
	{ 0x11, 0x24, 0x80 },        //   /
	{ 0x6b, 0x9d, 0x60 },        //   0
	{ 0x26, 0x22, 0x20 },        //   1
	{ 0xe1, 0x68, 0xf0 },        //   2
	{ 0xe1, 0x61, 0xe0 },        //   3
	{ 0x26, 0xaf, 0x20 },        //   4
	{ 0xf8, 0xe1, 0xe0 },        //   5
	{ 0x68, 0xe9, 0x60 },        //   6
	{ 0xf1, 0x24, 0x80 },        //   7
	{ 0x69, 0x69, 0x60 },        //   8
	{ 0x69, 0x71, 0x60 },        //   9
	{ 0x04, 0x04, 0x00 },        //   :
	{ 0x04, 0x04, 0x80 },        //   ;
	{ 0x24, 0x84, 0x20 },        //   <
	{ 0x0f, 0x0f, 0x00 },        //   =
	{ 0x84, 0x24, 0x80 },        //   >
	{ 0x69, 0x20, 0x20 },        //   ?
	{ 0x6d, 0xb8, 0x70 },        //   @
	{ 0x69, 0xf9, 0x90 },        //   A
	{ 0xe9, 0xe9, 0xe0 },        //   B
	{ 0x69, 0x89, 0x60 },        //   C
	{ 0xe9, 0x99, 0xe0 },        //   D
	{ 0xf8, 0xe8, 0xf0 },        //   E
	{ 0xf8, 0xe8, 0x80 },        //   F
	{ 0x68, 0xb9, 0x60 },        //   G
	{ 0x99, 0xf9, 0x90 },        //   H
	{ 0xe4, 0x44, 0xe0 },        //   I
	{ 0x11, 0x19, 0x60 },        //   J
	{ 0x9a, 0xca, 0x90 },        //   K
	{ 0x88, 0x88, 0xf0 },        //   L
	{ 0x9f, 0xf9, 0x90 },        //   M
	{ 0x9d, 0xeb, 0x90 },        //   N
	{ 0x69, 0x99, 0x60 },        //   O
	{ 0xe9, 0xe8, 0x80 },        //   P
	{ 0x60, 0x9b, 0x70 },        //   Q
	{ 0xe9, 0xea, 0x90 },        //   R
	{ 0x78, 0x61, 0xe0 },        //   S
	{ 0xe4, 0x44, 0x40 },        //   T
	{ 0x99, 0x99, 0x60 },        //   U
	{ 0xaa, 0xaa, 0x40 },        //   V
	{ 0x99, 0xff, 0x90 },        //   W
	{ 0x99, 0x69, 0x90 },        //   X
	{ 0xaa, 0x44, 0x40 },        //   Y
	{ 0xf2, 0x48, 0xf0 },        //   Z
	{ 0x64, 0x44, 0x60 },        //   [
	{ 0x88, 0x42, 0x10 },        /*   \    */
	{ 0x62, 0x22, 0x60 },        //   ]
	{ 0x4a, 0x00, 0x00 },        //   ^
	{ 0x00, 0x00, 0xf0 },        //   _
	{ 0x42, 0x00, 0x00 },        //   `
	{ 0x05, 0xbb, 0x50 },        //   a
	{ 0x88, 0xe9, 0xe0 },        //   b
	{ 0x04, 0xa8, 0x60 },        //   c
	{ 0x11, 0x79, 0x70 },        //   d
	{ 0x4a, 0xc8, 0x60 },        //   e
	{ 0x64, 0xe4, 0x40 },        //   f
	{ 0x79, 0x71, 0x70 },        //   g
	{ 0x88, 0xe9, 0x90 },        //   h
	{ 0x04, 0x04, 0x40 },        //   i
	{ 0x10, 0x11, 0x60 },        //   j
	{ 0x8a, 0xca, 0x90 },        //   k
	{ 0xc4, 0x44, 0xe0 },        //   l
	{ 0x09, 0xff, 0x90 },        //   m
	{ 0xe9, 0x99, 0x90 },        //   n
	{ 0x06, 0x99, 0x60 },        //   o
	{ 0x0e, 0x9e, 0x80 },        //   p
	{ 0x06, 0x97, 0x10 },        //   q
	{ 0x0a, 0xd8, 0x80 },        //   r
	{ 0x07, 0x42, 0xe0 },        //   s
	{ 0x4e, 0x44, 0x30 },        //   t
	{ 0x09, 0x99, 0x60 },        //   u
	{ 0x0a, 0xaa, 0x40 },        //   v
	{ 0x09, 0xff, 0x60 },        //   w
	{ 0x09, 0x66, 0x90 },        //   x
	{ 0x09, 0x71, 0x60 },        //   y
	{ 0x0f, 0x24, 0xf0 },        //   z
	{ 0x64, 0xc4, 0x60 },        //   {
	{ 0x44, 0x04, 0x40 },        //   |
	{ 0xc4, 0x64, 0xc0 },        //   }
	{ 0x05, 0xa0, 0x00 },        //   ~

};

int check_level(int level)
{
    if (level < 0 || level > 3)
        return 0;
    else
        return 1;
}

//scrolls a single character along the X axis, completely
//
void scroll_char_x(const char mychar, int delay, int level)
{
    int fontchar;
    fontchar = (int)mychar - 32;
    
    if (!check_level(level))
        level = 0;
    
    //default to a space if outside the range of the array
    if (fontchar < 0 || fontchar > 94)
        fontchar = 0;

    cube[level][0] = pgm_read_byte(&font[fontchar][0])>>4;
    delay_ms(delay);
    cube[level][1] = pgm_read_byte(&font[fontchar][0])>>4;
    cube[level][0] = pgm_read_byte(&font[fontchar][0]);
    delay_ms(delay);
    cube[level][2] = pgm_read_byte(&font[fontchar][0])>>4;
    cube[level][1] = pgm_read_byte(&font[fontchar][0]);
    cube[level][0] = pgm_read_byte(&font[fontchar][1])>>4;
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][0])>>4;
    cube[level][2] = pgm_read_byte(&font[fontchar][0]);
    cube[level][1] = pgm_read_byte(&font[fontchar][1])>>4;
    cube[level][0] = pgm_read_byte(&font[fontchar][1]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][0]);
    cube[level][2] = pgm_read_byte(&font[fontchar][1])>>4;
    cube[level][1] = pgm_read_byte(&font[fontchar][1]);
    cube[level][0] = pgm_read_byte(&font[fontchar][2])>>4;
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][1])>>4;
    cube[level][2] = pgm_read_byte(&font[fontchar][1]);
    cube[level][1] = pgm_read_byte(&font[fontchar][2])>>4;
    cube[level][0] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][1]);
    cube[level][2] = pgm_read_byte(&font[fontchar][2])>>4;
    cube[level][1] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][2])>>4;
    cube[level][2] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[level][3] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);

}


//scrolls a single character along the Y axis, completely
//
void scroll_char_y(const char mychar, int delay, int level)
{
    int fontchar;
    fontchar = (int)mychar - 32;
    
    if (!check_level(level))
        level = 0;
    
    //default to a space if outside the range of the array
    if (fontchar < 0 || fontchar > 94)
        fontchar = 0;

    set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[fontchar][0])>>4);
    delay_ms(delay);                                                   
    set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[fontchar][0])>>4);
    set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[fontchar][0]));
    delay_ms(delay);                                                   
    set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[fontchar][0])>>4);
    set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[fontchar][0]));
    set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[fontchar][1])>>4);
    delay_ms(delay);                                                   
    set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[fontchar][0])>>4);
    set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[fontchar][0]));
    set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[fontchar][1])>>4);
    set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[fontchar][1]));
    delay_ms(delay);                                                   
    set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[fontchar][0]));
    set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[fontchar][1])>>4);
    set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[fontchar][1]));
    set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[fontchar][2])>>4);
    delay_ms(delay);                                                   
    set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[fontchar][1])>>4);
    set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[fontchar][1]));
    set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[fontchar][2])>>4);
    set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[fontchar][2]));
    delay_ms(delay);
    set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[fontchar][1]));
    set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[fontchar][2])>>4);
    set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[fontchar][2]));
    delay_ms(delay);
    set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[fontchar][2])>>4);
    set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[fontchar][2]));
    delay_ms(delay);
    set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[fontchar][2]));
    delay_ms(delay);


}


//scrolls a single character along the Z axis, completely
//
void scroll_char_z(const char mychar, int delay, int level)
{
    int fontchar;
    fontchar = (int)mychar - 32;
    
    if (!check_level(level))
        level = 0;
    
    //default to a space if outside the range of the array
    if (fontchar < 0 || fontchar > 94)
        fontchar = 0;

    cube[0][level] = pgm_read_byte(&font[fontchar][0])>>4;
    delay_ms(delay);
    cube[1][level] = pgm_read_byte(&font[fontchar][0])>>4;
    cube[0][level] = pgm_read_byte(&font[fontchar][0]);
    delay_ms(delay);
    cube[2][level] = pgm_read_byte(&font[fontchar][0])>>4;
    cube[1][level] = pgm_read_byte(&font[fontchar][0]);
    cube[0][level] = pgm_read_byte(&font[fontchar][1])>>4;
    delay_ms(delay);
    cube[3][level] = pgm_read_byte(&font[fontchar][0])>>4;
    cube[2][level] = pgm_read_byte(&font[fontchar][0]);
    cube[1][level] = pgm_read_byte(&font[fontchar][1])>>4;
    cube[0][level] = pgm_read_byte(&font[fontchar][1]);
    delay_ms(delay);
    cube[3][level] = pgm_read_byte(&font[fontchar][0]);
    cube[2][level] = pgm_read_byte(&font[fontchar][1])>>4;
    cube[1][level] = pgm_read_byte(&font[fontchar][1]);
    cube[0][level] = pgm_read_byte(&font[fontchar][2])>>4;
    delay_ms(delay);
    cube[3][level] = pgm_read_byte(&font[fontchar][1])>>4;
    cube[2][level] = pgm_read_byte(&font[fontchar][1]);
    cube[1][level] = pgm_read_byte(&font[fontchar][2])>>4;
    cube[0][level] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[3][level] = pgm_read_byte(&font[fontchar][1]);
    cube[2][level] = pgm_read_byte(&font[fontchar][2])>>4;
    cube[1][level] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[3][level] = pgm_read_byte(&font[fontchar][2])>>4;
    cube[2][level] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);
    cube[3][level] = pgm_read_byte(&font[fontchar][2]);
    delay_ms(delay);

}


// this will scroll a text string along the X axis
// Remember to add an extra space at the back of the string
// This prevents repeating strings from jamming into each other
// and also stops a disappearing last letter if not repeating
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
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        delay_ms(delay);
        cube[level][3] = cube[level][2];
        cube[level][2] = cube[level][1];
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        delay_ms(delay);
        cube[level][3] = cube[level][2];
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        delay_ms(delay);
        cube[level][3] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        delay_ms(delay);
        cube[level][3] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][2])>>4;
        delay_ms(delay);
        cube[level][3] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        cube[level][2] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);
        cube[level][1] = pgm_read_byte(&font[(((int)mystring[y])-32)][2])>>4;
        cube[level][0] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        delay_ms(delay);
    }
    clear_x_plane(level);

}


// this will scroll a text string along the Z axis
// Remember to add an extra space at the back of the string
// This prevents repeating strings from jamming into each other
// and also stops a disappearing last letter if not repeating
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
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        delay_ms(delay);                                                     
        cube[3][level] = cube[2][level];                                     
        cube[2][level] = cube[1][level];                                     
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);   
        delay_ms(delay);                                                     
        cube[3][level] = cube[2][level];                                     
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);   
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        delay_ms(delay);                                                     
        cube[3][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4;
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);   
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);   
        delay_ms(delay);                                                     
        cube[3][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][0]);   
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);   
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2])>>4;
        delay_ms(delay);                                                     
        cube[3][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4;
        cube[2][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][1]);   
        cube[1][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2])>>4;
        cube[0][level] = pgm_read_byte(&font[(((int)mystring[y])-32)][2]);
        delay_ms(delay);
    }
    clear_z_plane(level);

}

// this will scroll a text string along the y axis
// Remember to add an extra space at the back of the string
// This prevents repeating strings from jamming into each other
// and also stops a disappearing last letter if not repeating
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
        set_y_line_pattern(3, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][1]));  //scroll previous letter if exists
        set_y_line_pattern(2, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][2])>>4);
        set_y_line_pattern(1, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][2]));
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4);
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][2])>>4);
        set_y_line_pattern(2, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][2]));
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4);
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, y==0?0:pgm_read_byte(&font[(((int)mystring[y-1])-32)][5]));
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4);
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4);
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0])>>4);
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4);
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][0]));
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4);
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2])>>4);
        delay_ms(delay);
        set_y_line_pattern(3, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1])>>4);
        set_y_line_pattern(2, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][1]));
        set_y_line_pattern(1, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2])>>4);
        set_y_line_pattern(0, level, cubesize, pgm_read_byte(&font[(((int)mystring[y])-32)][2]));
        delay_ms(delay);
    }
    clear_y_plane(level);

}
