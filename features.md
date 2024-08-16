# Features

By default, all the basics features from the Watchy are available :

- __Active Features__
    - About Watchy
    - Vibrate Motor
    - Show Accelerometer
    - Set Time
    - Setup WiFi
    - Update Firmware
    - Sync NTP
- __Passive Features__
    - Wifi
    - Bluetooth
    - Steps
    - Weather
    - Menu
    - Vibrate
    - Accelerometer
    - NTP
    - ...

And some features have been add due to some needs or just for fun.

## Darkmode

To activate the darkmode just set the var `DARKMODE` to true.

### Parameters

```cpp
#define DARKMODE true
```

## WiFi connection

Set the WiFi setting as code directly in the parameters.
Add a 2nd coneection as code.

__Passive feature__

Having the main WiFi settings as code, in order to reconnect-it automatically in case of reuploading sofware.

__Active feature__

In the menu, I added a 2ndary WiFi connection as code, in order to be able to setup an other WiFi point without using the web interface.

### Parameters

```cpp
#define WIFI_SSID_DEF "Pastabox-test"
#define WIFI_PASS_DEF "test"
#define WIFI_SSID_2ND "Volophone-test"
#define WIFI_PASS_2ND "test"
```

## Blagues du jour

A colleague of mine really like joke and really often come to me to ask one of my baddest joke. So I thougt implementing a joke generator or a joke downloader from the Watchy would be very helpful.

__Passive feature__

Everyday at `00h00min`, download 10 jokes from the `BLAGUE_URL` var API and register them has files (if network is available).

__Active feature__

A button is configure to show a joke from :

- download from the API
- read a joke from one of the file (random) in case network isn't available

### Parameters

```cpp
#define BLAGUE_TYPE "dev"
#define BLAGUE_URL "https://www.blagues-api.fr/api/type/" + String(BLAGUE_TYPE) + "/random"
#define BLAGUE_TOKEN "test"
```

## Send API

Make the Watchy a real smartchwatch by doing something _useful_ of the data generate by the watch.

__Passive feature__

Every hour at `xxh59min`, send data of the watch from the last hour to the `API_URL` var API from certains rules :

- if network isn't available, register the data in files
- if network is available send the data from the last hour
- if network is available send any registered data and delete the local files

From now this only allow to send :

- steps

### Parameters

```cpp
#define API_URL ""
#define API_TOKEN ""
```
