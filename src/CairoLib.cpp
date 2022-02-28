#include "CairoLib.hpp"
#include <pango/pangocairo.h>

cairo_t* CairoLib::ctx = nullptr;
cairo_surface_t* CairoLib::final_canvas = nullptr;

SDL_Point CairoLib::mouse_position;
SDL_Point CairoLib::mouse_delta;
bool CairoLib::MouseLeftButtonPressed;

using namespace CairoLib;

//overloading stream for Rectangle
std::ostream& operator <<(std::ostream& out, const Rect& r)
{
    out << r.x << ' ' << r.y << ' ' << r.w << ' ' << r.h;
    return out;
}

void CairoLib::CreateSurface(void* pixels, int w, int h, int stride)
{
   final_canvas = cairo_image_surface_create_for_data(
        (unsigned char*) pixels,
        CAIRO_FORMAT_ARGB32,
        w, h, stride
    );
    ctx = cairo_create(final_canvas);
    cairo_set_source_rgba(ctx, 0, 0, 0, 1.0);
    cairo_rectangle(ctx, 0, 0, w, h);
    cairo_fill(ctx);
}

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

bool CairoLib::GetMouseLeftClick()
{
    static bool lastState;
    bool clicked{false};
    if (MouseLeftButtonPressed && (MouseLeftButtonPressed != lastState))
        clicked = true;
    lastState = MouseLeftButtonPressed;
    return clicked;
}

constexpr const char* testfont = "Sans";

void CairoLib::DrawText(Text& text, Rect& rect, bool centered, float maxwidth)
{
    PangoLayout* layout = pango_cairo_create_layout(ctx);
    pango_layout_set_text(layout, text.text.c_str(), -1);

    std::string font = "Sans " + std::to_string(text.size);
    PangoFontDescription* desc = pango_font_description_from_string(font.c_str());
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);
    
    int offX{0}, txt_size, txt_height;
    pango_layout_get_size(layout, &txt_size, &txt_height);
    if (centered) offX = rect.w/2 - txt_size/PANGO_SCALE/2;
    
    if (maxwidth > 0) {
        pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
        pango_layout_set_width(layout, (int)(maxwidth*PANGO_SCALE));
    }
    cairo_set_source_rgba(ctx, text.color.r, text.color.g, text.color.b, text.color.a);
    double textX = rect.x + text.offX + offX;
    double textY = rect.y - (txt_height/PANGO_SCALE) + text.offY;

    //std::cout << "drawing text " << text.text << " at " << textX << ' ' << textY << '\n';

    cairo_move_to(ctx, textX, textY);

    pango_cairo_show_layout(ctx, layout);

    g_object_unref (layout);
}

void CairoLib::BeginPreRender(cairo_surface_t* surf, float w, float h)
{
    surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    ctx = cairo_create(surf);
}

void CairoLib::PaintSurface(cairo_surface_t* surf, float x, float y)
{
    cairo_set_source_surface(ctx, surf, x, y);
    cairo_paint(ctx);
}   
