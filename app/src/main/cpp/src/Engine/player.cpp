#include "engine/player.h"

namespace DoodleJumpGame {


    void Player::renderOn(DoodleJumpGame::Renderer *render) {

    }

    void Player::update(float deltaTime) {
        velocityY -= GRAVITY * deltaTime;
        y += velocityY * deltaTime;

        // IF the player is below the ground, reset position and apply jump force
        if (y < 0.0f) {
            y = 0.0f;
            jump();
        }
    }

    void Player::jump() {
        velocityY = JUMP_FORCE;
    }
}