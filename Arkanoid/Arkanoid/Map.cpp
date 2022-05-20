#include "Map.h"
#include <fstream>
#include "Game.h"

std::string Map::saveLocation = "Saves.txt";

Map::Map()
{
}

Map::~Map()
{
}

void Map::GenerateMap()
{
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {

            int index = (i * 13) + j;
            if(mapStruct.bricks[index].isEmpty){
                continue;
            }
            Brick* brick = new Brick( Game::WorldBorders.x + (j * 66), Game::WorldBorders.y + (i * 33), mapStruct.bricks[index].health, mapStruct.bricks[index].colour);
        }
    }
}

void Map::LoadMap(int index)
{
    std::fstream file(saveLocation, std::ios::in | std::ios::binary );
    
    // Seek location of save
    file.seekg((20+sizeof(MapStruct)) * index);
    loadedIndex = index;

    // Get map name
    char* name = new char[21];
    file.read((char*)name, 20);
    name[20] ='\0';
    mapName = name;

    // Get the map data
    file.read((char*)&mapStruct, sizeof(MapStruct));

    delete[] name;
    // Close file
    file.close();

}

void Map::SaveMap()
{
    std::fstream file(saveLocation, std::ios::out | std::ios::binary | std::ios::app);
    loadedIndex = GetMapCount();

    // Write to end of file
    file.write((char*)mapName.c_str(), 20);
    file.write((char*)&mapStruct, sizeof(MapStruct));

    // Close file
    file.close();
}

void Map::SaveMap(int index)
{
    std::fstream file(saveLocation, std::ios::in | std::ios::out | std::ios::binary);

    file.seekp((20 + sizeof(MapStruct)) * index);

    // Write to end of file
    file.write((char*)mapName.c_str(), 20);
    file.write((char*)&mapStruct, sizeof(MapStruct));

    // Close file
    file.close();
   
}

std::string Map::GetMapName(int index)
{
    std::fstream file(saveLocation, std::ios::in | std::ios::binary);

    // Seek location of save
    file.seekg((sizeof(MapStruct)+20) * index);

    // Get map name
    char* name = new char[21];
    file.read((char*)name, 20);
    name[20] = '\0';

    // Close file
    file.close();

    std::string stringName = std::string(name);
    delete[] name;

    return stringName;
}

int Map::GetMapCount()
{
    std::fstream file(saveLocation, std::ios::in | std::ios::binary);

    int currentAmount = 0;
    while (true) {
        // Go to the start of the next save
        file.seekg((sizeof(MapStruct) + 20) * currentAmount);
        // If there is nothing at that point, all saves have been read
        if (file.peek() == EOF) {
            // Close file
            file.close();
            return currentAmount;
        }

        // Add to save count
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
