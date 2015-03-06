# moodbeams
Arduino, ESP8266 ESP-01 and NeoPixel light bar. Uses a pair of neopixels mounted on each end of an acrylic rod to create a beam of color. The lights are controlled by a PhoneGap application utilizing a RESTful'ish API.

The original idea came from the fact that I needed a way to alert people when I was coding "in the zone" so I would, hopefully, be left alone. My plan was to just mount a couple of LEDs behind my LCD that would illuminate the wall red with a button click. The LEDs weren't bright enough to see during the day. Shining them through an acrylic rod with scores in it made the light more visible in bright conditions. The project grew into using different colors to indicate various moods I was in. Buttons became cumbersome so I added an ESP8266 and a mobile app that allows me to choose any color.

# General Architecture
Some of this has not been written yet but here's the general idea of the process flow a user would do to setup and use the moodbeam.

(todo: insert diagram)

* On powering up the moodbeam controller checks for the exisitence of `config.lua`.
    * If `config.lua` _does not_ exist
        * Configures itself in AP mode
        * Connect to the MOODBEAM AP and browse to 192.168.4.1
        * A web page is returned where an SSID and password of the home network is entered (also an email and password that is used by the external node server)
        * ESP module writes the information to `config.lua` and resets itself
    * If `config.lua` does 
