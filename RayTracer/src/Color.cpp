#include "Color.h"

#include <algorithm>

const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::White(255, 255, 255);
const Color Color::Black(0, 0, 0);

Color::Color()
	: r(0), g(0), b(0)
{

}

Color::Color(uint32_t R, uint32_t G, uint32_t B)
		: r(R), g(G), b(B) 
{

}


Color& Color::operator+=(const Color& rhs)
{
	r = std::min(r + rhs.r, (uint32_t)255);
	g = std::min(g + rhs.g, (uint32_t)255);
	b = std::min(b + rhs.b, (uint32_t)255);

	return *this;
}


Color& Color::operator*=(const Color& rhs)
{
	r = std::min(r + rhs.r, (uint32_t)255);
	g = std::min(g + rhs.g, (uint32_t)255);
	b = std::min(b + rhs.b, (uint32_t)255);

	return *this;
}

Color& Color::operator*=(const float& scalar)
{
	r = std::min(uint32_t(r * scalar), (uint32_t)255);
	g = std::min(uint32_t(g * scalar), (uint32_t)255);
	b = std::min(uint32_t(b * scalar), (uint32_t)255);

	return *this;
}


Color operator*(const Color& lhs, const Color& rhs)
{
	Color result;
	result.r = lhs.r * rhs.r / 255;
	result.g = lhs.g * rhs.g / 255;
	result.b = lhs.b * rhs.b / 255;
	return result;
}


Color operator*(const Color& lhs, const float& scalar)
{
	Color result;
	result.r = std::min(uint32_t(lhs.r * scalar), (uint32_t)255);
	result.g = std::min(uint32_t(lhs.g * scalar), (uint32_t)255);
	result.b = std::min(uint32_t(lhs.b * scalar), (uint32_t)255);
	return result;
}

Color operator+(const Color& lhs, const Color& rhs)
{
	Color result;
	result.r = std::min(lhs.r + rhs.r, (uint32_t)255);
	result.g = std::min(lhs.g + rhs.g, (uint32_t)255);
	result.b = std::min(lhs.b + rhs.b, (uint32_t)255);
	return result;
}
