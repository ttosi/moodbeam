#include <ThreadController.h>
#include <Thread.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include "moodbeam_v5.h"

SoftwareSerial serial(1, 2); // RX, TX
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(
	NUM_PIXELS,
	PIXEL_CONTROL_PIN,
	NEO_GRB + NEO_KHZ800
);

ThreadController controller = ThreadController();
Thread flashColorThread = Thread();
Thread alternateColorsThread = Thread();
Thread fadeThread = Thread();
Thread rainbowThread = Thread();

byte buffer[16];
bool toggle = false;
byte fadeBrightness;
int rainbowCounter;
bool isFlashColor = false;
bool isColorOne = false;
bool isRainbowRunning = false;
bool isFadingDown = true;

RGB color1;
RGB color2;
RGB off = { 0, 0, 0 };
RGB green = { 0, 255, 0 };
RGB red = { 255, 0, 0 };


ISR(TIMER0_COMPA_vect)
{
	controller.run();
}

void setup()
{
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02);
	TIMSK |= (1 << OCIE0A); 
	OCR0A = 255;

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

	pixels.begin();
	pixels.show();

	delay(2000);
	serial.begin(9600);
}

void loop()
{
	while (!serial.available());

	byte i = 0;
	delay(10);

	while(serial.available())
	{
		buffer[i] = serial.read();

		if(buffer[i] == 0x0A)
		{
			color1.r = buffer[1];
			color1.g = buffer[2];
			color1.b = buffer[3];

			switch (buffer[0])
			{
				case SHOW_COLOR:
					setColor(-1, color1);
					break;
				case TWO_COLOR:
					setColor(-1, off);
					break;
			}
		}

		i++;
	}
}

void setColor(int pixel, RGB color)
{
	if(pixel == -1) 
	{
		for(byte pix = 0; pix < NUM_PIXELS; pix++)
		{
			pixels.setPixelColor(pix, color.r, color.g, color.b);
		}
	}
	else
	{
		pixels.setPixelColor(pixel, color.r, color.g, color.b);
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
	if(toggle) 
	{
		setColor(-1, red);
	}
	else
	{
		setColor(-1, off);
	}

	toggle = !toggle;
}

void alternateColors()
{
	if(toggle)
	{
		setColor(0, color1);
		setColor(1, color2);
	}
	else
	{
		setColor(1, color1);
		setColor(0, color2);
	}

	toggle = !toggle;
}

void fade()
{
	if(toggle)
	{
		fadeBrightness -= 3;
		if(fadeBrightness <= 5)
		{
			toggle = false;
		}
	}
	else
	{
		fadeBrightness += 3;
		if(fadeBrightness >= 250)
		{
			toggle = true;
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