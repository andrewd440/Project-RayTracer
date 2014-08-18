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
	Color(uint32_t R, uint32_t G, uint32_t B);

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

	uint32_t r;
	uint32_t g;
	uint32_t b;

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