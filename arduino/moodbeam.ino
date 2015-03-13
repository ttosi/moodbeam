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
#include <ThreadController.h>
#include <Thread.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#include "moodbeam.h"

ThreadController controller = ThreadController();
Thread flashColorThread = Thread();
Thread alternateColorsThread = Thread();
Thread fadeThread = Thread();
Thread rainbowThread = Thread();

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	NUM_PIXELS,
	PIXEL_CONTROL_PIN,
	NEO_GRB + NEO_KHZ800
);

String cmd;
byte colorOne[3];
byte colorTwo[3];
byte fadeBrightness;
int rainbowCounter;

bool isFlashColor = false;
bool isColorOne = false;
bool isRainbowRunning = false;
bool isFadingDown = true;

void setup()
{
	flashColorThread.onRun(flashColor);
	flashColorThread.enabled = false;
	
	alternateColorsThread.onRun(alternateColors);
	alternateColorsThread.enabled = false;

	fadeThread.onRun(fade);
	fadeThread.enabled = false;

	rainbowThread.onRun(showRainbow);
	rainbowThread.enabled = false;

	controller.add(&flashColorThread);
	controller.add(&alternateColorsThread);
	controller.add(&fadeThread);
	controller.add(&rainbowThread);
	
	delay(2000);
	Serial.begin(9600);

	pixels.begin();
	pixels.show();
	
	Timer1.initialize(10000);
	Timer1.attachInterrupt(timerCallback);
	Timer1.start();
}

void loop()
{
	readCommand();
}

void readCommand() 
{
	while (!Serial.available());

	delay(5);

	while(Serial.available()) 
	{
		char data = (char)Serial.read();
		cmd += data;

		if(data == 0x0A) {

			flashColorThread.enabled = false;
			alternateColorsThread.enabled = false;
			fadeThread.enabled = false;
			rainbowThread.enabled = false;
				
			setBrightness(255);

			switch (cmd[0])
			{
				case (char)SHOW_COLOR:
					colorOne[0] = cmd[1];
					colorOne[1] = cmd[2];
					colorOne[2] = cmd[3];

					setColor(-1, cmd[1], cmd[2], cmd[3]);
					break;

				case (char)SET_BRIGHTNESS:
					setBrightness(cmd[1]);
					break;

				case (char)TWO_COLOR:
					setColor(0, cmd[1], cmd[2], cmd[3]);
					setColor(1, cmd[4], cmd[5], cmd[6]);
					break;

				case (char)ALTERNATE_COLORS:
					colorOne[0] = cmd[1];
					colorOne[1] = cmd[2];
					colorOne[2] = cmd[3];

					colorTwo[0] = cmd[4];
					colorTwo[1] = cmd[5];
					colorTwo[2] = cmd[6];

					alternateColorsThread.setInterval(cmd[7] * 50);
					alternateColorsThread.enabled = true;
					break;

				case (char)FLASH_COLOR:
					flashColorThread.setInterval(cmd[1] * 50);
					flashColorThread.enabled = true;
					break;

				case (char)SHOW_RAINBOW:
					rainbowCounter = 0;

					rainbowThread.setInterval(cmd[1] * 1);
					rainbowThread.enabled = true;
					break;

				case (char)FADE:
					fadeBrightness = 255;
					isFadingDown = true;

					fadeThread.setInterval(cmd[1] * 1);
					fadeThread.enabled = true;
					break;
			}
			
			cmd = "";
		}
	}
}

void timerCallback()
{
	controller.run();
}

void setColor(int pixel, byte r, byte g, byte b)
{
	if(pixel == -1) 
	{
		for(int pix = 0; pix < NUM_PIXELS; pix++)
		{
			pixels.setPixelColor(pix, r, g, b);
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

void flashColor()
{
	if(isFlashColor) 
	{
		setColor(-1, colorOne[0], colorOne[1], colorOne[2]);
	}
	else
	{
		setColor(-1, 0, 0, 0);
	}

	isFlashColor = !isFlashColor;
}

void alternateColors()
{
	if(isColorOne)
	{
		setColor(0, colorOne[0], colorOne[1], colorOne[2]);
		setColor(1, colorTwo[0], colorTwo[1], colorTwo[2]);
	}
	else
	{
		setColor(1, colorOne[0], colorOne[1], colorOne[2]);
		setColor(0, colorTwo[0], colorTwo[1], colorTwo[2]);
	}

	isColorOne = !isColorOne;
}

void fade()
{
	if(isFadingDown)
	{
		fadeBrightness -= 3;
		if(fadeBrightness <= 5)
		{
			isFadingDown = false;
		}
	}
	else
	{
		fadeBrightness += 3;
		if(fadeBrightness >= 250)
		{
			isFadingDown = true;
		}
	}

	setBrightness(fadeBrightness);
}

void showRainbow()
{
	rainbowCounter++;
	if(rainbowCounter >= 256 * 5)
	{
		rainbowCounter = 0;
	}

	for(int i = 0; i < pixels.numPixels(); i++) 
	{
		pixels.setPixelColor(i, Wheel(( i + rainbowCounter) & 255));
	}

	pixels.show();
}

uint16_t Wheel(byte WheelPos) 
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