#include "CairoLib.hpp"

DrawAudio VU = [](Component<Audio::AudioBuffer>* self)
{
    bool vertical{true}, inverted{false};
    
    float thin = std::min(20.0f, std::min(self->rect.w, self->rect.h) / 4);

    Rect bar;
    if (vertical) 
    bar = {
        self->rect.x + (self->rect.w/2) - (thin/2),
        self->rect.y + self->label.size + (spacing * 1.5f),
        thin,
        self->rect.h - (spacing * 3) - self->label.size
    };
    else bar = {
        self->rect.x + spacing,
        self->rect.y + self->label.size + (self->rect.h/2) - (thin/2),
        self->rect.w - (spacing * 2),
        thin,
    };

    float rms{0};
    try {
        for (auto& samp : self->buffer)
            rms += sqrt(std::abs( samp ));
    } catch (...) {
        rms = 0;
    }
    rms = rms/self->buffer.size();

    CairoLib::DrawText(self->label, bar);
    CairoLib::DrawRect(bar, fromHex(0x114400));
    CairoLib::DrawMovingBar(bar, rms, fromHex(0x22ff00), vertical, inverted);
};