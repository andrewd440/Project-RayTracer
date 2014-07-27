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

