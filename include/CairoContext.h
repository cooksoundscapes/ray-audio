#pragma once
#include <cairo/cairo.h>
#include <string>
#include "Library.h"

class Cairo
{
 cairo_t* cr;
public:
    Cairo();
    ~Cairo();

    cairo_t* ctx() {return cr;}

    void DrawRect(Rect, Color, bool filled = true);
    void DrawText(std::string content, int x, int y, int size, Color);
    int MeasureText(std::string content, int font_size);
    bool CheckCollision(SDL_Point, SDL_Rect);
};

