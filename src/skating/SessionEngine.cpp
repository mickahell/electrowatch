#include <Arduino.h>
#include "SessionEngine.h"

// RTC persistent state
RTC_DATA_ATTR SessionData sessionState = {
    0, 0.0f, 0, 0, 0, false
};

// ------------------ ENGINE ------------------

void SessionEngine::start(uint32_t timeNow) {
    if (sessionState.running) return;

    sessionState.pushCount = 0;
    sessionState.distance = 0;
    sessionState.elapsed = 0;
    sessionState.pausedElapsed = 0;
    sessionState.sessionStartTime = timeNow;
    sessionState.running = true;
}

void SessionEngine::stop(uint32_t timeNow) {
    sessionState.running = false;
    sessionState.elapsed = timeNow - sessionState.sessionStartTime;
}

void SessionEngine::pause(uint32_t timeNow) {
    if (!sessionState.running) {
        // Resume
        sessionState.sessionStartTime = timeNow;
        sessionState.running = true;
    } else {
        // Pause
        sessionState.pausedElapsed += timeNow - sessionState.sessionStartTime;
        sessionState.running = false;
    }
}

void SessionEngine::update(bool pushDetected, float mag, uint32_t timeNow) {
    if (!sessionState.running) return;

    if (pushDetected) {
        sessionState.pushCount++;
        sessionState.distance += fabs(mag - 1000) / 100 * 1.5;
    }
    sessionState.elapsed = sessionState.pausedElapsed + (timeNow - sessionState.sessionStartTime);
}

SessionData SessionEngine::getData() {
    SessionData data = sessionState;
    return data;
}

bool SessionEngine::isRunning() {
    return sessionState.running;
}
