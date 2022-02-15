
#pragma once
#include <raylib.h>
#include <string>
#include <functional>

class BaseComponent 
{
public:
    struct Label
    {
        std::string text;   
        Color color;
        int size;
        int offX;
        int offY;
    };
    Rectangle rect;
    Label label;
    
    BaseComponent(std::string id, Rectangle r, Label l) : rect{r}, label{l} {this->id = id;}
    virtual ~BaseComponent() {}

    virtual void draw() {}
    const std::string& getId() {return id;}
private:
    std::string id;
};  

//Audio::AudioBuffer or Audio::MidiBuffer
template<typename Buffer>
class Component : public BaseComponent
{
public: 
    using DrawFunction = std::function<void(Component<Buffer>*)>;
    Component() = default;

    Component(std::string id, const Buffer& b, DrawFunction d, Rectangle r, Label l) 
    : BaseComponent{id, r, l}, buffer{b}
    {  
        this->render = d;
    } 

    Component(std::string id, const Buffer& b, DrawFunction d, int ch, Rectangle r, Label l) 
     : BaseComponent{id, r, l}, buffer{b}
    {
        this->render = d;
        this->channel = ch;
        midi_value = 0;
    } 

    int channel;
    int midi_value;
    const Buffer& buffer;

    void draw() {
        render(this);
    }

private:    
    DrawFunction render;
};