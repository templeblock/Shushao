#pragma once

#include "Shushao/Core.h"
#include "Color.h"
#include "curve.h"
#include "sepch.h"

namespace se {

class Gradient {
public:
    enum class GradientMode {
        CURVE,
        RANGE
    };

    typedef struct {
        Color start;
        Color end;
    } Range;

    typedef struct {
        Color start;
        Color end;
        Curve curve;
    } CurveDef;

    // readonly
    const GradientMode& mode = _mode;
    const Color& start = _start;
    const Color& end = _end;
    const Curve& curve = _curve;

    Gradient();
    Gradient(Gradient& other);
    Gradient(Color start_, Color end_);
    Gradient(Color start_, Color end_, Curve& curve_);

    void Set(Color start_, Color end_);
    void Set(Color start_, Color end_, Curve& curve_);

    Gradient operator=(Gradient& other);
    /* void operator=(Range range_);
    void operator=(CurveDef curvedef_); */

    Color Evaluate(float x);

private:
    Color _start;
    Color _end;
    Curve _curve;

    GradientMode _mode = GradientMode::RANGE;
};

}  // namespace se