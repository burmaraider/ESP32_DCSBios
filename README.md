# ESP32_DCSBios

<p align="center">
  <a href="#about">About</a> •
  <a href="#function?">Function</a> •
  <a href="#features">Features</a> •
  <a href="#usage">Usage</a> •
  <a href="#todo">Todo</a> •
  <a href="#license">License</a>
</p>

## A wireless DCS-Bios solution that runs on an ESP32 using WiFi.
----

### **About**

This project is based on ESP32_MultiModule_DCSBios (https://github.com/pavidovich/ESP32_MultiModuleDCSBios)

This is an attempt to create a simple to use DCS-Bios interface through a WiFi connection. No one likes 10+ usb cables running around everywhere. 

----

### **Function**

This main class will subscribe to the multicast address in your network to send a receive data to and from DCS through DCS-Bios. 

ESP32DCSBios is the main class that will do the following for you:

* Initialize the WiFi Connection
* Has it's own `Update()` routine.
* Handles a single `Aircraft` reference to the `Aircraft` with its own `Update()` routine.

    ### **Features:**

    * `Aircraft` class to implement any aircraft DCS supports with ease
    * Each `Aircraft` child class contains its own `Update()` routine where you can do additional work keeping your ***main.cpp*** file clean
    * Command builder that allows you to send whatever command you want to any button that subscribes to it in DCS with `SendCommand()`
        * Commands:
            * INC
            * DEC
            * TOGGLE
            * SET

----

### **Usage**

ESP_DCSBios is very simple to setup and use. Very little boiler plate is required to get it up and running and listening to DCS-Bios.

```c++
#include "ESP32DCSBios.h" //DCS-Bios link
#include "Aircraft/F14.h" // Aircraft to use

ESP32DCSBios _dcs;

const char *ssid = "SSID";
const char *password = "PASSWORD";

void setup()
{
    // Set up the DCSBIOS interface
    _dcs = ESP32DCSBios((char *)ssid, (char *)password);

    F14 *f14 = new F14(&_dcs);
    _dcs.SetAirCraft(f14);
}

void loop()
{

    //Do button handling here
    ReadButtons();

    _dcs.Update(); //Call the DCS update, this also calls the Aircraft Update

    //You can check if ESP32DCSBios has "booted"
    if (_dcs.HasBooted())
    {
        DoThing();
    }

}
```

That's all that's required to get going. With the power this library has, you can easily react to any change the aircraft may experience at an `Aircraft` level; tailor made to each `Aircraft` you add.

----

### **Todo**

* ☐ RS485 and I2C Communication between nodes (Different Panels)
* ☐ Modular panel module creation
* ☐ USB Connection
* ☐ Web configuration tool

----

## License
[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg?style=flat-square)](https://tldrlegal.com/license/gnu-lesser-general-public-license-v3-(lgpl-3))