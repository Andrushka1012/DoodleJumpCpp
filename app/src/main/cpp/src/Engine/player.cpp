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
                camera.transformToOnScreenWidth(width),
                camera.transformToOnScreenHeight(height),
                0.0f
        );
        renderer->drawQuad();
    }

    void Player::update(float deltaTime) {
        velocityY += GameConstants::PLAYER_GRAVITY * deltaTime;
        setY(getY() + velocityY * deltaTime);

        if (horizontalMovement != 0.0f) {
            float moveSpeed = GameConstants::PLAYER_HORIZONTAL_MOVE_SPEED;
            setXWithScreenWrap(getX() + horizontalMovement * moveSpeed * deltaTime);
        }
    }

    void Player::jump() {
        velocityY = GameConstants::PLAYER_JUMP_FORCE;
    }
}