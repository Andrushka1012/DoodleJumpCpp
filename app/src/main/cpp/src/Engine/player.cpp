#include "engine/player.h"

namespace DoodleJumpGame {

    void Player::renderOn(Renderer *renderer) {
        if (!renderer || !visible) {
            return;
        }
        
        // Set player color (green)
        renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
        
        // Set transform: position and size
        renderer->setTransform(getX(), getY(), width / 100.0f, height / 100.0f);
        
        // Draw the player as a quad
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