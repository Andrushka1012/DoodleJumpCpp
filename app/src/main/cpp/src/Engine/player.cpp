#include "engine/player.h"

namespace DoodleJumpGame {


    void Player::renderOn(DoodleJumpGame::Renderer *render) {

    }

    void Player::update(float deltaTime) {
        velocityY -= GRAVITY * deltaTime;
        setY(velocityY * deltaTime);

        // IF the player is below the ground, reset position and apply jump force
        if (getY() < 0.0f) {
            setY(0.0f);
            jump();
        }
    }

    void Player::jump() {
        velocityY = JUMP_FORCE;
    }
}