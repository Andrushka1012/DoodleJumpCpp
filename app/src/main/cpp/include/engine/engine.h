#pragma once

#include "renderer.h"
#include "memory"

namespace DoodleJumpGame {
    class Engine {
    public:
        explicit Engine(std::unique_ptr<Renderer> renderer);

        ~Engine();

        void start();
        void drawFrame();
        void setViewport(int width, int height);

    private:
        std::unique_ptr<Renderer> renderer;
    };

    // C-style API for JNI compatibility
    void startEngine();

    void drawFrame();

    void setViewport(int width, int height);
} 