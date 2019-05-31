#include "sepch.h"

#include "Color.h"
#include "Utility.h"

namespace se {

	Color::Color() {
		Set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Color::Color(float r_, float g_, float b_, float a_) {
		Set(r_, g_, b_, a_);
	}

	Color::Color(int r_, int g_, int b_, float a_) {
		Set(r_, g_, b_, a_);
	}

	Color::Color(std::string hexcode) {
		glm::vec3 col = hex2rgb(hexcode);
		Set(col.r, col.g, col.b, 1.0f);
	}

	void Color::Set(float r_, float g_, float b_, float a_) {
		r = r_;
		g = g_;
		b = b_;
		a = a_;
		Clamp();
	}

	void Color::Set(int r_, int g_, int b_, float a_) {
		r = r_ / 255.0f;
		g = g_ / 255.0f;
		b = b_ / 255.0f;
		a = a_;
		Clamp();
	}

	void Color::Clamp() {
		r = glm::clamp(r, 0.0f, 1.0f);
		g = glm::clamp(g, 0.0f, 1.0f);
		b = glm::clamp(b, 0.0f, 1.0f);
		a = glm::clamp(a, 0.0f, 1.0f);
	}

	glm::vec4 Color::rgba() {
		Clamp();
		return { r, g, b, a };
	}

	glm::vec3 Color::rgb() {
		Clamp();
		return { r, g, b };
	}

	Color Color::grayscale() {
		float intensity = (r + g + b) / 3;
		return Color(intensity, intensity, intensity, a);
	}

	float Color::maxColorComponent() {
		Clamp();
		return glm::max(glm::max(r, g), b);
	}

	glm::vec3 Color::hex2rgb(std::string hexcode) {
		hexcode.erase(std::remove(hexcode.begin(), hexcode.end(), '#'), hexcode.end());

		int hexValue = std::stoi(hexcode, 0, 16);

		glm::vec3 rgbColor;
		rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0f;  // Extract the RR byte
		rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0f;  // Extract the GG byte
		rgbColor.b = ((hexValue) & 0xFF) / 255.0f;  // Extract the BB byte

		return rgbColor;
	}

	std::string Color::ToString() {
		return "(" + ts(r) + "," + ts(g) + "," + ts(b) + "," + ts(a) + ")";
	}

	bool Color::operator==(Color& other) {
		Clamp();
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	void Color::operator=(const Color& other) {
		Set(other.r, other.g, other.b, other.a);
	}

	Color Color::operator*(const Color& other) {
		return Color(
			glm::clamp(r * other.r, 0.0f, 1.0f),
			glm::clamp(g * other.g, 0.0f, 1.0f),
			glm::clamp(b * other.b, 0.0f, 1.0f),
			glm::clamp(a * other.a, 0.0f, 1.0f));
	}

	Color Color::operator*(const float value) {
		return Color(
			glm::clamp(r * value, 0.0f, 1.0f),
			glm::clamp(g * value, 0.0f, 1.0f),
			glm::clamp(b * value, 0.0f, 1.0f),
			glm::clamp(a * value, 0.0f, 1.0f));
	}

	Color Color::operator/(const Color& other) {
		return Color(
			glm::clamp(r / other.r, 0.0f, 1.0f),
			glm::clamp(g / other.g, 0.0f, 1.0f),
			glm::clamp(b / other.b, 0.0f, 1.0f),
			glm::clamp(a / other.a, 0.0f, 1.0f));
	}

	Color Color::operator/(const float value) {
		return Color(
			glm::clamp(r / value, 0.0f, 1.0f),
			glm::clamp(g / value, 0.0f, 1.0f),
			glm::clamp(b / value, 0.0f, 1.0f),
			glm::clamp(a / value, 0.0f, 1.0f));
	}

	Color Color::operator+(const Color& other) {
		return Color(
			glm::clamp(r + other.r, 0.0f, 1.0f),
			glm::clamp(g + other.g, 0.0f, 1.0f),
			glm::clamp(b + other.b, 0.0f, 1.0f),
			glm::clamp(a + other.a, 0.0f, 1.0f));
	}

	Color Color::operator+(const float value) {
		return Color(
			glm::clamp(r + value, 0.0f, 1.0f),
			glm::clamp(g + value, 0.0f, 1.0f),
			glm::clamp(b + value, 0.0f, 1.0f),
			glm::clamp(a + value, 0.0f, 1.0f));
	}

	Color& Color::operator+=(Color const& other) & {
		Set(
			glm::clamp(r + other.r, 0.0f, 1.0f),
			glm::clamp(g + other.g, 0.0f, 1.0f),
			glm::clamp(b + other.b, 0.0f, 1.0f),
			glm::clamp(a + other.a, 0.0f, 1.0f));
		return *this;
	}

	Color Color::operator-(const Color& other) {
		return Color(
			glm::clamp(r - other.r, 0.0f, 1.0f),
			glm::clamp(g - other.g, 0.0f, 1.0f),
			glm::clamp(b - other.b, 0.0f, 1.0f),
			glm::clamp(a - other.a, 0.0f, 1.0f));
	}

	Color Color::operator-(const float value) {
		return Color(
			glm::clamp(r - value, 0.0f, 1.0f),
			glm::clamp(g - value, 0.0f, 1.0f),
			glm::clamp(b - value, 0.0f, 1.0f),
			glm::clamp(a - value, 0.0f, 1.0f));
	}

	Color& Color::operator-=(Color const& other) & {
		Set(
			glm::clamp(r - other.r, 0.0f, 1.0f),
			glm::clamp(g - other.g, 0.0f, 1.0f),
			glm::clamp(b - other.b, 0.0f, 1.0f),
			glm::clamp(a - other.a, 0.0f, 1.0f));
		return *this;
	}

}  // namespace se
