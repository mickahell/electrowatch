#pragma once

class PushDetector {
    public:
        bool detect(float magnitude);
        void setDebounce(int d) { debounce = d; }
        int getDebounce() const { return debounce; }

    private:
        unsigned long lastPushTime = 0;
        float min_threshold = 925;      // tune this
        float max_threshold = 1075;      // tune this
        int debounce = 1000;         // ms
        bool wasOutbound = false;
};
