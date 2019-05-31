#include "sepch.h"

#include "Light.h"
#include "Design.h"
#include "Entity.h"
#include "Transform.h"

namespace se {

	void Light::Copy(Light* other) {
		if (other == nullptr) return;
		Component::Copy(other);

		enabled = other->enabled;
		type = other->type;
		direction = other->direction;
		ambient = other->ambient;
		diffuse = other->diffuse;
		specular = other->specular;
		attenuation = other->attenuation;
		cutoff = other->cutoff;
	}

	std::string Light::GetTypeDesc() {
		switch (type) {
		case Light::Type::SPOT:
			return "Spot";
			break;
		case Light::Type::DIRECTIONAL:
			return "Directional";
			break;
		case Light::Type::POINT:
			return "Point";
			break;
		case Light::Type::RECTANGLE:
			return "Rectangle";
			break;
		case Light::Type::DISC:
			return "Disc";
			break;
		}
		return "NP";
	}

	UniformLight Light::GetUniform() {
		return {
			entity->transform->position,
			entity->transform->forward,
			ambient.rgb(),
			diffuse.rgb(),
			specular.rgb(),
			attenuation.constant,
			attenuation.linear,
			attenuation.quadratic };
	}

	void Light::Update() {
		direction = entity->transform->forward;
	}

	void Light::Render() {
		if (Debug::enabled && Debug::drawTransforms) {
			Design::DrawVector(transform->position, direction, color::red, 1, true);
		}
	}

}  // namespace se
