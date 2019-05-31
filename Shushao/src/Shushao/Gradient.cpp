#include "sepch.h"

#include "Gradient.h"
#include "Debug.h"

namespace se {

	Gradient::Gradient() {}

	Gradient::Gradient(Gradient& other) {
		_start = other.start;
		_end = other.end;
		_curve = other.curve;
		_mode = other.mode;
	}

	Gradient::Gradient(Color start_, Color end_) {
		Set(start_, end_);
	}

	Gradient::Gradient(Color start_, Color end_, Curve& curve_) {
		Set(start_, end_, curve_);
	}

	void Gradient::Set(Color start_, Color end_) {
		_start = start_;
		_end = end_;
		_mode = GradientMode::RANGE;
	}

	void Gradient::Set(Color start_, Color end_, Curve& curve_) {
		_start = start_;
		_end = end_;
		_curve = curve_;
		_mode = GradientMode::CURVE;
	}

	Gradient Gradient::operator=(Gradient& other) {
		_start = other.start;
		_end = other.end;
		_curve = other.curve;
		_mode = other.mode;
		return *this;
	}

	Color Gradient::Evaluate(float x) {
		Color result;
		switch (mode) {
		case GradientMode::RANGE:
			result.Set(
				_start.r + (_end.r - _start.r) * x,
				_start.g + (_end.g - _start.g) * x,
				_start.b + (_end.b - _start.b) * x,
				_start.a + (_end.a - _start.a) * x);
			break;
		case GradientMode::CURVE:
			float evalx = _curve.Evaluate(x);
			result.Set(
				_start.r + (_end.r - _start.r) * evalx,
				_start.g + (_end.g - _start.g) * evalx,
				_start.b + (_end.b - _start.b) * evalx,
				_start.a + (_end.a - _start.a) * evalx);
			break;
		}
		return result;
	}
}  // namespace se