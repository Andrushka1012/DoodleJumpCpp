#include "engine/position.h"
#include "engine/camera.h"

namespace DoodleJumpGame {


    Position Camera::transformToOnScreenPosition(Position position) {
        return {
                position.x,
                position.y - cameraPosition.y - 0.7f
        };
    }
}