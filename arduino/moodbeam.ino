//-----------------------------------
//	moodbeam v0.2
//	https://github.com/ttosi/moodbeams
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

byte cmd[5];

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
}

void loop()
{
	if (Serial.available() >= 5) 
	{
		for (int i = 0; i < 5; i++) 
		{
			cmd[i] = Serial.read();
		}

		switch (cmd[0])
		{
			case SHOW_COLOR:
				setColor(cmd[1], cmd[2], cmd[3], 0);
				break;
			case SET_BRIGHTNESS:
				setBrightness(cmd[1]);
				break;
			case ALTERNATE_COLORS:
				break;
			case TWO_COLOR:
				break;
			case FLASH_COLOR:
				break;
			case SHOW_RAINBOW:
				showRainbow(cmd[1]);
				break;
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
	if(pixel == 0) 
	{
		for(int index = 0; pixel < NUM_PIXELS; index++)
		{
			pixels.setPixelColor(index, r, g, b);
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

void alternateColors(int address1, int address2, byte delay)
{
}

void twoColor(int address, int address2)
{
}

void flashColor(int address, byte delay)
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

void flashColor(int flashes, byte r, byte g, byte b)
{
	for(int i = 0; i < flashes; i++) 
	{
		setColor(r, g, b, 0);
		delay(125);

		setColor(0, 0, 0, 0);
		delay(75);
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