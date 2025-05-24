#pragma once

#include "renderer.h"
#include "memory"

namespace DoodleJumpGame {
    class Engine {
    public:
        explicit Engine(std::unique_ptr<Renderer> renderer);

        ~Engine();

    private:
        std::unique_ptr<Renderer> renderer;
    };

    // C-style API for JNI compatibility
    void startEngine();

    void renderFrame();

    void setViewport(int width, int height);
} 