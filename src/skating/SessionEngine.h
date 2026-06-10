#pragma once

struct SessionData {
    int pushCount;
    float distance;
    uint32_t elapsed;
    uint32_t pausedElapsed;
    uint32_t sessionStartTime;
    bool running;
};

extern SessionData sessionState;

class SessionEngine {
    public:
        void start(uint32_t timeNow);
        void stop(uint32_t timeNow);
        void pause(uint32_t timeNow);
        void update(bool pushDetected, float mag, uint32_t timeNow);

        SessionData getData();
        bool isRunning();
};
