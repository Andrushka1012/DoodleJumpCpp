#include "engine/platform.h"

namespace DoodleJumpGame {

    RenderObject Platform::getRenderObject() const {
        return {getX(), getY(), width, height, RenderColor(1.0f, 0.0f, 0.0f, 1.0f)};
    }

    void Platform::update(float deltaTime) {
    }


}