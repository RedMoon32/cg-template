#include "read_obj.h"


#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>

#include <algorithm>


ObjParser::ObjParser(std::string filename) :
	filename(filename)
{
}


ObjParser::~ObjParser()
{

}

int ObjParser::Parse()
{
	std::ifstream obj_file(filename, std::ifstream::in);
	if (obj_file.fail())
	{
		return 1;
	}

	std::string line;

	while (std::getline(obj_file, line))
	{
		if (line.rfind("v ", 0) == 0)
		{
			// Process vertex
			std::vector<std::string> vertexData = Split(line, ' ');
			// 4 dimensional vector xyzw
			float4 vertex{ 0, 0, 0, 1 };
			// i = 1 - skip 'v' symbol
			for (int i = 1; i < vertexData.size(); i++)
			{
				// convert to int and put coordinate to vector
				vertex[i - 1] = std::stof(vertexData.at(i));
			}
			vertexes.push_back(vertex);
		}
		else if (line.rfind("f ", 0) == 0)
		{
			// Process faces
			std::vector<std::string> facesData = Split(line, ' ');
			std::vector<int> indexes;
			for (int i = 1; i < facesData.size(); i++)
			{
				std::vector<std::string> indexData = Split(facesData.at(i), '/');
				int index = std::stoi(indexData.at(0));
				if (index > 0)
				{
					index -= 1;
				}
				else {
					index += vertexes.size();
				}
				indexes.push_back(index);
			}
			for (int i = 0; i < indexes.size() - 2; i++)
			{
				face face;
				face.vertexes[0] = vertexes[indexes.at(0)];
				face.vertexes[1] = vertexes[indexes.at(i + 1)];
				face.vertexes[2] = vertexes[indexes.at(i + 2)];
				faces.push_back(face);
			}

		}
	}

	return 0;
}

const std::vector<face>& ObjParser::GetFaces()
{
	return faces;
}

std::vector<std::string> ObjParser::Split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		if (std::isdigit(token[0]) || std::isalpha(token[0]) || token[0] == '-')
		{
			tokens.push_back(token);
		}
	}
	return tokens;
}


ReadObj::ReadObj(unsigned short width, unsigned short height, std::string obj_file) : LineDrawing(width, height)
{
	parser = new ObjParser(obj_file);
}

ReadObj::~ReadObj()
{
	delete parser;
}

void ReadObj::DrawScene()
{
	parser->Parse();

	int xcenter = width / 2;
	int ycenter = height / 2;
	unsigned short radius = std::min(xcenter, ycenter) - 40.0;

	for (auto face : parser->GetFaces())
	{
		DrawLine(xcenter + radius * face.vertexes[0].x,
			ycenter + radius * face.vertexes[0].y,
			xcenter + radius * face.vertexes[1].x,
			ycenter + radius * face.vertexes[1].y,
			color(255, 0, 0));

		DrawLine(xcenter + radius * face.vertexes[1].x,
			     ycenter + radius * face.vertexes[1].y,
			     xcenter + radius * face.vertexes[2].x,
			     ycenter + radius * face.vertexes[2].y,
			     color(0, 255, 0));

		DrawLine(xcenter + radius * face.vertexes[2].x,
			     ycenter + radius * face.vertexes[2].y,
			     xcenter + radius * face.vertexes[0].x,
			     ycenter + radius * face.vertexes[0].y,
			color(0, 0, 255));
	}
}