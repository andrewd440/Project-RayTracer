#pragma once

#include <algorithm>
#include <cstdint>
#include <stdexcept>

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

	/**
	* Checks for color equality.
	*/
	bool operator==(const Color& rhs) const;

	/**
	* Checks for color inequality.
	*/
	bool operator!=(const Color& rhs) const;

	/**
	* Overload of subscript operators for R = 0, G = 1, B = 2.
	*/
	float& operator[](std::size_t idx);

	/**
	* Overload of subscript operators for R = 0, G = 1, B = 2.
	*/
	const float& operator[](std::size_t idx) const;

	float r;
	float g;
	float b;

	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color White;
	static const Color Black;
};

/////////////////////////////////////////////////////////////////////
//////////////// Inlined Member Functions ///////////////////////////
/////////////////////////////////////////////////////////////////////

inline Color::Color()
	: r(0), g(0), b(0)
{

}

inline Color::Color(float R, float G, float B)
	: r(R), g(G), b(B)
{

}


inline Color& Color::operator+=(const Color& rhs)
{
	r = std::min(r + rhs.r, 1.f);
	g = std::min(g + rhs.g, 1.f);
	b = std::min(b + rhs.b, 1.f);

	return *this;
}


inline Color& Color::operator*=(const Color& rhs)
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;

	return *this;
}

inline Color& Color::operator*=(const float& scalar)
{
	r = std::min(r * scalar, 1.f);
	g = std::min(g * scalar, 1.f);
	b = std::min(b * scalar, 1.f);

	return *this;
}

inline bool Color::operator==(const Color& rhs) const
{
	for (size_t i = 0; i < 3; i++)
		if ((*this)[i] != rhs[i])
			return false;
	return true;
}

inline bool Color::operator!=(const Color& rhs) const
{
	return !(*this == rhs);
}

inline float& Color::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	default:
		throw std::out_of_range("Color subscript out of range.");
	}
}

inline const float& Color::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	default:
		throw std::out_of_range("Color subscript out of range.");
	}
}

/////////////////////////////////////////////////////////////////////
//////////////// Non-member Functions ///////////////////////////////
/////////////////////////////////////////////////////////////////////

/**
* Performs component-wise multiplication of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
inline Color operator*(Color lhs, const Color& rhs)
{
	lhs *= rhs;
	return lhs;
}

/**
* Performs scalar multiplication of a color
* @param lhs - Left operand
* @param scalar - Right operand
* @return Resulting color
*/
inline Color operator*(Color lhs, const float& scalar)
{
	lhs *= scalar;
	return lhs;
}

/**
* Performs component-wise addition of colors
* @param lhs - Left operand
* @param rhs - Right operand
* @return Resulting color
*/
inline Color operator+(Color lhs, const Color& rhs)
{
	lhs += rhs;
	return lhs;
}