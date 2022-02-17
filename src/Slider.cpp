#include "Library.h"

setup_t Slider = []()
{   
    std::pair<float, float> range{0, 127};   
    bool vertical{true}, inverted{false};
    return DrawControl( 
        [=](Component<Audio::MidiBuffer>* self) {
            //update midi value;
            updateValue(self);
    
            Rectangle bar;
            if (vertical) bar = {
                self->rect.x + (self->rect.width/2) - 10,
                self->rect.y + self->label.size + (spacing * 1.5f),
                20,
                self->rect.height - (spacing * 3) - self->label.size
            };
            else bar = {
                self->rect.x + spacing,
                self->rect.y + self->label.size + (self->rect.height/2) - 10,
                self->rect.width - (spacing * 2),
                20,
            };
            if (CheckCollisionPointRec(mouse_position, bar))
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    float newVal{ vertical ? 
                        (((bar.y + bar.height) - mouse_position.y) / bar.height) * range.second
                        : ((mouse_position.x - bar.x) / bar.width) * range.second
                    };
                    self->midi_value = newVal;
                }
            float ratio = self->midi_value/range.second;
            drawLabel(self->label, bar);
            DrawRectangle( bar.x, bar.y, bar.width, bar.height, {0, 150, 0, 255} );
            drawMovingBar(bar, ratio, {0, 255, 0, 255}, vertical, inverted);
        }
    );
};
