#include "engine/player.h"

namespace DoodleJumpGame {

    void Player::renderOn(Renderer *renderer, Camera camera) {
        if (!renderer || !visible) {
            return;
        }

        Position onScreenPosition = camera.transformToOnScreenPosition(getPosition());

        renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
        renderer->setTransform(
                onScreenPosition.x,
                onScreenPosition.y,
                camera.transformToOnScreenSize(width),
                camera.transformToOnScreenSize(height),
                0.0f
        );
        renderer->drawQuad();
    }

    void Player::update(float deltaTime) {
        velocityY += GRAVITY * deltaTime;
        setY(getY() + velocityY * deltaTime);
    }

    void Player::jump() {
        velocityY = JUMP_FORCE;
    }
}