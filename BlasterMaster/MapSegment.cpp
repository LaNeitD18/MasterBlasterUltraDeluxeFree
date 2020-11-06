#include "MapSegment.h"
/*
#include <iostream>
#include <fstream>

MapSegment::MapSegment(TextureLibrary * registerInfo, int sprite_id, const char* sprite_file, const char * environmentInfo, BoundingBox & boundingBox)
{
	textureLib = registerInfo;
	this->sprite_id = sprite_id;
	this->sprite_file = sprite_file;
	this->environmentInfo = environmentInfo;
	this->boundingBox = BoundingBox(boundingBox);
}

MapSegment::~MapSegment()
{
	delete sprite;
}

void MapSegment::PartialInit(MapSegment * top, MapSegment * left, MapSegment * bottom, MapSegment * right)
{
	this->top = top;
	this->bottom = bottom;
	this->left = left;
	this->right = right;
}

void MapSegment::FullInit()
{
	if (textureLib->Get(sprite_id) == NULL)
		textureLib->Add(sprite_id, LPCWSTR(sprite_file), 
						D3DCOLOR_XRGB(255, 255, 255));
	sprite = new Sprite(sprite_id, boundingBox, textureLib->Get(sprite_id));
	
	ifstream cin(environmentInfo);
	char str[MAX_SEGMENT_SIZE];
	while (cin.getline(str, MAX_SEGMENT_SIZE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		vector<Environment*> row;
		for (char element : line)
		{
			Environment* env;
			switch (element)
			{
			default:
				break;
			}
			row.push_back(env);
		}
		environment.push_back(row);
	}
}
//
//void MapSegment::Render()
//{
//	sprite->Draw(Point());
//}

void MapSegment::Release()
{
	delete sprite;
}
//*/