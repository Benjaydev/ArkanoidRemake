#pragma once
#include "raylib.h"
#include "Brick.h"

struct BrickStruct
{

	int colour = (int)0xFFFFFFFF;
	int health = 1;
	bool isEmpty = true;
};


struct MapStruct
{
	int backgroundColour = (int)0x333333FF;
	BrickStruct bricks[130] = {};
};


class Map
{
public:
	Map();
	~Map();

	std::string mapName = "New Level";
	MapStruct mapStruct;

	void GenerateMap();
	void LoadMap(int index);
	void SaveMap();

	std::string GetMapName(int index);
	int GetMapCount();

	void AddBrick(Brick* brick, int index);
	void AddBrick(BrickStruct brick, int index);

};




