
#include "Library.hpp"

void updateValue
(Component<Audio::MidiBuffer>* comp)
{
    for (auto& msg : comp->buffer)
        if( ((int)msg[1] == comp->channel) && (comp->midi_value != (int)msg[2]) )
            comp->midi_value = msg[2];   
}

Color fromHex(int hex)
{
    Color output;
    output.r = ((hex >> 16) & 0xFF) / 255.0f;  // Extract the RR byte
    output.g = ((hex >> 8) & 0xFF) / 255.0f;   // Extract the GG byte
    output.b = ((hex) & 0xFF) / 255.0f;        // Extract the BB byte
    output.a = 1;
    return output;
}