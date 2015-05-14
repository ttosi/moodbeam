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

byte buffer[16];
byte fadeBrightness;
bool toggle = false;
bool isFadingDown = true;

RGB color1;
RGB color2;
RGB off = { 0, 0, 0 };

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

	controller.add(&flashColorThread);
	controller.add(&alternateColorsThread);
	controller.add(&fadeThread);

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
			byte command = buffer[0];

			flashColorThread.enabled = false;
			alternateColorsThread.enabled = false;
			fadeThread.enabled = false;
			setBrightness(255);

			if(command != FLASH) 
			{
				color1.r = buffer[1];
				color1.g = buffer[2];
				color1.b = buffer[3];

				color2.r = buffer[4];
				color2.g = buffer[5];
				color2.b = buffer[6];
			}

			switch (command)
			{
				case SHOW_COLOR:
					setColor(-1, color1);
					break;

				case TWO_COLOR:
					setColor(0, color1);
					setColor(1, color2);
					break;

				case ALTERNATE_COLORS:
					alternateColorsThread.setInterval(buffer[7] * 50);
					alternateColorsThread.enabled = true;
					break;
				
				case FLASH:
					flashColorThread.setInterval(buffer[1] * 50);
					flashColorThread.enabled = true;
					break;

				case FADE:
					fadeBrightness = 255;
					toggle = true;

					fadeThread.setInterval(buffer[1] * 1);
					fadeThread.enabled = true;
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
		setColor(0, color1);
		setColor(1, color2);
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