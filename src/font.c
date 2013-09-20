// ############################################
// 
// 4x4x4 LED Cube project
// By Christian Moen 2008
// chr@syntaks.org
// License: GPL
//
// Additional material
// by Mike Durakovich
// mrdrako87@yahoo.com
// license: GPL
//
// ############################################

// Text library for 4x5 vertically scrolling font

// making this a x5 array means that each horiz font line
// already conforms to the existing way that lines are drawn
// in frames, without the need to bitshift even/odd lines.
// Each value corresponds with the pixels on a horiz. line of
// the character.

const unsigned char font[2][5] PROGMEM =       //94 counting lowercase, upper, nums, and all punct
{
	{ 0x06, 0x09, 0x0f, 0x09, 0x09 },        // A
	{ 0x0e, 0x09, 0x0e, 0x09, 0x0e },        // B
};

