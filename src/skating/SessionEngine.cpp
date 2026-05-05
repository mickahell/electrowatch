#include <Arduino.h>
#include "SessionEngine.h"

// RTC persistent state
RTC_DATA_ATTR SessionData sessionState = {
    0, 0.0f, 0, 0, false
};

// ------------------ ENGINE ------------------

void SessionEngine::start(uint32_t timeNow) {
    if (sessionState.running) return;

    sessionState.pushCount = 0;
    sessionState.distance = 0;
    sessionState.running = true;

    sessionState.sessionStartTime = timeNow;
}

void SessionEngine::stop(uint32_t timeNow) {
    sessionState.running = false;
    sessionState.elapsed = timeNow - sessionState.sessionStartTime;
}

void SessionEngine::update(bool pushDetected, uint32_t timeNow) {
    if (!sessionState.running) return;

    if (pushDetected) {
        sessionState.pushCount++;
        sessionState.distance = sessionState.pushCount * 1.5;
    }
    sessionState.elapsed = timeNow - sessionState.sessionStartTime;
}

SessionData SessionEngine::getData() {
    SessionData data = sessionState;
    return data;
}

bool SessionEngine::isRunning() {
    return sessionState.running;
}
