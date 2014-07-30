#pragma once

#include <algorithm>

/* Class for representing and manipulating RGB values */
class Color
{
public:
	Color();
	Color(unsigned int R, unsigned int G, unsigned int B);

	unsigned int r;
	unsigned int g;
	unsigned int b;

	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color White;
};

/**
* Performs component-wise multiplication of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
Color operator*(const Color& lhs, const Color& rhs);

/**
* Performs scalar multiplication of a color
* @param lhs - Left operand
* @param scalar - Right operand
* @return Resulting color
*/
Color operator*(const Color& lhs, const float& scalar);

/**
* Performs component-wise addition of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
Color operator+(const Color& lhs, const Color& rhs);