#include <iostream>
#include <functional>
#include "Graphics.h"

/*
 * Now what?
 * First, it should switch (again) to a more low level API such as SDL. Raylib is proving to be
 * hard on manipulating things like cropped boxes and texts. must do this
 * before implementing Lua theming. Good thing is that all the systems are WELL separated, so
 * it'll not impact on anything else.
 * 
 * another one is a drag & drop interaction between components, like DirTree and WaveTable;
 * 
 * got the idea of using Lua for theming. Will work something like a CSS file, with objects w/
 * key-value pairs for component types, background, "classes", and there'll be a precedence, e.g:
 * - apply "global" styles first: background, component types;
 * - apply "class" styles second: components with the "class" attribute, including containers;
 * Lua state will cook those objects into structs and hash maps, and the XmlParser will fetch 
 * all matching data at render time. Any change at the theme file should cause a re-rendering.
 */

int window_width;

int main(int argc, const char* argv[])
{
    window_width = 640;
    Graphics window{640, 480}; //size;
    Audio client{4, 2}; // audio & midi ports;

    //needs function to load different files and link between them!
    std::string file;
    if (!argv[1]) file = "view.xml";
    else file = argv[1];

    XmlParser document{file, client};

    window.loop(&document);

    return 0;
}