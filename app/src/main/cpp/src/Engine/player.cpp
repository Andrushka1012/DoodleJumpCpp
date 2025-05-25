#include "engine/player.h"

namespace DoodleJumpGame {


    void Player::renderOn(DoodleJumpGame::Renderer *render) {

    }

    void Player::update(float deltaTime) {
        velocityY -= GRAVITY * deltaTime;
        setY(velocityY * deltaTime);
    }

    void Player::jump() {
        velocityY = JUMP_FORCE;
    }
}