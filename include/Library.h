#pragma once
#include <map>
#include <iostream>
#include <algorithm>
#include "Audio.h"
#include "Component.h"

using DrawControl = Component<Audio::MidiBuffer>::DrawFunction;
using DrawAudio = Component<Audio::AudioBuffer>::DrawFunction;
using Scheme = std::map<std::string, Color>;
using Points = std::vector<float>;

const float spacing = 8;
enum Flow { H, V }; //horizontal, vertical

const std::map<std::string, Flow> container_types{
    {"Row", H},
    {"Column", V}
};

//conveniences
extern Vector2 mouse_position;
extern void drawMovingBar(Rectangle, float, Color, bool vertical = true, bool inverted = false);
extern void drawLabel(Rectangle, BaseComponent::Label, Color);
extern void updateValue(Component<Audio::MidiBuffer>*);

//components

extern DrawControl Slider;
extern DrawControl Knob;
extern DrawControl Toggle;
//extern DrawControl DirTree;
//extern DrawControl Table;
// extern DrawControl CarouselBox;
// extern DrawControl Multitable;
// extern DrawControl ListBox;
// extern DrawControl Text;
// extern DrawControl PadButton;

extern DrawAudio VU;
extern DrawAudio Oscilloscope;

const std::map<std::string, DrawAudio> audio_types{
    {"VU", VU},
    {"Oscilloscope", Oscilloscope}
};

const std::map<std::string, DrawControl> control_types{
    {"Slider", Slider},
    {"Knob", Knob},
    {"Toggle", Toggle},
    //{"Table", Table},
    //{"DirTree", DirTree}
    //{"CarouselBox", CarouselBox},
    //{"Multitable", Multitable},
    //{"ListBox", ListBox},
    //{"Text", Text},
    //{"PadButton", PadButton}
};


/*
DrawControl Slider(Rectangle, Scheme, Label, 
    int rangeMin = 0, int rangeMax = 127, bool vertical = true, bool inverted = false);

DrawAudio VU(Rectangle, Scheme, Label,
    bool vertical = true, bool inverted = false);

DrawControl Toggle(Rectangle, Scheme, Label);

DrawControl CarouselBox(Rectangle, Scheme, std::vector<std::string> options);

DrawControl DirTree(Rectangle, Scheme, std::string dir);

DrawControl Table(Rectangle, Scheme, Label, int length, Points points);

DrawControl MultiTable(Rectangle, Scheme, Label, std::vector<Points>);

DrawControl ListBox(Rectangle, Scheme, Label, std::vector<std::string>);

DrawAudio Oscilloscope(Rectangle, Scheme, Label, int size);

DrawControl Knob(Rectangle, Scheme, Label,
    float rangeMin = 0, float rangeMax = 127);

DrawControl Text(Rectangle, float size, Color);

DrawControl PadButton(Rectangle, Scheme, Label);
*/