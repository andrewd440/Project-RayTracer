#pragma once

/* Class for representing and manipulating RGB values */
struct Color
{
	Color() = default;
	Color(unsigned int R, unsigned int G, unsigned int B)
		: r(R), g(G), b(B) {}

	unsigned int r;
	unsigned int g;
	unsigned int b;
};

/**
* Performs component-wise multiplication of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
Color operator*(const Color& lhs, const Color& rhs)
{
	Color result;
	result.r = lhs.r * rhs.r / 255;
	result.g = lhs.g * rhs.g / 255;
	result.b = lhs.b * rhs.b / 255;
	return result;
}

