#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

namespace se {

class Curve {
public:
    enum class WrapMode {
        ONCE,
        LOOP,
        PINGPONG,
        CLAMP
    };

    struct Point {
        float time, value, inTangent, outTangent;

        Point(float time_ = 0.0f, float value_ = 0.0f, float inTangent_ = 0.0f, float outTangent_ = 0.0f) {
            time = glm::clamp(time_, 0.0f, 1.0f);
            value = glm::clamp(value_, 0.0f, 1.0f);
            inTangent = inTangent_;
            outTangent = outTangent_;
        }

        template <typename T>
        T vertex() {
            T v;
            v.x = time;
            v.y = value;
            return v;
        }
    };

    Curve();
    Curve(std::vector<Curve::Point> points_);

    Curve& operator=(const Curve& other) {
        _points = other.points;
        _length = other.length;
        wrapMode = other.wrapMode;
        return *this;
    }

    std::vector<Curve::Point>& points = _points;  // All points defined in the animation curve.
    unsigned int& length = _length;  // The number of points in the curve.(Read Only)
    WrapMode wrapMode = WrapMode::ONCE;

    Point operator[](int index);

    int AddPoint(float time, float value);  // Add a new key to the curve.
    int AddPoint(float time, float value, float tangent);
    int AddPoint(float time, float value, float inTangent, float outTangent);
    int MovePoint(int index, Point key);  // Removes the keyframe at index and inserts key.
    void RemovePoint(int index);
    float Evaluate(float time);  // Evaluate the curve at time.

    // static methods
    static float hermite(const float& time, const Point& start, const Point& end);
    static Curve Constant(float timeStart, float timeEnd, float value);
    static Curve EaseInOut(float timeStart, float valueStart, float timeEnd, float valueEnd);
    static Curve Linear(float timeStart, float valueStart, float timeEnd, float valueEnd);

private:
    std::vector<Point> _points;  // All points defined in the animation curve.
    unsigned int _length = 0;  // The number of points in the curve.(Read Only)

    void sortPoints();
};

}  // namespace se