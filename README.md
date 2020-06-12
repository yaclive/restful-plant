# RESTful Plant
By Clive Cooper

<img src="https://i.imgur.com/1QlZfPb.png" width="40%" />

## What is it?
RESTful Plant adds an undo function to horticulture. It adds the ability to draw nice colourful graphs to let you know exactly why you suck at growing plants. It serves as a conversation starter when your guests ask, "why exactly to you have a USB stick stuck in a pot of soil?". 

RESTful Plant is platform that mimicks the fundimental characteristics of a household plant. You stick it in a pot of soil and ensure it has adiquate light and water. Plant information can then be fetched using its API to then be used by an app to create beautiful visualisations and graphs. 

## Installation and Setup
Installation involves finding a pot, filling it with soil and inserting the RESTful Plant. The device is already calibrated for moisture, however, the LDR will need a day to collect light value samples of your environment, this is done automatically. The pot will need to be positioned somewhere near adiquate light and a source of power. When you a ready for the plant to begin its journey upward, plug in the RESTful Plant using a micro usb cable. 

## How it works
RESTful Plant uses a Capacitive Moisture Sensor and a Light Dependent Resistor to gather information about its environment. This data is taken and considered every five minutes by the brain, a Teensy 3.5. The Teensy keeps track of a number of variables that define the plant, such as its lifespan, satasfaction and health. Over time, the data the Teensy recieves from its sensors influences these variables in various ways. For example, if the plant is getting adiquate water and sunlight then its satasfaction increases, and so does its health. If the opposite environment occurs then the plant begins to die. 

Data is shared between the Teensy and an ESP8266 over Serial. The ESP8266 connects to your local wifi and hosts a RESTful API. The user can make HTTP requests to the ESP8266 and expect to recieve a JSON object containing the plant's status.

![Diagram](https://i.imgur.com/LpstiQu.png)

## Features
 - [x] Soil moisture sensing using a capacitive moisture sensor
 - [x] Light sensing using a LDR
 - [x] Auto calibration allowing for flexable usage
 - [x] API provided by a ESP8266

## References
[https://mancusoa74.blogspot.com/2018/02/simple-http-rest-server-on-esp8266.html](https://mancusoa74.blogspot.com/2018/02/simple-http-rest-server-on-esp8266.html)
[https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193](https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193)
[https://forum.pjrc.com/threads/27850-A-Guide-To-Using-ESP8266-With-TEENSY-3](https://forum.pjrc.com/threads/27850-A-Guide-To-Using-ESP8266-With-TEENSY-3)



## License
<p xmlns:dct="http://purl.org/dc/terms/" xmlns:cc="http://creativecommons.org/ns#" class="license-text"><a rel="cc:attributionURL" href="https://github.com/yaclive/restful-plant"><span rel="dct:title">RESTful Plant</span></a> by <a rel="cc:attributionURL" href="yaclive.github.com"><span rel="cc:attributionName">Clive Cooper</span></a> CC BY-NC 4.0<a href="https://creativecommons.org/licenses/by-nc/4.0"><img style="height:22px!important;margin-left: 3px;vertical-align:text-bottom;" src="https://search.creativecommons.org/static/img/cc_icon.svg" /><img  style="height:22px!important;margin-left: 3px;vertical-align:text-bottom;" src="https://search.creativecommons.org/static/img/cc-by_icon.svg" /><img  style="height:22px!important;margin-left: 3px;vertical-align:text-bottom;" src="https://search.creativecommons.org/static/img/cc-nc_icon.svg" /></a></p>
