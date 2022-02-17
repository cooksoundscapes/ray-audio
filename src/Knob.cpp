#include "Library.h"

setup_t Knob = []()
{
    return DrawControl(
        [=](Component<Audio::MidiBuffer>* self){

            std::pair<float, float> range{0, 127}; 

            //update midi value;
            updateValue(self);

            //real drawing part;
            float radius{ (std::min(self->rect.width, self->rect.height)) / 4.0f };
            Vector2 center{
                self->rect.x + (self->rect.width/2),
                self->rect.y + (self->rect.height/2)
            };
            DrawCircle( center.x, center.y, (int)radius, {160, 160, 160, 255} );

            if (CheckCollisionPointCircle(mouse_position, center, radius))
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                    self->midi_value = std::max(
                    0.0f, 
                    std::min(
                        (float)self->midi_value - GetMouseDelta().y,
                            range.second
                    )
                );            

            float angle = (self->midi_value/range.second) * 300 + 120;
            angle = angle * PI / 180;
            Vector2 radians{
                (float)(cos(angle) * radius) + center.x,
                (float)(sin(angle) * radius) + center.y
            };
            DrawLine(center.x, center.y, radians.x, radians.y, {255, 255, 255, 255} );
            drawLabel(self->label, {center.x - radius, center.y - radius, radius*2, radius*2});
        }
    );
};
