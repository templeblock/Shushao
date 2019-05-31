#pragma once

#include "Core.h"
#include "sepch.h"

namespace se {

class Rect {
public:
    Rect();
    Rect(float, float, float, float);

    std::string name = "";
    bool YUP = false;

    static Rect zero;

    static Rect MinMaxRect(float, float, float, float);  // Creates a rectangle from min/max coordinate values.
    static glm::vec2 NormalizedToPoint(Rect, glm::vec2);  // Returns a point inside a rectangle, given normalized coordinates.
    static glm::vec2 PointToNormalized(Rect, glm::vec2);  // Returns the normalized coordinates cooresponding the the point.

    glm::vec2 center;  // The position of the center of the rectangle.
    float height;  // The height of the rectangle, measured from the Y position.
    glm::vec2 max;  // The position of the maximum corner of the rectangle.
    glm::vec2 min;  // The position of the minimum corner of the rectangle.
    glm::vec2 position;  // The X and Y position of the rectangle.
    glm::vec2 size;  // The width and height of the rectangle.
    float width;  // The width of the rectangle, measured from the X position.
    float x;  // The X coordinate of the rectangle.
    float xMax;  // The maximum X coordinate of the rectangle.
    float xMin;  // The minimum X coordinate of the rectangle.
    float y;  // The Y coordinate of the rectangle.
    float yMax;  // The maximum Y coordinate of the rectangle.
    float yMin;  // The minimum Y coordinate of the rectangle.

    glm::vec2 topleft;
    glm::vec2 topright;
    glm::vec2 bottomleft;
    glm::vec2 bottomright;
    glm::vec2 left;
    glm::vec2 right;
    glm::vec2 top;
    glm::vec2 bottom;

    bool Contains(glm::vec2);  // Returns true if the x and y components of point is a point inside this rectangle. If allowInverse is present and true, the width and height of the Rect are allowed to take negative values (ie, the min value is greater than the max), and the test will still work.
    bool Contains(glm::vec3);

    bool Overlaps(Rect);  // Returns true if the other rectangle overlaps this one. If allowInverse is present and true, the widths and heights of the Rects are allowed to take negative values (ie, the min value is greater than the max), and the test will still work.
    void Set(float, float, float, float);  // Set components of an existing Rect.

    Rect Copy();

    std::string ToString();

    bool operator==(Rect other) {
        return (GetVertices() == other.GetVertices());
    }

    bool operator!=(Rect other) {
        return (GetVertices() != other.GetVertices());
    }

    /* Rect operator*(const float value) {
				Rect result(x * value, y * value, width * value, height * value);
				return result;
			} */

    void SetX(float);
    void SetY(float);
    void SetWidth(float);
    void SetHeight(float);
    void SetXMax(float);
    void SetYMax(float);
    void SetXMin(float);
    void SetYMin(float);
    void SetPosition(glm::vec2);
    void SetCenter(glm::vec2);
    void SetSize(glm::vec2);
    void SetMax(glm::vec2);
    void SetMin(glm::vec2);

    Rect operator*(const float& scalar_) const;

    std::vector<glm::vec2> GetVertices();
    std::vector<glm::vec3> GetVertices3D();
    float GetAspect();

protected:
private:
};

}  // namespace se
