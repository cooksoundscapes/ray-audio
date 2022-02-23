#pragma once
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Audio.hpp"
#include "BaseComponent.hpp"

using DrawControl = Component<Audio::MidiBuffer>::DrawFunction;
using DrawAudio = Component<Audio::AudioBuffer>::DrawFunction;
//setup_t returns the actual drawing function, providing a onetime setup;
using setup_t = std::function<DrawControl()>;

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

extern setup_t Slider;
extern setup_t Knob;
extern setup_t Toggle;
extern setup_t DirTree;
/*extern setup_t Wavetable;
extern setup_t Transport;
extern setup_t Table;
extern setup_t CarouselBox;
extern setup_t Multitable;
extern setup_t ListBox;
extern setup_t Text;
extern setup_t PadButton;*/

const std::map<std::string, Flow> container_types{
    {"Row", H},
    {"Column", V}
};

const std::map<std::string, DrawAudio> audio_types{
    {"VU", VU},
    {"Oscilloscope", Oscilloscope}
};

const std::map<std::string, setup_t> control_types{
    {"Slider", Slider},
    {"Knob", Knob},
    {"Toggle", Toggle},
    {"DirTree", DirTree},
    /*{"Wavetable", Wavetable}
    {"Table", Table},
    {"CarouselBox", CarouselBox},
    {"Multitable", Multitable},
    {"ListBox", ListBox},
    {"Text", Text},
    {"PadButton", PadButton}*/
};