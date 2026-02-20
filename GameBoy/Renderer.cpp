#include "Renderer.h"
namespace {
    static constexpr u32 palette[4] =
    {
       0xFFFFFFFF,
       0xFFAAAAAA,
       0xFF555555,
       0xFF000000
    };
}

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
            if (event.key.repeat == true) {
                break;
            };
            joypad->Press(Decode(event.key.key));
            break;
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

void Renderer::Step(const Shade* shade)
{
    Event();

    for (int i = 0; i < GB_WIDTH * GB_HEIGHT; i++)
        FrameBuffer[i] = palette[static_cast<u8>(shade[i])];

    SDL_UpdateTexture(texture, nullptr, FrameBuffer, GB_WIDTH * sizeof(u32));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    ShowFPS();
}
