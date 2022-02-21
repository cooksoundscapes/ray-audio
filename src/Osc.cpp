#include "Osc.hpp"
#include <cstddef>
#include <iostream>

OSC_Control::OSC_Control(const char* port)
{
    setOutPort(port);
                       
    receiver = lo_server_thread_new("8887", OSC_Control::osc_rcv_error);
    
    lo_server_thread_add_method(receiver, NULL, NULL, OSC_Control::generic_handler, NULL);

    lo_server_thread_start(receiver);
}

OSC_Control::~OSC_Control() 
{
    lo_server_thread_free(receiver);
}

void OSC_Control::setOutPort(const char* port)
{
    output_addr = lo_address_new(NULL, port);
}

void OSC_Control::osc_rcv_error(int num, const char* m, const char* path)
{
    std::cout << "OSC server error " << num << " in path " << path <<
        ": " << m << std::endl;
}

int OSC_Control::generic_handler(
    const char* path, 
    const char* types, 
    lo_arg** argv,
    int argc, 
    lo_message data, 
    void* user_data
)
{
    for (int i{0}; i < argc; i++) {
        std::cout << "Arg " << i << ' ' << types[i] << '\n';
        lo_arg_pp( (lo_type)types[i], argv[i] );
    }
    std::cout << std::endl;
    return 1;
}