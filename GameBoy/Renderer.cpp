#include "Renderer.h"

Renderer::Renderer(Input* joypad)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    if (!SDL_CreateWindowAndRenderer("GameBoy", GB_WIDTH * SCALE, GB_HEIGHT * SCALE, SDL_WINDOW_RESIZABLE, &win, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, GB_WIDTH, GB_HEIGHT);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return;
    }

    ShouldRun = true;
    this->joypad = joypad;

    lastCounter = SDL_GetPerformanceCounter();

}

void SDLCALL Renderer::CallbackFunk(void* userdata, const char* const* filelist, int filter) {

    if (!filelist) {
        SDL_Log("An error occured: %s", SDL_GetError());
        return;
    }

    else if (!*filelist) {
        SDL_Log("The user did not select any file.");
        SDL_Log("Most likely, the dialog was canceled.");
        return;
    }

    std::string location = *filelist;

    Renderer* self = static_cast<Renderer*>(userdata);
    self->path = location;
}

u8 Renderer::Decode(int code)
{
    switch (code)
    {
    case SDLK_Z:  return 1;
    case SDLK_X: return 2;
    case SDLK_BACKSPACE: return 3;
    case SDLK_RETURN: return 4;
    case SDLK_RIGHT: return 5;
    case SDLK_LEFT: return 6;
    case SDLK_UP: return 7;
    case SDLK_DOWN: return 8;
    }
}

void Renderer::Event()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        {
            if (event.key.repeat == true) {
                break;
            };
            if (event.key.key == SDLK_0)
            {
                NextPalette();
                break;
            }
            if (event.key.key == SDLK_9)
            {   
                PreviousPalette();
                break;
            }
            if (event.key.key == SDLK_8)
            {
                SDL_ShowOpenFileDialog(Renderer::CallbackFunk, this, win, &Filter, 1, nullptr, 0);
                break;
            }
            joypad->Press(Decode(event.key.key));
            break;
        }
        case SDL_EVENT_KEY_UP:
            if (event.key.repeat == true) {
                break;
            };
            joypad->Release(Decode(event.key.key));
            break;
        case SDL_EVENT_QUIT:
            ShouldRun = false;
            break;
        }
    }
}

void Renderer::ShowFPS()
{
    frameCount++;

    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - last >= 1000)
    {
        fps = frameCount * 1000.0f / (currentTime - last);

        frameCount = 0;
        last = currentTime;

        std::stringstream title;
        title << "GameBoy Emulator - FPS: " << fps;

        SDL_SetWindowTitle(win, title.str().c_str());
    }
}

void Renderer::LimitFPS()
{
    u64 currentCounter = SDL_GetPerformanceCounter();

    double elapsed = (double)(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();

    while (elapsed < Target)
    {
        currentCounter = SDL_GetPerformanceCounter();
        elapsed = (double)(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
    }

    lastCounter = SDL_GetPerformanceCounter();
}

void Renderer::Step(const Shade* shade)
{
    for (int i = 0; i < GB_WIDTH * GB_HEIGHT; i++)
        FrameBuffer[i] = PALETTES[CurrentPaletteIndex].colors[static_cast<u8>(shade[i])];

    SDL_UpdateTexture(texture, nullptr, FrameBuffer, GB_WIDTH * sizeof(u32));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    LimitFPS();
    ShowFPS();
}
