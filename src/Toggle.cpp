#include "CairoLib.hpp"

using namespace CairoLib;

DrawControl Toggle = [](Component<Audio::MidiBuffer>* self)
{
    updateValue(self);

    float side = std::min(self->rect.w, self->rect.h) / 6;
    Rect c_box{
        self->rect.x + (self->rect.w/2) - side/2,
        self->rect.y + (self->rect.h/2) - side/2,
        side,
        side
    };
    if (CheckCollision(mouse_position, c_box))
        if(GetMouseLeftClick())
            self->midi_value = !self->midi_value;

    DrawText(self->label, c_box);
    
    Color col{ (self->midi_value > 0) ? fromHex(0x22dd00) : fromHex(0x116600) };
    DrawRect( c_box, col);
};