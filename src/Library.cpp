
#include "Library.h"

SDL_Point mouse_position;

void DrawRect
(Rect rect, Color color, bool filled, float line=1)
{
    cairo_set_source_rgba(cr, color.r, color.g, color.b, color.a);
    cairo_rectangle(cr, rect.x, rect.y, rect.w, rect.h);
    cairo_set_line_width(cr, line);
    if (filled) cairo_fill(cr);
    else {
        cairo_set_line_width(cr, line);
        cairo_stroke(cr);
    }
}

void DrawText
(cairo_t* cr, std::string text, int x, int y, Color)
{

}

void drawMovingBar
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

void drawLabel
(BaseComponent::Label label, Rect rect, bool centered)
{
    int offX{0};
    if (centered)
        offX = (rect.w/2) - (MeasureText(label.text, label.size)/2);
    
    DrawText(
        label.text  , 
        rect.x + label.offX + offX,
        rect.y - label.size + label.offY,
        label.size,
        label.color
    );
}

void updateValue
(Component<Audio::MidiBuffer>* comp)
{
    for (auto& msg : comp->buffer)
        if( ((int)msg[1] == comp->channel) && (comp->midi_value != (int)msg[2]) )
            comp->midi_value = msg[2];   
}

Color fromHex(int hex)
{
    Color output;
    output.r = ((hex >> 16) & 0xFF) / 255;  // Extract the RR byte
    output.g = ((hex >> 8) & 0xFF) / 255;   // Extract the GG byte
    output.b = ((hex) & 0xFF) / 255;        // Extract the BB byte
    output.a = 1;
    return output;
}