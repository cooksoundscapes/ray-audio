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

Color Graphics::fromHex(int hex)
{
    Color output;

    output.r = ((hex >> 16) & 0xFF);  // Extract the RR byte
    output.g = ((hex >> 8) & 0xFF);   // Extract the GG byte
    output.b = ((hex) & 0xFF);        // Extract the BB byte
    output.a = 255;
    
    return output;
}