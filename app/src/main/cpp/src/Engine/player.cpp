#include "engine/player.h"

namespace DoodleJumpGame {

    RenderObject Player::getRenderObject() const{
        return {getX(), getY(), width, height, RenderColor(0.0f, 1.0f, 0.0f, 1.0f)};
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

    void Player::normalizeY(float offset) {
        setY(getY() - offset);
    }
}