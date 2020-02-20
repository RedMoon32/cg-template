#include "draw_line.h"


#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>


LineDrawing::LineDrawing(unsigned short width, unsigned short height): ColorSpace(width, height)
{

}

LineDrawing::~LineDrawing()
{
}

void LineDrawing::DrawLine(unsigned short x_begin, unsigned short y_begin, unsigned short x_end, unsigned short y_end, color color)
{
	bool steep = abs(x_end - x_begin) < abs(y_end - y_begin);
	
	if (steep)
	{
		std::swap(y_begin, x_begin);
		std::swap(y_end, x_end);
	}
	
	if (x_begin > x_end)
	{
		std::swap(x_begin, x_end);
		std::swap(y_begin, y_end);
	}

	float y = y_begin;
	float dx = x_end - x_begin;
	float dy = fabs(y_end - y_begin);
	float error = dx / 2.0;

	short value = y_begin > y_end ? -1 : 1;
	for (unsigned short x = x_begin; x <= x_end; x++) {

		if (steep)
			SetPixel(y, x, color);
		else 
			SetPixel(x, y, color);

		error -= dy;

		if (error < 0) {
			y += value;
			error += dx;
		}
	}
}

void LineDrawing::DrawScene()
{
	int xcenter = width / 2;
	int ycenter = height / 2;
	int radius = std::min(xcenter, ycenter) - 50;
	for (float angle = 0.0; angle < 360.0; angle += 5.0)
	{
		auto rotate = angle * M_PI / 180.0;
		DrawLine(xcenter, ycenter, xcenter + radius * std::cos(rotate), ycenter + radius * std::sin(rotate), color(255 * sin(rotate), 255 * cos(rotate), 255));
	}
}

