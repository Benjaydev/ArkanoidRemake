#pragma once
#include "Object.h"
#include "raylib.h"
class SpriteObject :
    public Object
{
public:
    SpriteObject();
    ~SpriteObject();

    Texture2D* texture = new Texture2D();
    Image* image = new Image();

    float textureScale = 1.0f;

    Color colour = WHITE;


    float GetWidth() { return texture->width; }

    float GetHeight() { return texture->height; }

    float defaultWidth = 0;
    float defaultHeight = 0;

    void Load(char* filename);

    void OnDraw() override;

};

