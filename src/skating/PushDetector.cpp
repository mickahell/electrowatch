#include "PushDetector.h"
#include <Arduino.h>

bool PushDetector::detect(float magnitude) {
    unsigned long now = millis();

    static float baseline = 1.0f;
    baseline = 0.99f * baseline + 0.01f * magnitude;

    float delta = fabs(magnitude - baseline);

    bool above = delta > threshold;

    if (above && !wasAbove && (now - lastPushTime) > debounce) {
        lastPushTime = now;
        wasAbove = true;
        return true;
    }

    if (!above) {
        wasAbove = false;
    }

    return false;
}
