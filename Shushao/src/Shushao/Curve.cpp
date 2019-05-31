
#include "sepch.h"

#include "Curve.h"
#include "Debug.h"

namespace se {

	Curve::Curve() {
		// nada
	}

	Curve::Curve(std::vector<Curve::Point> points_) {
		_points = points_;
	}

	Curve::Point Curve::operator[](int index) {
		return points[index];
	}

	int Curve::AddPoint(float time_, float value_) {
		Point key(time_, value_, 0.0f, 0.0f);

		// TODO: calcolo tangenti e weight automatico. è una parola

		_points.push_back(key);
		_length = (unsigned int)points.size();
		sortPoints();
		return _length - 1;
	}

	int Curve::AddPoint(float time_, float value_, float tangent_) {
		Point key(time_, value_, tangent_, tangent_);
		_points.push_back(key);
		_length = (unsigned int)points.size();
		sortPoints();
		return _length - 1;
	}

	int Curve::AddPoint(float time_, float value_, float inTangent_, float outTangent_) {
		Point key(time_, value_, inTangent_, outTangent_);
		_points.push_back(key);
		_length = (unsigned int)points.size();
		sortPoints();
		return _length - 1;
	}

	int Curve::MovePoint(int index, Point key) {
		if (index >= (int)_points.size()) return -1;
		_points[index] = key;
		sortPoints();
		return index;
	}

	void Curve::RemovePoint(int index) {
		if (index >= (int)_points.size()) return;
		_points.erase(points.begin() + index);
		_length = (unsigned int)points.size();
	}

	void Curve::sortPoints() {
		sort(points.begin(), points.end(), [](const Point& l, const Point& r) {
			return l.time < r.time;
			});
	}

	float Curve::Evaluate(float time) {
		if (points.size() < 2) return 0.0f;

		// se siamo fuori da 0-1
		if (time > 1.0f || time < 0.0f) {
			switch (wrapMode) {
			case WrapMode::ONCE:
				time = -0.999999f;
				break;
			case WrapMode::LOOP:
				time = glm::fract(time);
				break;
			case WrapMode::PINGPONG:
				time = 1.0f - glm::fract(time);
				break;
			case WrapMode::CLAMP:
				time = glm::clamp(time, 0.0f, 1.0f);
				break;
			}
		}

		Point* A;
		Point* B;

		for (size_t i = 0; i < points.size(); ++i) {
			if (points[i].time >= time) {
				A = &(points[i - 1]);
				B = &(points[i]);
				break;
			}
		}

		if (A == nullptr || B == nullptr) {
			return 0.0f;
		}

		float result = hermite((time - A->time) / (B->time - A->time), *A, *B);
		return result;
	}

	float Curve::hermite(const float& time, const Point& start, const Point& end) {
		float s2 = glm::pow2(time);
		float s3 = glm::pow3(time);
		float f1 = float(2) * s3 - float(3) * s2 + float(1);
		float f2 = float(-2) * s3 + float(3) * s2;
		float f3 = s3 - float(2) * s2 + time;
		float f4 = s3 - s2;
		return f1 * start.value + f2 * end.value + f3 * start.outTangent + f4 * end.inTangent;
	}

	Curve Curve::Constant(float timeStart, float timeEnd, float value) {
		return Linear(timeStart, value, timeEnd, value);
	}

	Curve Curve::EaseInOut(float timeStart, float valueStart, float timeEnd, float valueEnd) {
		if (timeStart == timeEnd) {
			Point key(timeStart, valueStart);
			return Curve({ key });
		}

		return Curve({ Point(timeStart, valueStart, 0.0f, 0.0f),
					  Point(timeEnd, valueEnd, 0.0f, 0.0f) });
	}

	Curve Curve::Linear(float timeStart, float valueStart, float timeEnd, float valueEnd) {
		if (timeStart == timeEnd) {
			Point key(timeStart, valueStart);
			return Curve({ key });
		}

		float tangent = (valueEnd - valueStart) / (timeEnd - timeStart);
		return Curve({ Point(timeStart, valueStart, 0.0f, tangent),
					  Point(timeEnd, valueEnd, tangent, 0.0f) });
	}

}  // namespace se
