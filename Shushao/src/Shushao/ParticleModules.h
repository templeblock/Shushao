#pragma once

#include "Color.h"
#include "Core.h"
#include "gradient.h"
#include "sepch.h"
#include "Variation.h"

namespace se {

struct ParticleSystemModule {
    bool enabled;  // Enable/disable the Emission module.
    void Enable() { enabled = true; }
    void Disable() { enabled = false; }
};

struct EmissionModule : public ParticleSystemModule {
    struct Burst {
        double time;
        unsigned int count;
        unsigned int cycles;
        double interval;
        float probability;

        unsigned int cycle = 0;
        double cycleTime = 0.0;

        Burst(double time_ = 0.0, unsigned int count_ = 20, unsigned int cycles_ = 0, double interval_ = 1.0, float probability_ = 1.0f) {
            Set(time_, count_, cycles_, interval_, probability_);
        }

        void Set(double time_ = 0.0, unsigned int count_ = 20, unsigned int cycles_ = 0, double interval_ = 1.0, float probability_ = 1.0f) {
            time = time_;
            count = count_;
            cycles = cycles_;
            interval = interval_;
            probability = probability_;
        }

        void reset() {
            cycle = 0;
            cycleTime = 0.0;
        }
    };

    unsigned int burstCount;  //	The current number of bursts.

    Variation rateOverDistance;  // The rate at which new particles are spawned, over distance.
    float rateOverDistanceScale = 1.0f;  // Change the rate over distance multiplier.
    Variation rateOverTime;  // The rate at which new particles are spawned, over time.
    float rateOverTimeScale = 1.0f;  // Change the rate over time multiplier.

    std::vector<Burst> bursts;

    void AddBurst(Burst burst_) {
        bursts.push_back(burst_);
    }

    void AddBurst(double time_ = 0.0, unsigned int count_ = 20, unsigned int cycles_ = 0, double interval_ = 1.0, float probability_ = 1.0f) {
        Burst b(time_, count_, cycles_, interval_, probability_);
        bursts.push_back(b);
    }
};

struct EmitterModule : public ParticleSystemModule {
    enum class Shape {
        SPHERE
    };

    Shape shape;
    bool is2D = false;

    // CIRCLE
    float radius = 0.0f;
    int arc = 360;
};

struct ColorOverLifetimeModule : public ParticleSystemModule {
    Gradient color;
};

struct SizeOverLifetimeModule : public ParticleSystemModule {
    bool separateAxes = false;
    Curve size;
    glm::vec2 sizeScale = glm::vec2(1.0f);

    Curve xSize;
    Curve ySize;
};

struct ForceOverLifetimeModule : public ParticleSystemModule {
    // TODO
};

struct RotationBySpeedModule : public ParticleSystemModule {
    // TODO
};

struct RotationOverLifetimeModule : public ParticleSystemModule {
    // TODO
};

struct SizeBySpeedModule : public ParticleSystemModule {
    // TODO
};

struct VelocityOverLifetimeModule : public ParticleSystemModule {
    // TODO
};

struct CollisionModule : public ParticleSystemModule {
    // TODO
};

}  // namespace se
