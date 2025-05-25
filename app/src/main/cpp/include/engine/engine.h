#pragma once

#include "renderer.h"
#include "memory"
#include "player.h"
#include "camera.h"
#include "platform.h"
#include <vector>

namespace DoodleJumpGame {
    class Engine {
    public:
        explicit Engine(std::unique_ptr<Renderer> renderer);

        ~Engine();

        void start();
        void drawFrame();
        void setViewport(int width, int height);
        void onHorizontalMove(float x);

    private:
        static constexpr float Y_NORMALIZE_THRESHOLD = 10000.0f;

        std::chrono::steady_clock::time_point lastFrameTime;

        std::unique_ptr<Renderer> renderer;
        Player player;
        Camera camera;
        std::vector<Platform> platforms;

        void initializeGame();
        void normalizeYPosition();
        void updateGamePosition();
        void renderObjects();
        float calculateDeltaTime();
    };

    // C-style API for JNI compatibility
    void startEngine();

    void drawFrame();

    void setViewport(int width, int height);

    void onHorizontalMove(float x);

} 