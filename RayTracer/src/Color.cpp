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

Color::Color(unsigned int R, unsigned int G, unsigned int B)
		: r(R), g(G), b(B) 
{

}


Color& Color::operator+=(const Color& rhs)
{
	r = std::min(r + rhs.r, (unsigned)255);
	g = std::min(g + rhs.g, (unsigned)255);
	b = std::min(b + rhs.b, (unsigned)255);

	return *this;
}


Color& Color::operator*=(const Color& rhs)
{
	r = std::min(r + rhs.r, (unsigned)255);
	g = std::min(g + rhs.g, (unsigned)255);
	b = std::min(b + rhs.b, (unsigned)255);

	return *this;
}

Color& Color::operator*=(const float& scalar)
{
	r = std::min(unsigned(r * scalar), (unsigned)255);
	g = std::min(unsigned(g * scalar), (unsigned)255);
	b = std::min(unsigned(b * scalar), (unsigned)255);

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
	result.r = std::min(unsigned(lhs.r * scalar), (unsigned)255);
	result.g = std::min(unsigned(lhs.g * scalar), (unsigned)255);
	result.b = std::min(unsigned(lhs.b * scalar), (unsigned)255);
	return result;
}

Color operator+(const Color& lhs, const Color& rhs)
{
	Color result;
	result.r = std::min(lhs.r + rhs.r, (unsigned)255);
	result.g = std::min(lhs.g + rhs.g, (unsigned)255);
	result.b = std::min(lhs.b + rhs.b, (unsigned)255);
	return result;
}
