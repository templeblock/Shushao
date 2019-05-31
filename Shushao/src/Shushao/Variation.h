#ifndef SE_VARIATION
#define SE_VARIATION

#include "Core.h"
#include "sepch.h"

#include "curve.h"

namespace se {

class Variation {
public:
    enum class VariationMode {
        CONSTANT,
        CURVE,
        RANGE
    };

    // readonly
    const VariationMode& mode = _mode;
    const float& constant = _constant;
    const float& min = _min;
    const float& max = _max;
    const float& curveScale = _curveScale;
    const Curve& curve = _curve;

    Variation();
    Variation(float constant_);
    Variation(float min_, float max_);
    Variation(float value_, Curve& curve_);

    void Set(float constant_);
    void Set(float min_, float max_);
    void Set(float scale_, Curve& curve_);

    void operator=(float constant_);

    float Evaluate(float x);

private:
    float _constant;
    float _min;
    float _max;
    Curve _curve;

    //Curve _curveMin;
    //Curve _curveMax;
    float _curveScale;
    VariationMode _mode = VariationMode::CONSTANT;
};

}  // namespace se

#endif