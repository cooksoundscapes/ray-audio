#include "SDL_Graphics.h"

SDL_Engine::SDL_Engine(int w, int h) : width{w}, height{h}
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cerr << SDL_GetError() << '\n';
    window = SDL_CreateWindow(
        "display::", 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
    );
    if (window == NULL)
        std::cerr << SDL_GetError() << '\n';
        
    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); 
    if (renderer == NULL)
        std::cerr << SDL_GetError() << '\n';   
}

SDL_Engine::~SDL_Engine() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
    SDL_Quit();
    std::cout << "Booh bye!\n";
}

void SDL_Engine::beginDrawing()
{
    void* pixels;
    int pitch;

    SDL_LockTexture(finalCanvas, NULL, &pixels, &pitch);

    cairo_surface_t *surface = cairo_image_surface_create_for_data(
        (unsigned char*) pixels,
        CAIRO_FORMAT_ARGB32,
        width, height, pitch
    );

    ctx = cairo_create(surface);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    cairo_set_source_rgba(ctx, 0, 0, 0, 1.0);
    cairo_rectangle(ctx, 0, 0, width, height);
    cairo_fill(ctx);
}

void SDL_Engine::endDrawing()
{
    SDL_UnlockTexture(finalCanvas);
    SDL_RenderCopy(renderer, finalCanvas, NULL, NULL);
    SDL_RenderPresent(renderer);
}   

void SDL_Engine::loop(XmlParser* document)
{
    finalCanvas = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );
    bool shouldClose{false};
    SDL_Event handler;

    while (!shouldClose) {
        while (SDL_PollEvent(&handler)) {
            if (handler.type == SDL_QUIT) {
                shouldClose = true;
                break;
            }
        }
        beginDrawing();
        
        document->drawComponents();

        endDrawing();
    }
}