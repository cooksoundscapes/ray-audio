#include "Library.h"

setup_t Toggle = []()
{
    return DrawControl(
        [](Component<Audio::MidiBuffer>* self) {
            float side = 20;
            Rectangle c_box{
                self->rect.x + (self->rect.width/2) - side/2,
                self->rect.y + (self->rect.height/2) - side/2,
                side,
                side
            };
            updateValue(self);

            Vector2& mouse = mouse_position;
            if (CheckCollisionPointRec(mouse, c_box))
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    self->midi_value = !self->midi_value;

            drawLabel(self->label, c_box);
            
            Color col;
            if (self->midi_value > 0) col = {0, 255, 0, 255};
            else col = {0, 120, 0, 255};
            DrawRectangle( c_box.x, c_box.y, c_box.width, c_box.height, col );
        }
    );
};