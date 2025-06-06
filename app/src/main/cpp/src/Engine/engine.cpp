#include "engine/engine.h"
#include "engine/renderer.h"
#include "engine/player.h"
#include <memory>
#include <chrono>
#include <random>

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

        highestPlatformY = 0.0f;
        platforms.clear();
        items.clear();
        platforms.emplace_back(0.0f, 0.0, PlatformType::Normal); // Start with a single platform at the center
        spawnPlatforms();
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
        float normalizedOffset = screenController.normalizeCameraY();
        player.normalizeY(normalizedOffset);

        for (Platform &platform: platforms) {
            platform.normalizeY(normalizedOffset);
        }
        highestPlatformY -= normalizedOffset;
    }

    void Engine::updateGamePosition() {
        removeInvisibleObjects();
        spawnPlatforms();

        float deltaTime = calculateDeltaTime();
        player.update(deltaTime);

        for (Platform &platform: platforms) {
            platform.update(deltaTime);

            if (platform.isVisible && platform.isColliding(player) && player.isFalling()) {
                player.jump();
                platform.touch();
            }
        }

        for (Item &item: items) {
            if (item.isVisible && item.isColliding(player)) {
                item.interact(player);
            }
        }


        screenController.adjustCameraPosition(player.getPosition());

    }

    void Engine::removeInvisibleObjects() {
        if (player.getPosition().isBelow(screenController.getCameraY() - GameConstants::PLAYER_END_OF_GAME_FALLING_OFFSET)) {
            onGameOverCallback(screenController.getMaxCameraHeightRecord() * 10.0f);
            return;
        }

        float cameraBottom = screenController.getCameraY() + GameConstants::CAMERA_REMOVE_OBJECT_OFFSET_Y;
        platforms.erase(
                std::remove_if(platforms.begin(), platforms.end(),
                               [cameraBottom](const Platform &platform) {
                                   return platform.getPosition().y < cameraBottom;
                               }),
                platforms.end()
        );
        items.erase(
                std::remove_if(items.begin(), items.end(),
                               [cameraBottom](const Item &item) {
                                   return item.getPosition().y < cameraBottom;
                               }),
                items.end()
        );
    }

    void Engine::spawnPlatforms() {
        int required = desiredPlatformCount();
        if (platforms.size() >= required) return;

        float difficultyCoef =
                (float) (GameConstants::BASE_PLATFORM_AMOUNT - required) /
                (GameConstants::BASE_PLATFORM_AMOUNT - GameConstants::MINIMAL_PLATFORM_AMOUNT);
        difficultyCoef = std::clamp(difficultyCoef, 0.25f, 1.0f);

        while (platforms.size() < required) {
            float newX = randomFloat(-0.9f, 0.9f);  // Clean horizontal range
            float baseGap = randomFloat(GameConstants::MIN_PLATFORM_GAP, GameConstants::MAX_PLATFORM_GAP);
            float gap = baseGap * difficultyCoef;
            float newY = highestPlatformY + gap;


            float itemChance = randomFloat(0.0f, 1.0f);
            if (itemChance > GameConstants::CHANGE_TO_SPAWN_BONUS && screenController.isCameraAbove(2.0f)) {
                ItemType itemType = randomBool() ? ItemType::Rocket : ItemType::BlackHole;
                items.emplace_back(newX, newY, itemType);
                continue;
            }

            auto type = getRandomPlatformType();
            float offset = 0;
            if (type == PlatformType::Moving) {
                offset = randomBool() ? -1 : 1;
            }

            Platform candidate(newX, newY, type, offset);

            if (!isOverlapping(candidate)) {
                platforms.push_back(candidate);
                highestPlatformY = std::max(highestPlatformY, newY);
            }
        }
    }


    int Engine::desiredPlatformCount() const {
        float heightFactor = screenController.getMaxCameraHeightRecord() * 2; // Remove 2 after one screen height units

        return std::max(static_cast<int>(GameConstants::BASE_PLATFORM_AMOUNT - heightFactor), GameConstants::MINIMAL_PLATFORM_AMOUNT);
    }

    PlatformType Engine::getRandomPlatformType() {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_int_distribution<int> dist(0, 3);

        return static_cast<PlatformType>(dist(rng));
    }

    bool Engine::isOverlapping(const Platform &newPlatform) const {
        bool isOverlappingPlatforms = std::any_of(platforms.begin(), platforms.end(), [&](const Platform &platform) {
            return platform.isColliding(newPlatform);
        });

        bool isOverlappingItems = std::any_of(items.begin(), items.end(), [&](const Item &item) {
            return item.isColliding(newPlatform);
        });


        return isOverlappingPlatforms || isOverlappingItems;
    }

    void Engine::drawObjects() {
        if (renderer) {
            renderer->clear(0.0f, 0.0f, 0.0f, 1.0f);

            for (const Platform &platform: platforms) {
                if (platform.isVisible) {
                    draw(platform);
                }
            }

            for (const Item &item: items) {
                if (item.isVisible) {
                    draw(item);
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

    float Engine::randomFloat(float min, float max) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    bool Engine::randomBool() {
        static std::mt19937 rng(std::random_device{}());
        static std::bernoulli_distribution dist(0.5); // 0.5 = 50% chance true

        return dist(rng);
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
