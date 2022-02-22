#include "CairoLib.hpp"

DrawAudio Oscilloscope = [](Component<Audio::AudioBuffer>* self)
{
    Rect cnv{
        self->rect.x + spacing,
        self->rect.y + self->label.size,
        self->rect.w - (spacing*2),
        self->rect.h - self->label.size - spacing,
    };
    float midpoint = cnv.h/2;
    float jump = cnv.w / self->buffer.size();

    CairoLib::DrawRect(cnv, fromHex(0x999999));
    //CairoLib::DrawText(self->label, cnv, false);

    for (size_t i{0}; i < self->buffer.size()-1; i++)
    {
        float amp{
            ((self->buffer[i]*-1)+1)*(midpoint) + self->rect.y + self->label.size
        };
        float nextamp{
            ((self->buffer[i+1]*-1)+1)*(midpoint) + self->rect.y + self->label.size
        };
        CairoLib::DrawLine(
            { (int)(i*jump+cnv.x), (int)amp }, 
            { (int)((i+1)*jump+cnv.x), (int)nextamp }, 
            fromHex(0xfafafa) );
    }
};