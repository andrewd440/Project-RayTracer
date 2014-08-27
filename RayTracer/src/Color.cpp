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

Color::Color(float R, float G, float B)
		: r(R), g(G), b(B) 
{

}


Color& Color::operator+=(const Color& rhs)
{
	r = std::min(r + rhs.r, 1.f);
	g = std::min(g + rhs.g, 1.f);
	b = std::min(b + rhs.b, 1.f);

	return *this;
}


Color& Color::operator*=(const Color& rhs)
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;

	return *this;
}

Color& Color::operator*=(const float& scalar)
{
	r = std::min(r * scalar, 1.f);
	g = std::min(g * scalar, 1.f);
	b = std::min(b * scalar, 1.f);

	return *this;
}


Color operator*(Color lhs, const Color& rhs)
{
	lhs *= rhs;
	return lhs;
}


Color operator*(Color lhs, const float& scalar)
{
	lhs *= scalar;
	return lhs;
}

Color operator+(Color lhs, const Color& rhs)
{
	lhs += rhs;
	return lhs;
}
