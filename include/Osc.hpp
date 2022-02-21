#include <lo/lo.h>

class OSC_Control
{
    lo_address output_addr;
    lo_server_thread receiver;

    static void osc_rcv_error(
        int num, 
        const char* m, 
        const char* path
    );

    static int generic_handler(
        const char* path, 
        const char* types, 
        lo_arg** argv,
        int argc, 
        lo_message data, 
        void* user_data
    );

public:
    OSC_Control(const char* port);
    ~OSC_Control();   
    void setInPort(const char*);
    void setOutPort(const char*);

    lo_address getAddr() {return output_addr;}
};