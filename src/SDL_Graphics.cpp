#include "SDL_Graphics.hpp"

using namespace CairoLib;

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
    bool shouldClose{false};
    SDL_Event handler;

    finalCanvas = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );
    //set window name based on view file;
    std::string newName = "display::" + document->view_name;
    SDL_SetWindowTitle(window, newName.c_str());

    //first render;
    SDL_GetWindowSize(window, &document->win_w, &document->win_h);
    document->render();
    std::cout << "render success, entering main loop;\n";

    while (!shouldClose) {
        while (SDL_PollEvent(&handler)) {
            switch (handler.type)
            {
                case SDL_QUIT:
                    shouldClose = true;
                    break;
                case SDL_WINDOWEVENT:
                    if (handler.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetRendererOutputSize(renderer, &width, &height);
                        SDL_DestroyTexture(finalCanvas);
                        finalCanvas = SDL_CreateTexture(
                            renderer,
                            SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING,
                            width, height
                        );
                        document->win_w = width;
                        document->win_h = height;
                        document->render();
                    }
                    break;
                case SDL_MOUSEMOTION:
                    mouse_position = {
                        handler.motion.x,
                        handler.motion.y
                    };
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (handler.button.button == SDL_BUTTON_LEFT) {
                        MouseLeftButtonPressed = true;
                        mouse_delta = {
                            handler.button.x,
                            handler.button.y
                        };
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (handler.button.button == SDL_BUTTON_LEFT) {
                        MouseLeftButtonPressed = false;
                    }
                    break;
            }
        }
        beginDrawing();

        document->drawComponents();

        endDrawing();
    }
    SDL_DestroyTexture(finalCanvas);
    finalCanvas = NULL;
}