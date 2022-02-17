#include "Library.h"

DrawAudio VU = [](Component<Audio::AudioBuffer>* self, ...)
{
    bool vertical{false}, inverted{false};
    
    float fixed_width = 20;

    Rectangle bar;
    if (vertical) 
    bar = {
        self->rect.x + (self->rect.width/2) - (fixed_width/2),
        self->rect.y + self->label.size + (spacing * 1.5f),
        fixed_width,
        self->rect.height - (spacing * 3) - self->label.size
    };
    else bar = {
        self->rect.x + spacing,
        self->rect.y + self->label.size + (self->rect.height/2) - (fixed_width/2),
        self->rect.width - (spacing * 2),
        fixed_width,
    };

    float rms{0};
    try {
        for (auto& samp : self->buffer)
            rms += sqrt(std::abs( samp ));
    } catch (...) {
        rms = 0;
    }
    rms = rms/self->buffer.size();

    drawLabel(self->label, bar);
    DrawRectangle(
        bar.x,
        bar.y,
        bar.width,
        bar.height,
        {0, 150, 0, 255}
    );
    drawMovingBar(bar, rms, {0, 255, 0, 255}, vertical, inverted);
};