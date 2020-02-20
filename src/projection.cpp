#include "projection.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


Projection::Projection(unsigned short width, unsigned short height, std::string obj_file) : ReadObj(width, height, obj_file)
{
}

Projection::~Projection()
{
}

void Projection::DrawScene()
{
	//read obj
	parser->Parse();

	float angle = 0;

	float3 eye{ 0,0,0 };
	float3 look_at{ 0,0,1 };
	float3 up{ 0,1,0 };

	float3 zaxis = normalize(eye - look_at);
	float3 xaxis = normalize(cross(up, zaxis));
	float3 yaxis = cross(zaxis, xaxis);


	//pay attention that each initializer list inside is a column
	float4x4 rotation
	{
			{1, 0, 0, 0},
			{0, cos(angle), sin(angle), 0},
			{0, -sin(angle), cos(angle), 0},
			{0, 0, 0, 1}
	};

	float4x4 translate
	{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 3, 1}
	};

	float4x4 world = mul(translate, rotation);

	float4x4 view{
			{xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye)},
			{yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye)},
			{zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye)},
			{0,0,0,1}
	};

	float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	float fov_y_angle = M_PI / 3;
	float y_scale = 1 / tan(fov_y_angle / 2);
	float x_scale = y_scale / aspect_ratio;
	float zFar = 10;
	float zNear = 1;

	float4x4 projection
	{
			{x_scale, 0, 0, 0},
			{0, y_scale, 0, 0},
			{0, 0, zFar / (zNear - zFar), zNear * zFar / (zNear - zFar)},
			{0, 0, 1, 0}
	};

	float4x4 translateMatrix = mul(projection, view, world);

	for (auto face : parser->GetFaces())
	{
		float4 translated[3];
		for (int i = 0; i < 3; i++)
		{
			translated[i] = mul(translateMatrix, face.vertexes[i]);
			translated[i] /= translated[i].w;
		}
		DrawTriangle(translated, color(100, 150, 50));
	}

}

void Projection::DrawTriangle(float4 cords[3], color col) {
	float x_center = width / 2;
	float y_center = height / 2;
	float radius = std::min(x_center, y_center) - 40.0f;

	DrawLine(static_cast<unsigned short>(x_center + x_center * cords[0].x),
		static_cast<unsigned short>(y_center - y_center * cords[0].y),
		static_cast<unsigned short>(x_center + x_center * cords[1].x),
		static_cast<unsigned short>(y_center - y_center * cords[1].y), col);

	DrawLine(static_cast<unsigned short>(x_center + x_center * cords[1].x),
		static_cast<unsigned short>(y_center - y_center * cords[1].y),
		static_cast<unsigned short>(x_center + x_center * cords[2].x),
		static_cast<unsigned short>(y_center - y_center * cords[2].y), col);

	DrawLine(static_cast<unsigned short>(x_center + x_center * cords[2].x),
		static_cast<unsigned short>(y_center - y_center * cords[2].y),
		static_cast<unsigned short>(x_center + x_center * cords[0].x),
		static_cast<unsigned short>(y_center - y_center * cords[0].y), col);
}

