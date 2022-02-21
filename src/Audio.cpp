#include <iostream>
#include <algorithm>
#include "Audio.hpp"

extern "C" {
#include <string.h>
}

int Audio::outside_process_call(jack_nframes_t nframes, void* self)
{
    return static_cast<Audio*>(self)->process(nframes);
}

int Audio::process(jack_nframes_t nframes) 
{
    for (size_t i{0}; i < ports.size(); i++)
    {
        float* temp = (float*)jack_port_get_buffer(ports[i], nframes);
        for (size_t s{0}; s < bufferSize; s++)
        {
            buffers[i][s] = temp[s];
        }
    }
    
    //process midi messages
    jack_midi_event_t current;
    for (size_t m{0}; m < midi_ports.size(); m++) //for each midi channel 
    {
        void* midi_buff = jack_port_get_buffer(midi_ports[m], nframes);

        jack_nframes_t event_count = jack_midi_get_event_count(midi_buff);
        if (event_count > 0) 
        {
            midi_buffer.clear();
            for (int e{0}; e < (int)event_count; e++)
            {
                jack_midi_event_get(&current, midi_buff, e);
                
                midi_msg_t msg;

                for (size_t i{0}; i < current.size ; i++) {
                    msg[i] = static_cast<unsigned int>(current.buffer[i]);
                }

                midi_buffer.push_back(msg);
            }
        }
    }
    
    return 0;
}

Audio::Audio(int chan, int midi_chan)
{
    buffers.resize(chan);
    if((client = jack_client_open("Pd-Interface", JackNullOption, NULL)) == NULL)
    {
        std::cerr << "\nJack server not running, program will not process audio." << std::endl;        
        for (auto& buff : buffers)
        {
            buff.resize(1);
        }
        return;
    } else {
        bufferSize = jack_get_buffer_size(client);
    }

    //pre allocate & initialize audio buffers
    for (auto& buff: buffers)
    {
        buff.resize(bufferSize);
    }
    //pre allocate only the jack ports;
    ports.reserve(chan);
    midi_ports.reserve(midi_chan);

    //register audio inputs
    for (int i{0}; i < chan; i++) 
    {
        std::string portName = "input_"+std::to_string(i+1);
        ports.push_back(jack_port_register(
            client, 
            portName.c_str(), 
            JACK_DEFAULT_AUDIO_TYPE, 
            JackPortIsInput, 
            0
        ));
    }
    //register midi inputs
    for (int i{0}; i < midi_chan; i++) 
    {
        std::string portName = "midi-in_"+std::to_string(i+1);
        midi_ports.push_back(jack_port_register(
            client,
            portName.c_str(),
            JACK_DEFAULT_MIDI_TYPE,
            JackPortIsInput,
            0
        ));
    }

    jack_set_process_callback(client, Audio::outside_process_call, this);

    if (jack_activate (client)) 
    {
        std::cerr << "Could not activate client";
    }

    //FOR TESTING PURPOSES ONLY! (maybe)
    jack_connect(client, "a2j:Midi Through [14] (capture): Midi Through Port-0", "Pd-Interface:midi-in_1");
    jack_connect(client, "system:capture_1", "Pd-Interface:input_1");
    jack_connect(client, "system:capture_2", "Pd-Interface:input_2");
};

Audio::~Audio()
{
    if (!client) return;
    if(jack_deactivate(client)) {
        std::cerr << "Failed to close client;\n";
    }
}

void Audio::connect() 
{
    // Here, it should find the pure data ports
    const char** system_ports { 
        jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput)
    };

    std::vector<std::string> sysport_list;

    while( *system_ports != nullptr)
    {
        sysport_list.push_back(*system_ports++);
    }

    for (size_t i{0}; i < ports.size(); i++) 
    {
        const char* destination = jack_port_name(ports[i]);
        const char* source = sysport_list[i].c_str();
        if (jack_connect(client, source, destination)) 
        {
            std::cerr << "Can't connect " << source << " to " << destination << ".\n";
        };
    }
}