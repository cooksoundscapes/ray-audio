#include <iostream>
#include <functional>
#include "SDL_Graphics.hpp"

int window_width;

int main(int argc, const char* argv[])
{
    SDL_Engine window{640, 480}; //size;
    Audio client{4, 2}; // audio & midi ports;

    //needs function to load different files and link between them!
    std::string file;
    if (!argv[1]) file = "view.xml";
    else file = argv[1];

    XmlParser document{file, client, window.getRenderer()};

    window.loop(&document);

    return 0;
}