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

#include "moodbeam.h"
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

String cmd;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	NUM_PIXELS,
	PIXEL_CONTROL_PIN,
	NEO_GRB + NEO_KHZ800
);

void setup()
{
	delay(2000);
	Serial.begin(9600);

	pixels.begin();
	pixels.show();
	
	cmd.reserve(64);
}

void loop()
{
	if (Serial.available() > 0) 
	{
		char data = (char)Serial.read();
		cmd += data;

		if(data == 0x0A) {
			switch (cmd[0])
			{
				case (char)SHOW_COLOR:
					setColor(cmd[1], cmd[2], cmd[3], -1);
					break;
				case (char)SET_BRIGHTNESS:
					setBrightness(cmd[1]);
					break;
				case (char)TWO_COLOR:
					setColor(cmd[1], cmd[2], cmd[3], 0);
					setColor(cmd[4], cmd[5], cmd[6], 1);
					break;
				case (char)ALTERNATE_COLORS:
					break;
				case (char)FLASH_COLOR:
					break;
				case (char)SHOW_RAINBOW:
					showRainbow(cmd[1]);
					break;
			}
			
			cmd = "";
		}
	}

	if(cmd[0] == SHOW_RAINBOW)
	{
		showRainbow(cmd[1]);
	}

	delay(100);
}

void setColor(byte r, byte g, byte b, int pixel)
{
	if(pixel == -1) 
	{
		for(int i = 0; i < NUM_PIXELS; i++)
		{
			pixels.setPixelColor(i, r, g, b);
		}
	}
	else
	{
		pixels.setPixelColor(pixel, r, g, b);
	}

	pixels.show();
}

void setBrightness(byte brightness)
{
	pixels.setBrightness(brightness);
	pixels.show();
}

void alternateColors()
{
}

void flashColor()
{
}

void showRainbow(byte wait)
{
	uint16_t i;
	uint16_t j;

	for(j = 0; j < 256 * 5; j++) 
	{
		for(i = 0; i < pixels.numPixels(); i++) 
		{
			pixels.setPixelColor(i, Wheel(( i + j) & 255));
		}

		pixels.show();
		delay(wait);
	}
}

uint32_t Wheel(byte WheelPos) 
{
	WheelPos = 255 - WheelPos;

	if(WheelPos < 85) 
	{
		return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if(WheelPos < 170) 
	{
		WheelPos -= 85;
		return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else 
	{
		WheelPos -= 170;
		return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}