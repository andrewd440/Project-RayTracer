#include "Texture.h"

#include <fstream>
#include <iostream>

FTexture::FTexture(const std::string& Filename)
{
	if (Filename.substr(Filename.length() - 4) == ".tga")
	{
		if (!ReadTGAImage(*this, Filename))
			return;
	}
	else
	{
		std::cout << Filename << " file format not supported." << std::endl;
		return;
	}
}


FTexture::~FTexture()
{
}

uint32_t FTexture::GetWidth() const
{
	return mWidth;
}

uint32_t FTexture::GetHeight() const
{
	return mHeight;
}

FColor FTexture::GetPixel(uint32_t X, uint32_t Y) const
{
	return mPixels[Y * mWidth + X];
}

FColor FTexture::GetSample(float U, float V) const
{
	return GetPixel(uint32_t(U * mWidth) % mWidth, uint32_t(V * mHeight) % mHeight);
}

/**
* Reads in image data from a .tga image file into a FTexture object.
* Image pixel data, width, and height is taken from the image.
* @param Texture object to read data into.
* @param Filename of .tga image to read.
* @return False if could not read the image.
*/
static bool ReadTGAImage(FTexture& Texture, const std::string& Filename)
{
	std::fstream InputFile(Filename, std::ios::in | std::ios::binary);
	if (!InputFile.is_open())
	{
		std::cout << "Could not open image file: " << Filename << std::endl;
		return false;
	}

	static const uint8_t UnCompressed[10] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	static const uint8_t Compressed[10] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

	uint8_t HeaderData[18] = { 0 };
	std::vector<uint8_t> ImageData;
	uint8_t BitsPerPixel;
	InputFile.read(reinterpret_cast<char*>(&HeaderData), sizeof(HeaderData));

	// read compressed file
	if (!std::memcmp(UnCompressed, &HeaderData, sizeof(UnCompressed)))
	{
		BitsPerPixel = HeaderData[16];
		Texture.mWidth = (HeaderData[13] << 8) + HeaderData[12];
		Texture.mHeight = (HeaderData[15] << 8) + HeaderData[14];
		const uint32_t& ImageSize = ((Texture.mWidth * BitsPerPixel + 31) / 32) * 4 * Texture.mHeight;

		if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
		{
			std::cout << "Invalid .tga format. Bits per pixel must be 24 or 32: " << Filename << std::endl;
			return false;
		}

		ImageData.resize(ImageSize);
		InputFile.read(reinterpret_cast<char*>(ImageData.data()), ImageSize);
	}
	// read uncompressed file
	else if (!std::memcmp(Compressed, &HeaderData, sizeof(Compressed)))
	{
		BitsPerPixel = HeaderData[16];
		// .tga is little edian, so bitshift
		Texture.mWidth = (HeaderData[13] << 8) + HeaderData[12];
		Texture.mHeight = (HeaderData[15] << 8) + HeaderData[14];
		const uint32_t& ImageSize = ((Texture.mWidth * BitsPerPixel + 31) / 32) * 4 * Texture.mHeight;

		if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
		{
			std::cout << "Invalid .tga format. Bits per pixel must be 24 or 32: " << Filename << std::endl;
			return false;
		}

		struct FPixel { uint8_t R, G, B, A; };

		FPixel Pixel;
		uint32_t CurrentByte = 0;
		size_t CurrentPixel = 0;
		uint8_t Header = { 0 };
		uint8_t BytesPerPixel = (BitsPerPixel / 8);
		ImageData.resize(Texture.mWidth * Texture.mHeight * sizeof(FPixel));

		do
		{
			InputFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

			if (Header < 128)
			{
				++Header;
				for (int i = 0; i < Header; i++, CurrentPixel++)
				{
					InputFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

					ImageData[CurrentByte++] = Pixel.B;
					ImageData[CurrentByte++] = Pixel.G;
					ImageData[CurrentByte++] = Pixel.R;

					if (BitsPerPixel > 24)
						ImageData[CurrentByte++] = Pixel.A;
				}
			}
			else
			{
				Header -= 127;
				InputFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

				for (int i = 0; i < Header; i++, CurrentPixel++)
				{
					ImageData[CurrentByte++] = Pixel.B;
					ImageData[CurrentByte++] = Pixel.G;
					ImageData[CurrentByte++] = Pixel.R;

					if (BitsPerPixel > 24)
						ImageData[CurrentByte++] = Pixel.A;
				}
			}
		} while (CurrentPixel < (Texture.mWidth * Texture.mHeight));
	}
	// error in file structure
	else
	{
		InputFile.close();
		std::cout << "Error in .tga header file: " << Filename << std::endl;
		return false;
	}

	InputFile.close();

	Texture.mPixels.clear();
	const float InvColorMax(1.0f / 255.0f);
	for (int i = 0; i < Texture.mWidth * Texture.mHeight; i++)
	{
		const uint32_t& BaseIndex = i * ((BitsPerPixel > 24) ? 4 : 3);
		const float& R = (float)ImageData[BaseIndex] * InvColorMax;
		const float& G = (float)ImageData[BaseIndex + 1] * InvColorMax;
		const float& B = (float)ImageData[BaseIndex + 2] * InvColorMax;
		Texture.mPixels.push_back(FColor(R, G, B));
	}

	return true;
}