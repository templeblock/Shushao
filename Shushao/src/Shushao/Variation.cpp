#include "sepch.h"

#include "Variation.h"
#include "Debug.h"

namespace se {

	Variation::Variation() {
		_constant = 0.0f;
		_mode = VariationMode::CONSTANT;
	}

	Variation::Variation(float constant_) {
		Set(constant_);
	}

	Variation::Variation(float min_, float max_) {
		Set(min_, max_);
	}

	Variation::Variation(float scale_, Curve& curve_) {
		Set(scale_, curve_);
	}

	void Variation::Set(float constant_) {
		_constant = constant_;
		_mode = VariationMode::CONSTANT;
	}

	void Variation::Set(float min_, float max_) {
		_min = min_;
		_max = max_;
		_mode = VariationMode::RANGE;
	}

	void Variation::Set(float scale_, Curve& curve_) {
		_curveScale = scale_;
		_curve = curve_;
		_mode = VariationMode::CURVE;
	}

	void Variation::operator=(float constant_) {
		_constant = constant_;
		_mode = VariationMode::CONSTANT;
	}

	float Variation::Evaluate(float x) {
		float result;
		switch (mode) {
		case VariationMode::CONSTANT:
			return constant;
			break;
		case VariationMode::RANGE:
			result = x * (max - min);
			break;
		case VariationMode::CURVE:
			result = _curve.Evaluate(x) * curveScale;
			break;
		}
		return result;
	}
}  // namespace se