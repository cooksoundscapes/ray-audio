#include "CairoLib.hpp"

using namespace CairoLib;

std::pair<float, float> range{0, 127};   
bool vertical{true}, inverted{false};

DrawControl Slider = [](Component<Audio::MidiBuffer>* self)
{       
    //update midi value;
    updateValue(self);

    float thin = std::min(20.0f, std::min(self->rect.w, self->rect.h) / 4);
    Rect bar;
    if (vertical) bar = {
        self->rect.x + (self->rect.w/2) - thin/2,
        self->rect.y + self->label.size + (spacing * 1.5f),
        thin,
        self->rect.h - (spacing * 3) - self->label.size
    };
    else bar = {
        self->rect.x + spacing,
        self->rect.y + self->label.size + (self->rect.h/2) - thin/2,
        self->rect.w - (spacing * 2),
        thin,
    };
    if (CheckCollision(mouse_position, bar))
        if (MouseLeftButtonPressed) {
            float newVal{ vertical ? 
                (((bar.y + bar.h) - mouse_position.y) / bar.h) * range.second
                : ((mouse_position.x - bar.x) / bar.w) * range.second
            };
            self->midi_value = newVal;
        }
    float ratio = self->midi_value/range.second;
    DrawText(self->label, bar);
    DrawRect( bar, fromHex(0x114400) );
    DrawMovingBar(bar, ratio, fromHex(0x22ff00), vertical, inverted);
};
