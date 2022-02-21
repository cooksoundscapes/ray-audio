#pragma once
#include <jack/jack.h>
#include <jack/midiport.h>
#include <vector>
#include <array>
#include <string>

class Audio
{
public:
    using JackPorts = std::vector<jack_port_t*>;
    using AudioBuffer = std::vector<float>;
    using midi_msg_t = std::array<unsigned int, 3>;
    using MidiBuffer = std::vector<midi_msg_t>;

    Audio(int a_ch, int m_ch);
    ~Audio();

    int process(jack_nframes_t);

    const auto& getMidiBuffer() const {return midi_buffer;}
    const AudioBuffer& getBuffer(int chan) const {return buffers[chan];}

    int getBuffSize() {return (int)bufferSize;};
    void connect();

private:
    jack_nframes_t bufferSize;
    jack_client_t* client;

    JackPorts ports;
    JackPorts midi_ports;

    std::vector<AudioBuffer> buffers;

    MidiBuffer midi_buffer;

    static int outside_process_call(jack_nframes_t, void*);
};
