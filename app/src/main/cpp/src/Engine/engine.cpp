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
        camera = Camera(0.0f, 0.0f);
    }

    void Engine::setViewport(int width, int height) {
        if (renderer) {
            renderer->setViewport(width, height);
        }
    }

    void Engine::drawFrame() {
        if (camera.isAbove(Y_NORMALIZE_THRESHOLD)) {
            normalizeYPosition();
        }

        player.update(0.016f);



        if (renderer) {
            renderer->render();
        }
    }

    void Engine::normalizeYPosition() {
        // TODO: Implement Y position normalization logic
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
