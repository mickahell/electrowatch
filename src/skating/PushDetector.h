#pragma once

class PushDetector {
public:
    bool detect(float magnitude);

private:
    unsigned long lastPushTime = 0;
    float threshold = 1.5;      // tune this
    int debounce = 400;         // ms
};
