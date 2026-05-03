#include <Wire.h>
#include "WatchyRTC.h"

// Patched constructor: initialize rtc_ds with the global Wire instance
WatchyRTC::WatchyRTC() : rtc_ds(Wire) {}
