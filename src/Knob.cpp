#include "CairoLib.hpp"

setup_t Knob = []()
{
    return DrawControl(
        [=](Component<Audio::MidiBuffer>* self){

            std::pair<float, float> range{0, 127}; 

            //update midi value;
            updateValue(self);

            //real drawing part;
            float radius{ (std::min(self->rect.w, self->rect.h)) / 4.0f };
            SDL_Point center{
                self->rect.x + (self->rect.w/2),
                self->rect.y + (self->rect.h/2)
            };
            CairoLib::DrawCircle(center, radius, fromHex(0xb1b1b1) );

            /*if (CheckCollisionPointCircle(mouse_position, center, radius))
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                    self->midi_value = std::max(
                    0.0f, 
                    std::min(
                        (float)self->midi_value - GetMouseDelta().y,
                            range.second
                    )
                );   */         

            float angle = (self->midi_value/range.second) * 300 + 120;
            angle = angle * PI / 180;
            SDL_Point radians{
                (float)(cos(angle) * radius) + center.x,
                (float)(sin(angle) * radius) + center.y
            };
            CairoLib::DrawLine(center, radians, fromHex(0xffffff) );
            //drawLabel(self->label, {center.x - radius, center.y - radius, radius*2, radius*2});
        }
    );
};
