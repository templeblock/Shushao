#include "glm/glm.hpp"

#include "hermite.h"

using namespace se;

void Hermite::Awake() {
    GeometryRenderer* gr = AddComponent<GeometryRenderer>();
    gr->sortingLayerID = Config::SortingLayers.over();

    /* Curve curve = Curve::EaseInOut(0.0f, 0.0f, 1.0f, 1.0f);

    //curve.AddPoint(0.2813312f, 0.6913764f, -1.775311f);
    //curve.AddPoint(0.7149807f, 0.04005498f, -9.476109f, 0.2281767f);

    //Curve curve;
    //curve.AddPoint(0, 1, -2, -2);
    //curve.AddPoint(1, 0, 0, 0);

    std::vector<glm::vec3> points;
    for (double d = 0.0; d <= 1.0; d += 0.001) {
        glm::vec3 point = glm::vec3(d * 6.0, curve.Evaluate(d) * 2.0f, 0.0f);
        points.push_back(point);
    }

    for (auto point : curve.points) {
        Debug::Log << "point: " << point.time << ", " << point.value << std::endl;
    }

    gr->AddPolyline(points, color::green); */

    Gradient gradient(color::red, Color(0.5f, 1.0f, 0.0f, 0.4f));

    for (double d = 0.0; d <= 1.0; d += 0.001) {
        Color color = gradient.Evaluate(d);
        gr->AddLine(glm::vec3(d * 6.0f, 0.0f, 0.0f), glm::vec3(d * 6.0f, 2.0f, 0.0f), color);
        //gr->AddLine(glm::vec3(d * 6.0f, 0.0f, 0.0f), glm::vec3(d * 6.0f, 2.0f, 0.0f), color::red);
        //Debug::Log << (d * 6.0f) << ", " << 0.0f << ", " << 0.0f << " -> " << (d * 6.0f) << ", " << 2.0f << ", " << 0.0f << std::endl;
        // Debug::Log << "r: " << color.r << "; g: " << color.g << "; b: " << color.b << "; a: " << color.a << std::endl;
    }
}