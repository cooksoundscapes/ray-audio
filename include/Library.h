#pragma once
#include <map>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cairo/cairo.h>
#include "Audio.h"
#include "Component.h"

using DrawControl = Component<Audio::MidiBuffer>::DrawFunction;
using DrawAudio = Component<Audio::AudioBuffer>::DrawFunction;
//setup_t returns the actual drawing function, providing a onetime setup;
using setup_t = std::function<DrawControl()>;

struct Rect{ float x, y, w, h; };
struct Color{ double r, g, b, a; };

enum Flow { H, V }; //horizontal, vertical
const float spacing = 8;
extern SDL_Point mouse_position;
extern int window_width;

/*
 * Using the Cairo lib;
 * The drawing functions must NOT interact with cairo objects and calls;
 */
extern void drawMovingBar(Rect, float, Color, bool vertical = true, bool inverted = false);
extern void drawLabel(BaseComponent::Label, Rect, bool centered = true);
extern void updateValue(Component<Audio::MidiBuffer>*);
extern Color fromHex(int);

//breakpoints based on window width;
const std::map<int, int> font_breakpoints{
    {320, 10},
    {480, 20},
    {720, 30}
};

//components
extern DrawAudio VU;
//extern DrawAudio Oscilloscope;

extern setup_t Slider;
/*extern setup_t Knob;
extern setup_t Toggle;
extern setup_t DirTree;
extern setup_t Wavetable;
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
    //{"Oscilloscope", Oscilloscope}
};

const std::map<std::string, setup_t> control_types{
    {"Slider", Slider},
    /*{"Knob", Knob},
    {"Toggle", Toggle},
    {"DirTree", DirTree},
    {"Wavetable", Wavetable}
    {"Table", Table},
    {"CarouselBox", CarouselBox},
    {"Multitable", Multitable},
    {"ListBox", ListBox},
    {"Text", Text},
    {"PadButton", PadButton}*/
};