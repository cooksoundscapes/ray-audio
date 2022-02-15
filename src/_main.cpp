#include <iostream>
#include <functional>
#include "Graphics.h"

/*
 * Now what?
 * got the idea of using Lua for theming. Will work something like a CSS file, with objects w/
 * key-value pairs for component types, background, "classes", and there'll be a precedence, e.g:
 * - apply "global" styles first: background, component types;
 * - apply "class" styles second: components with the "class" attribute, including containers;
 * Lua state will cook those objects into structs and hash maps, and the XmlParser will fetch 
 * all matching data at render time. Any change at the theme file should cause a re-rendering.
 */
int main(int argc, const char* argv[])
{
    Graphics window{1024, 768}; //size;
    Audio client{4, 2}; // audio & midi ports;

    //needs function to load different files and link between them!
    std::string file;
    if (!argv[1]) file = "view.xml";
    else file = argv[1];

    XmlParser document{file, client};

    window.loop(&document);

    return 0;
}