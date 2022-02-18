#include "Graphics.h"
#include <algorithm>
#include <iostream>

Graphics::Graphics(int w, int h) : width{w}, height{h}
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(width, height, "Pd::ReFace");
    SetTargetFPS(60);            
}

Graphics::~Graphics()
{
    CloseWindow();
}

void Graphics::loop(XmlParser* document)
{
    document->render();
    while (!WindowShouldClose())
    {
        if (width != GetScreenWidth() || height != GetScreenHeight())
        {
            width = GetScreenWidth();
            height = GetScreenHeight();
            document->render();
        }
        mouse_position = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);

        document->drawComponents();
        //document->drawNodeBoxes();
        
        EndDrawing();
    }
}