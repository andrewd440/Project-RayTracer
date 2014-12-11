#pragma once
#include "Color.h"
#include "Vector2.h"

#include <vector>
#include <fstream>
#include <cstdint>

/* Used to write to a .ppm image file */
class FImage
{
public:
	/**
	* Constructs an Image with a filename and output resolution in pixels
	* @param Filename - Name of the output image file
	* @param OutputResolution - Resolution (in pixels) of the output image
	*/
	FImage(const std::string& Filename, const Vector2i& OutputResolution);

	/**
	* Sets the color value of a pixel.
	* @param X - x coordinate of the pixel
	* @param Y - y coordinate of the pixel
	* @param Color - The color of the pixel
	*/
	void setPixel(const int32_t& X, const int32_t& Y, const FColor& Color);

	/**
	* Writes a .ppm image file with the rgb values currently stored for
	* each pixel.
	*/
	void writeImage();

	/**
	* Sets the filename for the output image file.
	* @param Filename - Name of the output file
	*/
	void setFilename(const std::string& Filename);

	/**
	* Retrieves the filename for the output image file.
	* @return String containing the filename
	*/
	std::string getFilename() const;

private:
	/* Color values for each pixel. Indexed by [HorizontalPosition][VerticalPosition] */
	std::vector<std::vector<FColor>> mPixelColors;

	std::string mFilename; /* Filename for the output image file */
	Vector2i mOutputResolution; /* Resolution of the output image file */
};

