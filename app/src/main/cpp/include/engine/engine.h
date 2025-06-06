#pragma once

#include "renderer.h"
#include "memory"
#include "player.h"
#include "screen-controller.h"
#include "platform.h"
#include "item.h"
#include "game-object.h"
#include <vector>
#include <functional>

namespace DoodleJumpGame {
    using GameOverCallback = std::function<void(float)>;

    class Engine {
    public:
        explicit Engine(std::unique_ptr<Renderer> renderer, GameOverCallback callback);

        ~Engine();

        void launch();
        void startGame();
        void drawFrame();
        void setViewport(int width, int height);
        void onHorizontalMove(float x);

    private:
        static constexpr float Y_NORMALIZE_THRESHOLD = 1000.0f;

        std::chrono::steady_clock::time_point lastFrameTime;

        std::unique_ptr<Renderer> renderer;
        GameOverCallback onGameOverCallback;
        Player player;
        Camera screenController;
        std::vector<Platform> platforms;
        std::vector<Item> items;
        float highestPlatformY = 0.0f;

        void normalizeYPosition();
        void updateGamePosition();
        void removeInvisibleObjects();
        void drawObjects();
        void draw(const GameObject &renderable);
        float calculateDeltaTime();

        [[nodiscard]] int desiredPlatformCount() const;
        void spawnPlatforms();

        static static float randomFloat(float min, float max);

        static PlatformType getRandomPlatformType();

        static bool randomBool();

        bool isOverlapping(const Platform &newPlatform) const;
    };

    // C-style API for JNI compatibility
    void startEngine(const GameOverCallback& onGameOverCallback);

    void drawFrame();

    void setViewport(int width, int height);

    void onHorizontalMove(float x);

    void startGame();
} 