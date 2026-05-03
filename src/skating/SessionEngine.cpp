#include <Arduino.h>
#include "SessionEngine.h"

void SessionEngine::start() {
    pushCount = 0;
    distance = 0;
    startTime = millis();
    running = true;
}

void SessionEngine::stop() {
    running = false;
}

void SessionEngine::update(bool pushDetected) {
    if (!running) return;

    if (pushDetected) {
        pushCount++;
        distance = pushCount * 1.5; // meters per push (tune later)
    }
}

SessionData SessionEngine::getData() {
    SessionData data;
    data.pushCount = pushCount;
    data.distance = distance;
    data.elapsed = running ? (millis() - startTime) / 1000 : 0;
    data.running = running;
    return data;
}

bool SessionEngine::isRunning() {
    return running;
}
