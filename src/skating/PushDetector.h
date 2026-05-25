#pragma once

class PushDetector {
    public:
        bool detect(float magnitude);
        void setThreshold(float t) { threshold = t; }
        void setDebounce(int d) { debounce = d; }
        float getThreshold() const { return threshold; }
        int getDebounce() const { return debounce; }

    private:
        unsigned long lastPushTime = 0;
        float threshold = 0.15;      // tune this
        int debounce = 120;         // ms
        bool wasAbove = false;
};
