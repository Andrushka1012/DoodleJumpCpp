#pragma once

#include "renderer.h"
#include "memory"
#include "player.h"
#include "camera.h"

namespace DoodleJumpGame {
    class Engine {
    public:
        explicit Engine(std::unique_ptr<Renderer> renderer);

        ~Engine();

        void start();
        void drawFrame();
        void setViewport(int width, int height);

    private:
        static constexpr float Y_NORMALIZE_THRESHOLD = 10000.0f;

        std::unique_ptr<Renderer> renderer;
        Player player;
        Camera camera;

        void initializeGame();
        void normalizeYPosition();
    };

    // C-style API for JNI compatibility
    void startEngine();

    void drawFrame();

    void setViewport(int width, int height);

} 