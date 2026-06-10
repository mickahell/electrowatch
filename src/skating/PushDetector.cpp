#include "PushDetector.h"
#include <Arduino.h>

bool PushDetector::detect(float magnitude) {
    unsigned long now = millis();

    float baseline = 1.0f;
    baseline = 0.99f * baseline + 0.01f * magnitude;

    float delta = fabs(magnitude - baseline);

    bool outbound = not (min_threshold <= delta && delta <= max_threshold);

    if (outbound && (now - lastPushTime) > debounce) {
        lastPushTime = now;
        return true;
    }

    return false;
}
