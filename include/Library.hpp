#pragma once
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Audio.hpp"
#include "BaseComponent.hpp"

using DrawControl = Component<Audio::MidiBuffer>::DrawFunction;
using DrawAudio = Component<Audio::AudioBuffer>::DrawFunction;

enum Flow { H, V }; //horizontal, vertical
const float spacing = 8;

extern void updateValue(Component<Audio::MidiBuffer>*);
extern Color fromHex(int);

//breakpoints based on window width;
const std::map<int, int> font_breakpoints{
    {320, 10},
    {480, 14},
    {720, 20}
};

extern DrawAudio VU;
extern DrawAudio Oscilloscope;

extern DrawControl Slider;
extern DrawControl Knob;
extern DrawControl Toggle;
extern DrawControl DirTree;
extern DrawControl WaveFile;
/*extern DrawControl Transport;
extern DrawControl Table;
extern DrawControl CarouselBox;
extern DrawControl Multitable;
extern DrawControl ListBox;
extern DrawControl Text;
extern DrawControl PadButton;*/

const std::map<std::string, Flow> container_types{
    {"Row", H},
    {"Column", V}
};

const std::map<std::string, DrawAudio> audio_types{
    {"VU", VU},
    {"Oscilloscope", Oscilloscope}
};

const std::map<std::string, DrawControl> control_types{
    {"Slider", Slider},
    {"Knob", Knob},
    {"Toggle", Toggle},
    {"DirTree", DirTree},
    {"WaveFile", WaveFile}
    /*{"Table", Table},
    {"CarouselBox", CarouselBox},
    {"Multitable", Multitable},
    {"ListBox", ListBox},
    {"Text", Text},
    {"PadButton", PadButton}*/
};