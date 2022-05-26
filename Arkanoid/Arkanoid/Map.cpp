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
    // Loop through each brick location
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            // Get index in singular form
            int index = (i * 13) + j;
            // If brick is empty, ignore and move on
            if(mapStruct.bricks[index].isEmpty){
                continue;
            }
            // Create new brick using loaded data
            Brick* brick = new Brick( Game::WorldBorders.x + (j * 66), Game::WorldBorders.y + (i * 33), mapStruct.bricks[index].health, mapStruct.bricks[index].colour);
        }
    }
}

void Map::LoadMap(int index)
{
    // Open saves file
    std::fstream file(saveLocation, std::ios::in | std::ios::binary );
    
    // Seek location of save
    file.seekg((20+sizeof(MapStruct)) * index);
    // Store loaded index
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
    // Open saves file
    std::fstream file(saveLocation, std::ios::out | std::ios::binary | std::ios::app);
    // Store loaded index
    loadedIndex = GetMapCount();

    // Write to end of file
    file.write((char*)mapName.c_str(), 20);
    file.write((char*)&mapStruct, sizeof(MapStruct));

    // Close file
    file.close();
}

// Override save at index
void Map::SaveMap(int index)
{
    // Open saves file
    std::fstream file(saveLocation, std::ios::in | std::ios::out | std::ios::binary);

    // Seek location to write at
    file.seekp((20 + sizeof(MapStruct)) * index);

    // Write map data to location
    file.write((char*)mapName.c_str(), 20);
    file.write((char*)&mapStruct, sizeof(MapStruct));

    // Close file
    file.close();
   
}

std::string Map::GetMapName(int index)
{
    // Open saves file
    std::fstream file(saveLocation, std::ios::in | std::ios::binary);

    // Seek location of save
    file.seekg((sizeof(MapStruct)+20) * index);

    // Get map name
    char* name = new char[21];
    file.read((char*)name, 20);
    name[20] = '\0';

    // Close file
    file.close();

    // Convert name to string
    std::string stringName = std::string(name);
    delete[] name;

    return stringName;
}

int Map::GetMapCount()
{
    // Open saves file
    std::fstream file(saveLocation, std::ios::in | std::ios::binary);

    int currentAmount = 0;
    // Loop until all maps are read
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
    // Add brick to map using Brick class
    mapStruct.bricks[index] = BrickStruct({ ColorToInt(brick->sprite->colour), brick->maxHealth, false });
}
void Map::AddBrick(BrickStruct brick, int index)
{
    // Add brick to map using Brick struct class
    mapStruct.bricks[index] = brick;
}
