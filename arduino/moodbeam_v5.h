//-----------------------------------
//	moodbeam v0.2
//	https://github.com/ttosi/moodbeam
//	01/17/2015
//
//	Tony S. Tosi
//	ttosi519@gmail.com
//	irc & github: ttosi
//	twitter: tonytosi
//	http://blog.turningdigital.com
//------------------------------------

#define STATUS_LED 4
#define NUM_PIXELS 1
#define PIXEL_CONTROL_PIN 0

#define SHOW_COLOR 0xF0
#define TWO_COLOR 0xF1
#define ALTERNATE_COLORS 0xF2
#define FLASH 0xF3
#define RAINBOW 0xF4
#define FADE 0xF5

struct RGB
{
	byte r;
	byte g;
	byte b;
};