#pragma once
#include <cairo/cairo.h>
#include <string>
#include "Library.hpp"

namespace CairoLib
{
    extern cairo_t* ctx;

    extern void DrawRect(Rect, Color, bool filled=true, float line=1);
    extern void DrawText(BaseComponent::Label&, Rect&, bool centered=true);
    extern int MeasureText(std::string content, int font_size);
    extern bool CheckCollision(SDL_Point, SDL_Rect);

    extern void DrawMovingBar(Rect, float, Color, bool vertical=true, bool inverted=false);
};
