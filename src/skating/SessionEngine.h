#pragma once

struct SessionData {
    int pushCount;
    float distance;
    uint32_t elapsed;
    bool running;
};

class SessionEngine {
    public:
        void start();
        void stop();
        void update(bool pushDetected);

        SessionData getData();
        bool isRunning();

    private:
        int pushCount = 0;
        float distance = 0;
        uint32_t startTime = 0;
        bool running = false;
};
