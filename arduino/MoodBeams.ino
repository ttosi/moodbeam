//-----------------------------------
//	MoodBeams v0.2
//	01/17/2015
//
//	Tony S. Tosi
//	ttosi519@gmail.com
//	irc & github: ttosi
//	twitter: tonytosi
//	http://blog.turningdigital.com
//------------------------------------

#include "MoodBeams.h"
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

byte command[5];

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
			command[i] = Serial.read();
		}

		switch (command[0])
		{
			case SET_COLOR:
				setColor(command[1], command[2], command[3]);
				break;
			case SET_BRIGHTNESS:
				setBrightness(command[1]);
				break;
			case SET_PRESET:
				setPreset(command[1]);
				break;
			case DEFINE_PRESET:
				definePreset(command[1], command[2], command[3], command[4]);
				break;
			case SHOW_RAINBOW:
				showRainbow(command[1]);
				break;
		}
	}

	if(command[0] == SHOW_RAINBOW)
	{
		showRainbow(command[1]);
	}

	delay(100);
}

void setColor(byte r, byte g, byte b)
{
	for(int pixel = 0; pixel < NUM_PIXELS; pixel++)
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

void setPreset(byte address)
{
	byte r = EEPROM.read(address);
	byte g = EEPROM.read(++address);
	byte b = EEPROM.read(++address);

	setColor(r, g, b);
}

void definePreset(int address, byte r, byte g, byte b)
{
	EEPROM.write(address, r);
	EEPROM.write(++address, g);
	EEPROM.write(++address, b);

	flashColor(4, r, g, b);
	setColor(r, g, b);
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
		setColor(r, g, b);
		delay(125);

		setColor(0, 0, 0);
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