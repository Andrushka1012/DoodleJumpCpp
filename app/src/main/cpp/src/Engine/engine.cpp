#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/player.h"
#include <memory>

namespace DoodleJumpGame {
    // Global renderer instance for JNI compatibility
    std::unique_ptr<Engine> g_engine;

    Engine::Engine(std::unique_ptr<Renderer> renderer)
            : renderer(std::move(renderer)) {
    }

    Engine::~Engine() = default; // Or define custom cleanup logic if necessary

    void Engine::start() {
        if (!renderer || !renderer->initialize()) {
            // TODO: Handle render initialization failure
            return;
        }
        initializeGame();
    }

    void Engine::initializeGame() {
        player = Player(0.0f, 0.0f);
        camera = Camera(0.0f);
    }

    void Engine::setViewport(int width, int height) {
        if (renderer) {
            renderer->setViewport(width, height);
            camera.setAspectRatio(static_cast<float>(width) / height);
        }
    }

    void Engine::drawFrame() {
        if (camera.isAbove(Y_NORMALIZE_THRESHOLD)) {
            normalizeYPosition();
        }

        updateGamePosition();
        renderObjects();
    }

    void Engine::normalizeYPosition() {
        // TODO: Implement Y position normalization logic
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

    void Engine::updateGamePosition() {
        float deltaTime = calculateDeltaTime();

        player.update(deltaTime);

        if (camera.isAbove(player.getPosition())) {
            player.jump();
        }

    }

    void Engine::renderObjects() {
        if (renderer) {
            renderer->clear(0.0f, 0.0f, 0.0f, 1.0f);
            player.renderOn(renderer.get(), camera);
        }
    }


    // C-style API implementation
    void startEngine() {
        if (!g_engine) {
            auto renderer = std::make_unique<Renderer>();
            g_engine = std::make_unique<Engine>(std::move(renderer));
        }
        g_engine->start();
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
}
