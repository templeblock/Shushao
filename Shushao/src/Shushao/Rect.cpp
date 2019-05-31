#include "sepch.h"

#include "Rect.h"

namespace se {

	Rect::Rect() {
		Set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Rect::Rect(float _x, float _y, float _w, float _h) {
		Set(_x, _y, _w, _h);
	}

	Rect Rect::Copy() {
		Rect c{ x, y, width, height };
		return c;
	}

	std::string Rect::ToString() {
		std::ostringstream stream;

		stream << "\n*** Rect Info Dump ***"
			<< "\n\n";

		stream << " - x \t\t= " << x << "\n";
		stream << " - y \t\t= " << y << "\n";
		stream << " - width \t= " << width << "\n";
		stream << " - height \t= " << height << "\n";
		stream << " - xMin \t= " << xMin << "\n";
		stream << " - xMax \t= " << xMax << "\n";
		stream << " - yMin \t= " << yMin << "\n";
		stream << " - yMax \t= " << yMax << "\n";
		stream << " - center \t= " << center.x << " " << center.y << "\n";
		stream << " - size \t= " << size.x << " " << size.y << "\n";
		stream << " - position \t= " << position.x << " " << position.y << "\n";
		stream << " - max \t\t= " << max.x << " " << max.y << "\n";
		stream << " - min \t\t= " << min.x << " " << min.y << "\n";

		return stream.str();
	}

	void Rect::Set(float _x, float _y, float _w, float _h) {
		x = _x;
		y = _y;

		position = { _x, _y };
		size = { _w, _h };

		height = _h;
		width = _w;

		xMax = x + width;
		xMin = x;

		if (YUP) {
			yMax = y;
			yMin = y - height;

			topleft = { x, y };
			topright = { xMax, y };
			bottomleft = { x, yMin };
			bottomright = { xMax, yMin };
			top = { center.x, yMax };
			bottom = { center.x, yMin };

			center = { x + (width / 2), y - (height / 2) };

		}
		else {
			yMax = y + height;
			yMin = y;

			topleft = { x, y };
			topright = { xMax, yMin };
			bottomleft = { xMin, yMax };
			bottomright = { xMax, yMax };
			top = { center.x, yMin };
			bottom = { center.x, yMax };

			center = { x + (width / 2), y + (height / 2) };
		}

		max = { xMax, yMax };
		min = { xMin, yMin };

		left = { xMin, center.y };
		right = { xMax, center.y };
	}

	void Rect::SetX(float _x) {
		Set(_x, y, width, height);
	}

	void Rect::SetY(float _y) {
		Set(x, _y, width, height);
	}

	void Rect::SetWidth(float _w) {
		Set(x, y, _w, height);
	}

	void Rect::SetHeight(float _h) {
		Set(x, y, width, _h);
	}

	void Rect::SetXMax(float _x) {
		Set(x, y, (_x - x), height);
	}

	void Rect::SetYMax(float _y) {
		Set(x, (_y - y), x, height);
	}

	void Rect::SetXMin(float _x) {
		Set(_x, y, (xMax - _x), height);
	}

	void Rect::SetYMin(float _y) {
		Set(x, _y, width, (yMax - _y));
	}

	void Rect::SetPosition(glm::vec2 _p) {
		Set(_p.x, _p.y, width, height);
	}

	void Rect::SetCenter(glm::vec2 _p) {
		Set(_p.x - (width / 2), _p.y - (height / 2), width, height);
	}

	void Rect::SetSize(glm::vec2 _p) {
		Set(x, y, _p.x, _p.y);
	}

	void Rect::SetMax(glm::vec2 _max) {
		Set(x, y, (_max.x - x), (_max.y - y));
	}

	void Rect::SetMin(glm::vec2 _min) {
		Set(_min.x, _min.y, (xMax - _min.x), (yMax - _min.y));
	}

	std::vector<glm::vec2> Rect::GetVertices() {
		return { topleft, topright, bottomright, bottomleft };
	}

	std::vector<glm::vec3> Rect::GetVertices3D() {
		return {
			glm::vec3(topleft, 0),
			glm::vec3(topright, 0),
			glm::vec3(bottomright, 0),
			glm::vec3(bottomleft, 0) };
	}

	float Rect::GetAspect() {
		return width / height;
	}

	Rect Rect::operator*(const float& scalar_) const {
		return Rect(x * scalar_, y * scalar_, width * scalar_, height * scalar_);
	}

	Rect Rect::zero = { 0.0f, 0.0f, 0.0f, 0.0f };

}  // namespace se
