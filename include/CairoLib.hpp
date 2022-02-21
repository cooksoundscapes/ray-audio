#pragma once
#include <cairo/cairo.h>
#include <string>
#include "Library.hpp"
#define PI 3.14159265359

namespace CairoLib
{
    extern cairo_t* ctx;

    extern void DrawRect(Rect, Color, bool filled=true, float line=1);
    extern void DrawText(BaseComponent::Label&, Rect&, bool centered=true);
    extern void DrawCircle(SDL_Point center, float radius, Color, bool filled=true);
    extern void DrawLine(SDL_Point start, SDL_Point end, Color, float line_width=1.0f);
    extern void DrawMovingBar(Rect, float, Color, bool vertical=true, bool inverted=false);
    extern int MeasureText(std::string content, int font_size);
    extern bool CheckCollision(SDL_Point, SDL_Rect);
};
