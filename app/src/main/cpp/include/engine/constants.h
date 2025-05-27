#pragma once

namespace DoodleJumpGame::GameConstants {
    // World dimensions
    constexpr float WORLD_HEIGHT = 2.0f;

    // Player constants
    constexpr float PLAYER_WIDTH = 0.15f;
    constexpr float PLAYER_HEIGHT = 0.2f;
    constexpr float PLAYER_GRAVITY = -2.2f;
    constexpr float PLAYER_JUMP_FORCE = 1.5f;
    constexpr float PLAYER_HORIZONTAL_MOVE_SPEED = 4;
    constexpr float PLAYER_END_OF_GAME_FALLING_OFFSET = 2;
    constexpr float PLAYER_ROCKET_FORCE = 2.5f;
    constexpr float PLAYER_ROCKET_TIME = 5.0f;


    // Platform constants
    constexpr float PLATFORM_WIDTH = 0.25;
    constexpr float PLATFORM_HEIGHT = 0.05f;
    constexpr float PLATFORM_MOVING_SPEED = 1.0f;
    constexpr float PLATFORM_FALLING_SPEED = 1.5f;
    constexpr int BASE_PLATFORM_AMOUNT = 45;
    constexpr int MINIMAL_PLATFORM_AMOUNT = 10;
    constexpr float MIN_PLATFORM_GAP = 0.1f;
    constexpr float MAX_PLATFORM_GAP = 0.3f;


    // Camera constants
    constexpr float CAMERA_OFFSET_Y = -0.7f;
    constexpr float CAMERA_FOLLOW_OFFSET_Y = 0.55f;
    constexpr float CAMERA_MAX_FOLLOW_DOWN = 1;
    constexpr float CAMERA_REMOVE_OBJECT_OFFSET_Y = -0.3;

    // Items
    constexpr float CHANGE_TO_SPAWN_BONUS = 0.97f;
    constexpr float ROCKET = 0.2f;
    constexpr float BACK_HOLE = 0.4f;

}

