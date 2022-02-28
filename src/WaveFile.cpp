#include "CairoLib.hpp"
#include <filesystem>
#include <sstream>  
#include <fstream>
#include <iostream>
#include "AudioFile.h"  

using namespace CairoLib;

namespace _WaveFile {

bool done{false};
cairo_surface_t* waveform;

}
using namespace _WaveFile;

DrawControl WaveFile = [](Component<Audio::MidiBuffer>* self)
{
    Rect cnv{
        self->rect.x + spacing,
        self->rect.y + self->label.size,
        self->rect.w - (spacing*2),
        self->rect.h - self->label.size - spacing,
    };
    float midpoint = cnv.h/2;

    DrawRect(cnv, fromHex(0x99a090));
    DrawText(self->label, cnv, false);

    if (!done)
    {
        AudioFile<float> file;
        file.load("/home/me/samples/Loops/menina.wav");
        file.printSummary();
        float jump = cnv.w / file.getNumSamplesPerChannel();

        BeginPreRender(waveform, cnv.w, cnv.h);
        for (size_t i{0}; i < file.getNumSamplesPerChannel()-1; i++)
        {
            float amp{
                ((file.samples[0][i]*-1)+1)*(midpoint) + cnv.y
            };
            float nextamp{
                ((file.samples[0][i+1]*-1)+1)*(midpoint) + cnv.y
            };
            DrawLine(
                { (int)(i*jump+cnv.x), (int)amp }, 
                { (int)((i+1)*jump+cnv.x), (int)nextamp }, 
                fromHex(0xfafafa) 
            );
        }
        done = true;
    }
    else {
        PaintSurface(waveform, cnv.x, cnv.y);
    }
};