#pragma once
#include <SDL2/SDL.h>
#include "XmlParser.hpp"
#include "CairoLib.hpp"

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

    SDL_Texture* finalCanvas;
};