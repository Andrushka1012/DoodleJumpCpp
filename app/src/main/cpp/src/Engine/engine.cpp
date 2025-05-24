#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/player.h"
#include <memory>

namespace Engine {
    // Global renderer instance for JNI compatibility
    std::unique_ptr<Renderer> g_renderer;

    Engine::Engine(std::unique_ptr<Renderer> renderer)
            : renderer(std::move(renderer)) {
        // You can add initialization logic here if needed
    }

    Engine::~Engine() = default; // Or define custom cleanup logic if necessary


    // C-style API implementation
    void startEngine() {
        if (!g_renderer) {
            g_renderer = std::make_unique<Renderer>();
        }
        g_renderer->initialize();
    }

    void renderFrame() {
        if (g_renderer) {
            g_renderer->render();
        }
    }

    void setViewport(int width, int height) {
        if (g_renderer) {
            g_renderer->setViewport(width, height);
        }
    }
}
