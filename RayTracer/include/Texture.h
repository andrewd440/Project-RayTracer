#pragma once

#include "Color.h"
#include <string>
#include <vector>

class FTexture
{
public:
	FTexture(const std::string& Filename);
	~FTexture();

	/** Horizontal resolution of the texture image */
	uint32_t GetWidth() const;
	
	/** Vertical resolution of the texture image */
	uint32_t GetHeight() const;

	/**
	* Retrieves the color of a pixel in the texture image.
	* @param X coordinate
	* @param Y coordinate
	* @param Color of the pixel
	*/
	FColor GetPixel(uint32_t X, uint32_t Y) const;

	/**
	* Retrieves the color sample from UV coordinates for the texture.
	* @param U value
	* @param V value
	* @param Color of the sample
	*/
	FColor GetSample(float U, float V) const;

	friend static bool ReadTGAImage(FTexture& Texture, const std::string& Filename);

private:
	std::vector<FColor> mPixels;
	uint32_t mWidth;
	uint32_t mHeight;
};

