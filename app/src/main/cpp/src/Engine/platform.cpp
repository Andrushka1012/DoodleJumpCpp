#include "engine/platform.h"

namespace DoodleJumpGame {

    void Platform::renderOn(Renderer *renderer, Camera camera) const {
        if (!renderer || !visible) {
            return;
        }

        Position onScreenPosition = camera.transformToOnScreenPosition(getPosition());

        renderer->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        renderer->setTransform(
                onScreenPosition.x,
                onScreenPosition.y,
                camera.transformToOnScreenWidth(width),
                camera.transformToOnScreenHeight(height),
                0.0f
        );
        renderer->drawQuad();
    }

    void Platform::update(float deltaTime) {
    }

}