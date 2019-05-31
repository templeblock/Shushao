#include "part.h"

using namespace se;

void Part::Awake() {
    /* Particle System */

    ParticleSystem* ps = AddComponent<ParticleSystem>();
    ps->SetDuration(10.0f)->SetLoop(true);
    ps->SetTexture(Resources::Get<Texture>("particlestar"))
        ->SetMaxParticles(1000)
        ->SetPlayOnAwake(true)
        ->SetSimulationSpace(Transform::Origin::WORLD)
        // ->SetEmitterVelocityMode(ParticleSystem::EmitterVelocityMode::IGNORE);
        // ->SetStartDelay(3.0f)
        ->SetStartSize({0.6, 0.6})
        ->SetStartLifetime(5.0f)
        ->SetStartColor(color::green)
        ->SetStartSpeed(1.5f);

    ps->emitter.Enable();
    ps->emitter.shape = EmitterModule::Shape::SPHERE;
    ps->emitter.radius = 0.0f;
    ps->emitter.arc = 360;
    ps->emitter.is2D = true;

    ps->emission.Enable();
    //ps->emission.rateOverTime = Variation::Range({0, 400});
    Curve curve = Curve::EaseInOut(0.0f, 0.0f, 1.0f, 1.0f);
    ps->emission.rateOverTime.Set(400.0f, curve);
    ps->emission.rateOverDistance = 20;
    //ps->emission.AddBurst(2.0, 100, 0, 1.0, 0.6f);

    ps->sizeOverLifetime.Enable();
    ps->sizeOverLifetime.size = Curve::EaseInOut(0.0f, 0.2f, 1.0f, 1.0f);

    ps->colorOverLifetime.Enable();
    Gradient gradient(color::white, Color(1.0f, 0.0f, 0.0f, 0.0f));
    Curve c = Curve::EaseInOut(0.0f, 0.0f, 1.0f, 1.0f);
    ps->colorOverLifetime.color.Set(color::white, Color(1.0f, 0.0f, 0.0f, 0.0f), c);
}