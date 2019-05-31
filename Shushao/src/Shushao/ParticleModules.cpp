#include "sepch.h"

#include <glad/glad.h>

#include "Core.h"
#include "Entity.h"
#include "ParticleSystem.h"
#include "Physics/Rigidbody2d.h"
#include "Shaders/ParticlesShader.h"
#include "Transform.h"
#include "Utility.h"

namespace se {

	void ParticleSystem::setup() {
		material = new Material();
		material->SetShader(new ParticlesShader());

		VAO = new VertexArray();
		VAO->AddBuffer("positions", { GL_ARRAY_BUFFER, ShaderLocation::LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0, GL_DYNAMIC_DRAW });
		VAO->AddBuffer("colors", { GL_ARRAY_BUFFER, ShaderLocation::LOCATION_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void*)0, GL_DYNAMIC_DRAW });
		VAO->AddBuffer("sizes", { GL_ARRAY_BUFFER, ShaderLocation::LOCATION_SIZE, 2, GL_FLOAT, GL_FALSE, 0, (void*)0, GL_DYNAMIC_DRAW });
		VAO->Init();
	}

	ParticleSystem::~ParticleSystem() {
		if (material != nullptr) delete (material);
		delete (VAO);
		VAO = nullptr;
	}

	ParticleSystem* ParticleSystem::SetTexture(Texture* value_) {
		texture = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetMaxParticles(unsigned int max) {
		maxParticles = max;
		particles.clear();
		particles.reserve(max);
		positions.reserve(max);
		colors.reserve(max);
		sizes.reserve(max);
		return this;
	}

	bool ParticleSystem::isReady() {
		return (
			material != nullptr &&
			material->shader != nullptr &&
			transform != nullptr);
	}

	void ParticleSystem::Awake() {
		if (texture != nullptr) {
			material->SetMainTexture(texture);
		}

		material->shader->Use();
		if (simulationSpace == Transform::Origin::LOCAL) {
			material->shader->Disable("worldspace");
		}
		else {
			material->shader->Enable("worldspace");
		}
		material->shader->Leave();

		if (startState == Playable::State::PLAY) {
			Play();
		}
	}

	void ParticleSystem::Play() {
		if (!isReady()) return;
		state = Playable::State::PLAY;
		startTime = Time::GetTime();
		isLoopEnded = false;
		elapsed = 0.0f;
		ratio = 0.0f;
		isPlaying = true;
		isStopped = false;
		isPaused = false;
	}

	void ParticleSystem::Pause() {
		if (!isReady()) return;
		state = Playable::State::PAUSE;
		isPlaying = false;
		isStopped = false;
		isPaused = true;
	}

	void ParticleSystem::Stop() {
		if (!isReady()) return;
		state = Playable::State::STOP;
		Clear();
		isPlaying = false;
		isStopped = true;
		isPaused = false;
	}

	void ParticleSystem::EmitParticle() {
		if (!emitter.enabled) return;

		// common properties
		Color color = startColor;
		float rotation = startRotation;
		glm::vec2 size = startSize;
		float lifetime = startLifetime;
		glm::vec3 position = (simulationSpace == Transform::Origin::LOCAL ? Transform::VEC3_ZERO : transform->position);
		glm::vec3 direction = transform->forward;
		glm::vec3 velocity;

		if (emitterVelocityMode == EmitterVelocityMode::RIGIDBODY && entity->GetComponent<Rigidbody2D>() != nullptr) {
			velocity = glm::vec3(entity->GetComponent<Rigidbody2D>()->velocity, 0.0f);
		}
		else if (emitterVelocityMode == EmitterVelocityMode::TRANSFORM) {
			velocity = transform->velocity;
		}
		else {
			velocity = Transform::VEC3_ZERO;
		}

		switch (emitter.shape) {
		case EmitterModule::Shape::SPHERE:

			float inclination = glm::radians((float)util::random(0, emitter.arc));
			float azimuth = glm::radians((float)util::random(0, emitter.arc));

			float sInclination = sinf(inclination);

			if (emitter.is2D) {
				direction.x = cosf(azimuth);
				direction.y = sinf(azimuth);
				direction.z = 0.0f;
			}
			else {
				direction.x = sInclination * cosf(azimuth);
				direction.y = sInclination * sinf(azimuth);
				direction.z = cosf(inclination);
			}

			AddParticle(
				(direction * emitter.radius) + position,  // position
				direction * startSpeed + velocity,  // velocity
				color,  // color
				rotation,  // rotation
				size,  // size
				lifetime  // lifetime
			);

			break;
		}
	}

	void ParticleSystem::Emit(unsigned int count) {
		for (unsigned int c = 0; c < count; c++) {
			EmitParticle();
		}
	}

	void ParticleSystem::UpdateParticles() {
		positions.clear();
		colors.clear();
		sizes.clear();

		particleCount = 0;

		// update particles
		for (auto& particle : particles) {
			if (particle->age >= particle->lifetime) {
				continue;
			}
			particleCount++;
			particle->position += particle->velocity * Time::deltaTime;
			particle->age += Time::deltaTime;

			float moment = particle->age / particle->lifetime;

			// process sizeOverLifetime
			if (sizeOverLifetime.enabled && particle->lifetime > 0) {
				glm::vec2 scale = glm::vec2(
					startSize.x * sizeOverLifetime.sizeScale.x,
					startSize.y * sizeOverLifetime.sizeScale.y);
				if (!sizeOverLifetime.separateAxes) {
					float sizevar = sizeOverLifetime.size.Evaluate(moment);
					particle->size = glm::vec2(
						sizevar * scale.x,
						sizevar * scale.y);
				}
				else {
					particle->size = glm::vec2(
						sizeOverLifetime.xSize.Evaluate(moment) * scale.x,
						sizeOverLifetime.ySize.Evaluate(moment) * scale.y);
				}
			}

			// process colorOverLifetime
			if (colorOverLifetime.enabled) {
				particle->color = colorOverLifetime.color.Evaluate(moment);
			}

			positions.push_back(particle->position);
			colors.push_back(particle->color.rgba());
			sizes.push_back(particle->size);
		}
	}

	void ParticleSystem::Update() {
		if (!isReady()) return;

		if (startDelay > 0.0f && Time::GetTime() - startTime < startDelay) {
			return;
		}

		if (isPlaying && emission.enabled && !isLoopEnded) {
			ProcessEmission();
			ProcessBursts();
			isEmitting = true;
		}
		else {
			isEmitting = false;
		}

		if (isPlaying) {
			UpdateParticles();
			LoadBuffers();
		}

		if (isLoopEnded && particleCount == 0) {
			Stop();
			LoadBuffers();
		}

		elapsed += Time::deltaTime;
		ratio = elapsed / duration;
		if (elapsed >= duration) {
			if (!loop) {
				isLoopEnded = true;
			}
			elapsed = 0.0f;
		}
	}

	void ParticleSystem::ProcessEmission() {
		float rate = emission.rateOverTime.Evaluate(ratio);
		if (rate > 0.0f) {
			toEmit += rate * emission.rateOverTimeScale * Time::deltaTime;
			if (toEmit >= 1.0) {
				int toEmitInt = floor(toEmit);
				Emit(toEmitInt);
				toEmit -= toEmitInt;
			}
		}

		float drate = emission.rateOverDistance.Evaluate(ratio);
		if (drate > 0.0f) {
			toEmitDistance += drate * emission.rateOverDistanceScale * distance(transform->position, lastPosition);
			if (toEmitDistance >= 1.0) {
				int toEmitInt = floor(toEmitDistance);
				Emit(toEmitInt);
				toEmitDistance -= toEmitInt;
			}
			lastPosition = transform->position;
		}
	}

	void ParticleSystem::ProcessBursts() {
		if (emission.bursts.size() > 0) {
			for (auto& b : emission.bursts) {
				if (b.cycle == 0) {
					if (elapsed >= b.time) {
						if (b.probability >= 1.0f || util::happens(b.probability)) {
							Emit(b.count);
						}
						b.cycle++;
						b.cycleTime = Time::GetTime();
					}
				}
				else {
					if (b.cycles > 0 && b.cycle >= b.cycles) continue;
					if (Time::GetTime() - b.cycleTime >= b.interval) {
						if (b.probability >= 1.0f || util::happens(b.probability)) {
							Emit(b.count);
						}
						b.cycle++;
						b.cycleTime = Time::GetTime();
					}
				}
			}
		}
	}

	void ParticleSystem::Clear() {
		positions.clear();
		colors.clear();
		sizes.clear();

		for (Particle* p : particles) {
			delete (p);
			p = nullptr;
		}
		particles.clear();
	}

	void ParticleSystem::LoadBuffers() {
		// load vertices
		VAO->Use();

		if (last_positions != positions) {
			VAO->Load<glm::vec3>("positions", positions);
			last_positions.swap(positions);
		}

		if (last_colors != colors) {
			VAO->Load<glm::vec4>("colors", colors);
			last_colors.swap(colors);
		}

		if (last_sizes != sizes) {
			VAO->Load<glm::vec2>("sizes", sizes);
			last_sizes.swap(sizes);
		}

		VAO->Leave();
	}

	bool ParticleSystem::AddParticle(Particle* particle_) {
		if (particles.size() < maxParticles) {
			particles.push_back(particle_);
			return true;
		}
		else {
			for (Particle* p : particles) {
				if (p->age >= p->lifetime) {
					// delete(p);
					p = particle_;
					return true;
					break;
				}
			}
		}
		return false;
	}

	bool ParticleSystem::AddParticle(glm::vec3 position_, glm::vec3 velocity_, Color color_, float rotation_, glm::vec2 size_, float lifetime_) {
		if (particles.size() < maxParticles) {
			Particle* particle = new Particle(position_, velocity_, color_, rotation_, size_, lifetime_);
			particles.push_back(particle);
			return true;
		}
		else {
			for (Particle* p : particles) {
				if (p->age >= p->lifetime) {
					p->reset(position_, velocity_, color_, rotation_, size_, lifetime_);
					return true;
					break;
				}
			}
		}
		return false;
	}

	void ParticleSystem::Render() {
		if (!isReady() || isStopped) return;

		VAO->Use();
		material->shader->Use();
		material->shader->SetRenderColor(material->color);
		material->shader->SetM(transform->uM());
		material->shader->SetV(transform->uV());
		material->shader->SetP(transform->uP());
		material->update();

		VAO->GetBuffer("positions")->Bind();
		glEnablei(GL_BLEND, VAO->GetBuffer("positions")->Id);
		glDrawArrays(GL_POINTS, 0, VAO->GetBuffer("positions")->size);
		glDisablei(GL_BLEND, VAO->GetBuffer("positions")->Id);
		VAO->GetBuffer("positions")->Unbind();

		material->shader->Leave();
		VAO->Leave();
	}

	void ParticleSystem::OnDestroy() {
		if (!isReady()) return;
		VAO->Leave();
		material->shader->Leave();
		material->shader->exit();
		delete (material);
		material = nullptr;
	}

	ParticleSystem* ParticleSystem::SetPlayOnAwake(bool value_) {
		playOnAwake = value_;
		if (playOnAwake) {
			startState = Playable::State::PLAY;
		}
		return this;
	}

	ParticleSystem* ParticleSystem::SetStartDelay(float value_) {
		startDelay = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetStartLifetime(float value_) {
		startLifetime = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetStartSpeed(float value_) {
		startSpeed = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetStartSize(glm::vec2 value_) {
		startSize = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetStartColor(Color value_) {
		startColor = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetStartRotation(float value_) {
		startRotation = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetSimulationSpace(Transform::Origin value_) {
		simulationSpace = value_;
		return this;
	}

	ParticleSystem* ParticleSystem::SetEmitterVelocityMode(EmitterVelocityMode value_) {
		emitterVelocityMode = value_;
		return this;
	}

}  // namespace se
