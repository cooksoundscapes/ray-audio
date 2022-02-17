#include "Library.h"

DrawAudio Oscilloscope = [](Component<Audio::AudioBuffer>* self)
{
    Rectangle cnv{
        self->rect.x + spacing,
        self->rect.y + self->label.size,
        self->rect.width - (spacing*2),
        self->rect.height - self->label.size - spacing,
    };
    float midpoint = cnv.height/2;
    float jump = cnv.width / self->buffer.size();

    DrawRectangle(
        cnv.x,
        cnv.y,
        cnv.width,
        cnv.height,
        {120, 120, 120, 255}
    );
    drawLabel(self->label, cnv, false);
    for (size_t i{0}; i < self->buffer.size()-1; i++)
    {
        float amp{
            ((self->buffer[i]*-1)+1)*(midpoint) + self->rect.y + self->label.size
        };
        float nextamp{
            ((self->buffer[i+1]*-1)+1)*(midpoint) + self->rect.y + self->label.size
        };
        DrawLine((int)i*jump+cnv.x, (int)amp, (int)(i+1)*jump+cnv.x, (int)nextamp, {255, 255, 255, 255} );
    }
};