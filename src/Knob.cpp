#include "CairoLib.hpp"
#include <algorithm>

using namespace CairoLib;

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
                (int)(self->rect.x + self->rect.w/2),
                (int)(self->rect.y + self->rect.h/2)
            };
            DrawCircle(center, radius, fromHex(0xb1b1b1) );

            if (CheckCollision(mouse_position, 0, center, (int)radius))
                if (MouseLeftButtonPressed)
                    self->midi_value = std::max(
                    0.0f, 
                    std::min(
                        (float)self->midi_value - GetMouseDelta().y,
                            range.second
                    )
                );         

            float angle = (self->midi_value/range.second) * 300 + 120;
            angle = angle * PI / 180;
            SDL_Point radians{
                (int)(cos(angle) * radius + center.x),
                (int)(sin(angle) * radius + center.y)
            };
            DrawLine(center, radians, fromHex(0xffffff) );
            //drawLabel(self->label, {center.x - radius, center.y - radius, radius*2, radius*2});
        }
    );
};
