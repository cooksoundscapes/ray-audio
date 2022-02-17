
#include "Library.h"

Vector2 mouse_position;

void drawMovingBar(
    Rectangle rect, float mod, Color color, bool vertical, bool inverted)
{
    DrawRectangle(
        (!vertical && inverted) ? rect.x + rect.width - (mod*rect.width) : rect.x,
        (vertical && !inverted) ? rect.y + rect.height - (mod*rect.height) : rect.y,
        (!vertical) ? mod*rect.width : rect.width, 
        (vertical) ? mod*rect.height : rect.height,
        color
    );
}

void drawLabel(BaseComponent::Label label, Rectangle rect, bool centered)
{
    int offX{0};
    if (centered)
        offX = (rect.width/2) - (MeasureText(label.text.c_str(), label.size)/2);
    
    DrawText(
        label.text.c_str(), 
        rect.x + label.offX + offX,
        rect.y - label.size + label.offY,
        label.size,
        label.color
    );
}

void updateValue(Component<Audio::MidiBuffer>* comp)
{
    for (auto& msg : comp->buffer)
        if( ((int)msg[1] == comp->channel) && (comp->midi_value != (int)msg[2]) )
            comp->midi_value = msg[2];   
}


Color fromHex(int hex)
{
    Color output;

    output.r = ((hex >> 16) & 0xFF);  // Extract the RR byte
    output.g = ((hex >> 8) & 0xFF);   // Extract the GG byte
    output.b = ((hex) & 0xFF);        // Extract the BB byte
    output.a = 255;
    
    return output;
}