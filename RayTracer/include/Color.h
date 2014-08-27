#pragma once

#include <algorithm>
#include <cstdint>

/* Class for representing and manipulating RGB values */
class Color
{
public:
	/**
	* Default constructor. Creates a black color.
	*/
	Color();

	/**
	* Constructs a color with given r, g, b values.
	* @param R - Red component
	* @param G - Green component
	* @param B - Blue component
	*/
	Color(float R, float G, float B);

	/**
	* Performs component-wise addition of colors.
	* @param rhs - Color to be added
	* @return The resulting color
	*/
	Color& operator+=(const Color& rhs);

	/**
	* Performs component-wise multiplication of colors.
	* @param rhs - Color to be multiplied
	* @return The resulting color
	*/
	Color& operator*=(const Color& rhs);

	/**
	* Performs component-wise multiplication of with a scalar.
	* @param scalar - Unit to multiply by
	* @return The resulting color
	*/
	Color& operator*=(const float& scalar);

	float r;
	float g;
	float b;

	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color White;
	static const Color Black;
};

/**
* Performs component-wise multiplication of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
Color operator*(Color lhs, const Color& rhs);

/**
* Performs scalar multiplication of a color
* @param lhs - Left operand
* @param scalar - Right operand
* @return Resulting color
*/
Color operator*(Color lhs, const float& scalar);

/**
* Performs component-wise addition of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
Color operator+(Color lhs, const Color& rhs);