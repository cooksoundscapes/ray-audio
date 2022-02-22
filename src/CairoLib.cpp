#include "CairoLib.hpp"

cairo_t* CairoLib::ctx;
SDL_Point CairoLib::mouse_position;
SDL_Point CairoLib::mouse_delta;
bool CairoLib::MouseLeftButtonPressed;

using namespace CairoLib;

void CairoLib::DrawRect
(Rect rect, Color color, bool filled, float line)
{
    cairo_set_source_rgba(ctx, color.r, color.g, color.b, color.a);
    cairo_rectangle(ctx, rect.x, rect.y, rect.w, rect.h);
    cairo_set_line_width(ctx, line);
    if (filled) cairo_fill(ctx);
    else {
        cairo_stroke(ctx);
    }
}

void CairoLib::DrawText(BaseComponent::Label& label, Rect& rect, bool centered)
{
    int offX{0};
    if (centered)
        offX = (rect.w/2) - (MeasureText(label.text, label.size)/2);
}

int CairoLib::MeasureText(std::string text, int fontSize)
{
    return 0;
}

void CairoLib::DrawMovingBar
(Rect rect, float mod, Color color, bool vertical, bool inverted)
{
    DrawRect(
        {(!vertical && inverted) ? rect.x + rect.w - (mod*rect.w) : rect.x,
        (vertical && !inverted) ? rect.y + rect.h - (mod*rect.h) : rect.y,
        (!vertical) ? mod*rect.w : rect.w, 
        (vertical) ? mod*rect.h : rect.h},
        color
    );
}

void CairoLib::DrawCircle(SDL_Point center, float radius, Color color, bool filled)
{
    cairo_set_source_rgba(ctx, color.r, color.g, color.b, color.a);
    cairo_arc(ctx, center.x, center.y, radius, 0, PI*2);
    if (filled) cairo_fill(ctx);
    else {
        cairo_stroke(ctx);
    }
}

void CairoLib::DrawLine(SDL_Point start, SDL_Point end, Color color, float width)
{
    cairo_set_source_rgba(ctx, color.r, color.g, color.b, color.a);
    cairo_move_to(ctx, start.x, start.y);
    cairo_line_to(ctx, end.x, end.y);
    cairo_stroke(ctx);
}

bool CairoLib::CheckCollision(SDL_Point p, Rect r)
{
    if ( ((float)p.x > r.x) && ((float)p.x < (r.x+r.w)) &&
         ((float)p.y > r.y) && ((float)p.y < (r.y+r.h)) )
    return true;
    else return false;
}

//circle de
bool CairoLib::CheckCollision(SDL_Point p, int r1, SDL_Point c, int r2)
{
    int dx = p.x - c.x;
    int dy = p.y - c.y;
    float distance = sqrt(dx*dx + dy*dy);
    if (distance < r1 + r2) return true;
    else return false;
}
SDL_Point CairoLib::GetMouseDelta() 
{
    SDL_Point diff{0, 0};   
    if ((mouse_position.x != mouse_delta.x) || (mouse_position.y != mouse_delta.y)) {
        diff = {
            mouse_position.x - mouse_delta.x,
            mouse_position.y - mouse_delta.y
        };
        mouse_delta = mouse_position;
    }
    return diff;
}