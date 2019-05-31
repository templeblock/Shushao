#pragma once

#include "Core.h"
#include "sepch.h"

namespace se {

class Playable {
public:
    enum class State {
        STOP,
        PLAY,
        PAUSE
    };

    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;

    float GetDuration() {
        return duration;
    }

    Playable::State GetState() {
        return state;
    }

    bool GetLoop() {
        return loop;
    }

    Playable* SetState(State value) {
        state = value;
        return this;
    }

    Playable* SetStartState(State value) {
        startState = value;
        return this;
    }

    Playable* SetLoop(bool value) {
        loop = value;
        return this;
    }

    Playable* SetDuration(float value) {
        duration = value;
        return this;
    }

protected:
    double elapsed = 0.0;
    double ratio = 0.0;

    State state = State::STOP;
    State startState = State::STOP;

    float duration;
    bool loop;
};
}  // namespace se