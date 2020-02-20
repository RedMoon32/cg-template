#include "color_space.h"


ColorSpace::ColorSpace(unsigned short width, unsigned short height): BlackImage(width, height)
{

}

ColorSpace::~ColorSpace()
{
}

void ColorSpace::DrawScene()
{
	for (unsigned long i = 0; i < width; i++) {
		for (unsigned long j = 0; j < height; j++)
			SetPixel(i, j, color(
				0,
				static_cast<unsigned char>(i * 255 / width),
				static_cast<unsigned char>(j * 255 / height)));
	}

}

void ColorSpace::SetPixel(unsigned short x, unsigned short y, color color)
{
	frame_buffer[y * width + x] = color;
}

