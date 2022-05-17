#include "Map.h"
#include <fstream>


Map::Map()
{
}

Map::~Map()
{
}

void Map::GenerateMap()
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 13; j++) {

            int index = (i * 13) + j;
            if(&mapStruct.bricks[index] == nullptr){
                continue;
            }
            Brick* brick = new Brick(j * 66, i * 33, mapStruct.bricks[index].health, mapStruct.bricks[index].colour);
        }
    }
}

void Map::LoadMap(int index)
{
    std::fstream file("Saves.txt", std::ios::in | std::ios::binary );
    
    file.seekg(sizeof(MapStruct) * index);

    // Get map name
    char* name = new char[21];
    file.read((char*)name, 20);
    name[20] ='\0';
    mapName = name;

    file.read((char*)&mapStruct, sizeof(MapStruct));

    file.close();

    delete[] name;

}

void Map::SaveMap()
{
    std::fstream file("Saves.txt", std::ios::out | std::ios::binary | std::ios::app);

    file.write((char*)mapName.c_str(), 20);
    file.write((char*)&mapStruct, sizeof(MapStruct));

    file.close();
}

std::string Map::GetMapName(int index)
{

    std::fstream file("Saves.txt", std::ios::in | std::ios::binary);

    file.seekg((sizeof(MapStruct)+20) * index);

    // Get map name
    char* name = new char[21];
    file.read((char*)name, 20);
    name[20] = '\0';

    file.close();

    std::string stringName = std::string(name);

    delete[] name;

    return stringName;
}

int Map::GetMapCount()
{
    std::fstream file("Saves.txt", std::ios::in | std::ios::binary);

    int currentAmount = 0;
    while (true) {
        if (file.peek() == EOF) {
            file.close();
            return currentAmount;
        }

        file.seekg((sizeof(MapStruct) + 20) * currentAmount);

        currentAmount++;  
    }

}

void Map::AddBrick(Brick* brick, int index)
{
    mapStruct.bricks[index] = BrickStruct({ ColorToInt(brick->sprite->colour), brick->maxHealth, false });
}
void Map::AddBrick(BrickStruct brick, int index)
{
    mapStruct.bricks[index] = brick;


}
