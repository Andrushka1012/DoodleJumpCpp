#include "engine/position.h"
#include "engine/screen-controller.h"
#include <algorithm>

namespace DoodleJumpGame {

    void Camera::adjustCameraPosition(const Position &position) {
        float cameraFollowOffsetY = cameraPosition.y + GameConstants::CAMERA_FOLLOW_OFFSET_Y;

        if (cameraFollowOffsetY < position.y) {
            cameraPosition.y = position.y - GameConstants::CAMERA_FOLLOW_OFFSET_Y;
            maxCameraHeight = cameraPosition.y;

        } else if (position.y < cameraPosition.y) {
            // Prevent camera from going below the maximum height
            float minDownPosition = maxCameraHeight - GameConstants::CAMERA_MAX_FOLLOW_DOWN;

            cameraPosition.y = std::max(position.y, minDownPosition);
        }
    }
}