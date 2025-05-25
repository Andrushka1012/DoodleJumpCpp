#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/player.h"
#include <memory>
#include <chrono>

namespace DoodleJumpGame {
    // Global renderer instance for JNI compatibility
    std::unique_ptr<Engine> g_engine;

    Engine::Engine(std::unique_ptr<Renderer> renderer, GameOverCallback callback)
            : renderer(std::move(renderer)), onGameOverCallback(std::move(callback)) {
    }

    Engine::~Engine() = default; // Or define custom cleanup logic if necessary

    void Engine::launch() {
        if (!renderer || !renderer->initialize()) {
            // TODO: Handle draw initialization failure
            return;
        }
        screenController = Camera(0.0f, GameConstants::WORLD_HEIGHT);
    }

    void Engine::startGame() {
        player = Player(0.0f, 0.0f);
        screenController.reset();

        platforms.clear();
        platforms.emplace_back(0.0f, 0.0, PlatformType::Normal);
        platforms.emplace_back(0.5f, 0.1, PlatformType::Crumbling);
        platforms.emplace_back(-0.5f, 0.4, PlatformType::Moving, 1);
        platforms.emplace_back(0.5f, 0.6, PlatformType::Falling);
        platforms.emplace_back(-0.5f, 0.8, PlatformType::Normal);
        platforms.emplace_back(0.5f, 1.0, PlatformType::Crumbling);
        platforms.emplace_back(-0.5f, 1.2, PlatformType::Normal);
        platforms.emplace_back(0.5f, 1.4, PlatformType::Normal);
        platforms.emplace_back(-0.5f, 1.6, PlatformType::Crumbling);
    }

    void Engine::setViewport(int width, int height) {
        if (renderer) {
            renderer->setViewport(width, height);
            screenController.setAspectRatio(static_cast<float>(width) / height);
        }
    }

    void Engine::onHorizontalMove(float x) {
        player.setHorizontalMovement(x);
    }

    void Engine::drawFrame() {
        if (screenController.isCameraAbove(Y_NORMALIZE_THRESHOLD)) {
            normalizeYPosition();
        }

        updateGamePosition();
        drawObjects();
    }

    void Engine::normalizeYPosition() {
        // TODO: Implement Y position normalization logic
    }

    void Engine::updateGamePosition() {
        removeInvisibleObjects();

        float deltaTime = calculateDeltaTime();
        player.update(deltaTime);

        for (Platform& platform: platforms) {
            platform.update(deltaTime);

            if (platform.isVisible && platform.isColliding(player) && player.isFalling()) {
                player.jump();
                platform.touch();
            }
        }


        screenController.adjustCameraPosition(player.getPosition());

    }

    void Engine::removeInvisibleObjects() {
        // TODO: Implement logic to remove platforms that are no longer visible on camera or isVisible = false

        if (player.getPosition().isBelow(screenController.getY() - GameConstants::PLAYER_END_OF_GAME_FALLING_OFFSET)) {
            onGameOverCallback(screenController.getMaxCameraHeightRecord() * 10.0f);
            return;
        }
    }

    void Engine::drawObjects() {
        if (renderer) {
            renderer->clear(0.0f, 0.0f, 0.0f, 1.0f);

            for (const Platform &platform: platforms) {
                if (platform.isVisible) {
                    draw(platform);
                }
            }

            draw(player);
        }
    }


    float Engine::calculateDeltaTime() {
        auto currentTime = std::chrono::steady_clock::now();

        static bool firstFrame = true;
        if (firstFrame) {
            lastFrameTime = currentTime;
            firstFrame = false;
            return 0.016f; // Initial delta time for the first frame (assuming 60 FPS)
        }

        auto duration = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        float deltaTime = std::chrono::duration<float>(duration).count();

        return std::min(deltaTime, 0.033f); // Cap delta time to 30 FPS (1/30 seconds)
    }

    void Engine::draw(const GameObject &renderable) {
        if (renderer) {
            auto renderObject = renderable.getRenderObject();
            auto adjustedObject = screenController.adjustToScreen(renderObject);

            renderer->draw(adjustedObject);
        }
    }

    // C-style API implementation
    void startEngine(const GameOverCallback &onGameOverCallback) {
        if (!g_engine) {
            auto renderer = std::make_unique<Renderer>();
            g_engine = std::make_unique<Engine>(std::move(renderer), onGameOverCallback);
            g_engine->launch();
        }
    }

    void drawFrame() {
        if (g_engine) {
            g_engine->drawFrame();
        }
    }

    void setViewport(int width, int height) {
        if (g_engine) {
            g_engine->setViewport(width, height);
        }
    }

    void onHorizontalMove(float x) {
        if (g_engine) {
            g_engine->onHorizontalMove(x);
        }
    }

    void startGame() {
        if (g_engine) {
            g_engine->startGame();
        }
    }
}
