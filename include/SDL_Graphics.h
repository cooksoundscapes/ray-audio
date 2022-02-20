#pragma once
#include <SDL2/SDL.h>
#include <cairo/cairo.h>
#include "XmlParser.h"

class SDL_Engine
{
public:
    SDL_Engine(int w, int h);
    ~SDL_Engine();

    SDL_Renderer* getRenderer() {return renderer;}
    void loop(XmlParser*);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;  

    void beginDrawing();
    void endDrawing();

    cairo_t* ctx;
    SDL_Texture* finalCanvas;
};