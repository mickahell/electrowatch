#include "PushDetector.h"
#include <Arduino.h>

bool PushDetector::detect(float magnitude) {
    unsigned long now = millis();

    if (magnitude > threshold && (now - lastPushTime) > debounce) {
        lastPushTime = now;
        return true;
    }

    return false;
}
