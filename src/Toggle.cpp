#include "CairoLib.hpp"

setup_t Toggle = []()
{
    return DrawControl(
        [](Component<Audio::MidiBuffer>* self) {
            float side = 20;
            Rect c_box{
                self->rect.x + (self->rect.w/2) - side/2,
                self->rect.y + (self->rect.h/2) - side/2,
                side,
                side
            };
            updateValue(self);

            /*if (CheckCollisionPointRec(mouse, c_box))
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    self->midi_value = !self->midi_value;

            drawLabel(self->label, c_box);*/
            
            Color col{ (self->midi_value > 0) ? fromHex(0x22dd00) : fromHex(0x116600) };
            CairoLib::DrawRect( c_box, col );
        }
    );
};