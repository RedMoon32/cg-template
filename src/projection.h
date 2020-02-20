#pragma once


#include "read_obj.h"


class Projection: public ReadObj
{
public:
	Projection(unsigned short width, unsigned short height, std::string obj_file);
	virtual ~Projection();

	void DrawScene();
	void DrawTriangle(float4 cords[3], color col);
};
