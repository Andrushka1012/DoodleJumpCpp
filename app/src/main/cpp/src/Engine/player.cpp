#include "engine/player.h"

namespace DoodleJumpGame {

    void Player::renderOn(Renderer *renderer, float cameraOffsetY) {
        if (!renderer || !visible) {
            return;
        }

//        Position framePosition = getPosition() - cameraPosition;

        renderer->setColor(0.0f, 1.0f, 0.0f, 1.0f);
        renderer->setTransform(
                getX(),
                getY() ,
                width / 200,
                height / 200,
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