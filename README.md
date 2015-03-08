# moodbeam
Arduino, ESP8266 ESP-01 and NeoPixel light bar. Uses a pair of neopixels mounted on each end of an acrylic rod to create a beam of color. The lights are controlled by a RESTful'ish API.

The original idea came from the fact that I needed a way to alert people when I was coding "in the zone" so I would, hopefully, be left alone. My plan was to just mount a couple of LEDs behind my LCD that would illuminate the wall red with a button click. The LEDs weren't bright enough to see during the day. Shining them through an acrylic rod with scores in it made the light more visible in bright conditions. The project grew into using different colors to indicate various moods I was in. Buttons became cumbersome so I added an ESP8266 and a mobile app that allows me to choose any color.

![alt As a red beam](https://raw.githubusercontent.com/ttosi/moodbeam/master/red_beam.jpg)

![alt As a blue beam](https://raw.githubusercontent.com/ttosi/moodbeam/master/blue_beam.jpg)

## General Architecture
Some of this has not been written yet but here's the general idea of the process flow a user would do to setup and use the moodbeam.

(todo: insert diagram)


### Moodbeam Controller
__Initial Configuration for moodbeam controller__ (`config.lua` does not exist on the ESP8266's flash):
* Configures itself in AP mode
* User connects to the MOODBEAM AP and browses to 192.168.4.1
* The ESP module returns a webpage where an SSID and password of the home network are entered (also an email and password that is used by the external node server)
* ESP module writes the information to `config.lua` and resets itself
* User recconects to their home network
   
__Normal Operation for moodbeam controller__ (`config.lua` exists on the ESP8266's flash):
* Reads the ssid, ssid password, username & password from `config.lua`
* Sets itself in station mode and connects to the home wifi network
* Makes a TCP connection to the external nodejs server sending the configured username and password which is registered on the server
* Sends a heartbeat byte to the server every hour(?) to keep the connection alive
   
### Server
__Initial User Setup__ 
* _Before_ a user configures a moodbeam they must have created an account
* User creates account: email address & password
* Confirm email account
* Send email which contains instructions and a link to the AP (so they don't have to type in an IP address)
* Configures as described above

__Normal User Opertation__
* A phonegap app or a web ui can be used to control the moodbeam from anywhere
