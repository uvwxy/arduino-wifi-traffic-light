# Wifi Traffic Light

This project enables the [wifi traffic](https://www.thingiverse.com/thing:2463193) light to be set from outside via an API.

## Building

Copy `config.h.example` to `config.h` and modify the WiFi connection and Api authentication settings.

```
#define WIFI_SSID "WiFiName"
#define WIFI_PASS "WiFiPass""

#define HTTP_USER "ApiUserName"
#define HTTP_PASS "ApiUserPass"
```

Flash with arduino to a Wemos D1 Mini, configured as described in the project linked above.

## Running

Flash the device and power via USB. The IP address will be printed to the serial console (115200 baud):

```
IP address: 192.168.178.57```

During setup the device will try to connect to the WiFi every 10 seconds. 
The green light will be on after connecting and the server is started.

## API /api/lights

### On/Off
With the following query parameters you can set the state of the lights:

- `red` *0* off, *1* on
- `yellow` *0* off, *1* on
- `green` *0* off, *1* on

Omitting a value will set it to *0*.

**Example:** *(Turn on: red + green, Turn off: yellow)*
```
curl --user ApiUserName:ApiUserPass "http://192.168.178.57/api/lights?red=1&yellow=0&green=1"
```

### Flashing
With the following query parameters you can flip the on/off state every *n/10* seconds:

- `flashRed` *n* = (0...255)
- `flashYellow` *n* = (0...255)
- `flashGreen` *n* = (0...255)

Omitting a value will set it to *0*.

**Example:** *(Flash green every 1 second)*

```
 curl --user ApiUserName:ApiUserPass "http://192.168.178.57/api/lights?green=1&flashGreen=10"
```

