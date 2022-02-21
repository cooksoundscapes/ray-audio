#include "CairoLib.hpp"
#include <filesystem>
#include <sstream>  
#include <fstream>
#include <iostream>
#include "AudioFile.h"  

setup_t Wavetable = []()
{
    AudioFile<float> file;
    file.load("/home/me/samples/Loops/menina.wav");
    file.printSummary();
    
    return DrawControl(
        [file](Component<Audio::MidiBuffer>* self) {
            Rectangle cnv{
                self->rect.x + spacing,
                self->rect.y + self->label.size,
                self->rect.width - (spacing*2),
                self->rect.height - self->label.size - spacing,
            };
            float midpoint = cnv.height/2;
            float jump = cnv.width / file.getNumSamplesPerChannel();

            DrawRectangleRec(cnv, fromHex(0x99a090));
            drawLabel(self->label, cnv, false);

            for (size_t i{0}; i < file.getNumSamplesPerChannel()-1; i++)
            {
                float amp{
                    ((file.samples[0][i]*-1)+1)*(midpoint) + self->rect.y + self->label.size
                };
                float nextamp{
                    ((file.samples[0][i+1]*-1)+1)*(midpoint) + self->rect.y + self->label.size
                };
                DrawLine((int)i*jump+cnv.x, (int)amp, (int)(i+1)*jump+cnv.x, (int)nextamp, fromHex(0xfafafa) );
            }
        }
    );
};