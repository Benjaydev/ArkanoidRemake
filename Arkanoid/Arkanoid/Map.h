#pragma once
#include "raylib.h"
#include "Brick.h"

// The struct that brick data is stored in
struct BrickStruct
{
	int colour = (int)0xFFFFFFFF;
	int health = 1;
	bool isEmpty = true;
};

// The struct that map data is stored in
struct MapStruct
{
	int backgroundColour = (int)0x333333FF;
	BrickStruct bricks[169] = {};
};


class Map
{
public:
	Map();
	~Map();

	std::string mapName = "New Level";
	MapStruct mapStruct;
	int loadedIndex = -1;

	static std::string saveLocation;

	void GenerateMap();
	void LoadMap(int index);
	void SaveMap();
	void SaveMap(int index);

	std::string GetMapName(int index);
	int GetMapCount();

	void AddBrick(Brick* brick, int index);
	void AddBrick(BrickStruct brick, int index);

};




