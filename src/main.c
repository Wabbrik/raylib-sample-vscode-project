#include "raylib.h"
#include "raymath.h"

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define G                       400
#define PLAYER_JUMP_SPEED       350.0f
#define PLAYER_HORIZONTAL_SPEED 200.0f

#define PLAYER_RECTANGLE_SIZE 40.0f

#define SCREEN_WIDTH       800
#define SCREEN_HEIGHT      450
#define HALF_SCREEN_WIDTH  SCREEN_WIDTH / 2.0
#define HALF_SCREEN_HEIGHT SCREEN_HEIGHT / 2.0

typedef struct BoundingBox2D {
    Vector2 min;
    Vector2 max;
} BoundingBox2D;

typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
} Player;

typedef struct EnvironmentItem {
    Rectangle rectangle;
    bool blocking;
    Color color;
} EnvironmentItem;

typedef struct GameState {
    Player player;
    EnvironmentItem* environmentItems;
    int environmentItemsLength;
    BoundingBox2D environmentBoundingBox;
    Camera2D camera;
} GameState;

typedef struct InputState {
    bool moveLeft;
    bool moveRight;
    bool jump;
    bool resetPositionAndCamera;
    float zoomDelta;
} InputState;

BoundingBox2D CalculateEnvironmentBoundingBox(EnvironmentItem* envItems, int envItemsLength);
InputState ReadInputState();
void Draw(GameState*);
void UpdateGameState(GameState*, InputState*, float deltaTime);
void DrawPlayer(Player*);
void UpdatePlayer(GameState*, InputState*, float deltaTime);
void UpdateCameraCenterInsideMap(GameState*);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - 2d camera");

    EnvironmentItem environmentItems[]
        = {{{0, 0, 1000, 400}, false, LIGHTGRAY},
           {{0, 400, 1000, 200}, true, GRAY},
           {{300, 200, 400, 10}, true, GRAY},
           {{250, 300, 100, 10}, true, GRAY},
           {{650, 300, 100, 10}, true, GRAY}};

    int environmentItemsLength = COUNT_OF(environmentItems);

    Vector2 playerPosition = {HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT};
    GameState gameState = {
        .player =
            (Player) {
                .position = playerPosition,
                .speed = 0,
                .canJump = false,
            },
        .environmentItems = environmentItems,
        .environmentItemsLength = environmentItemsLength,
        .environmentBoundingBox = CalculateEnvironmentBoundingBox(environmentItems, environmentItemsLength),
        .camera =
            (Camera2D) {
                .target = playerPosition,
                .offset = (Vector2) {HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT},
                .rotation = 0.0f,
                .zoom = 1.0f,
            },
    };

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        InputState inputState = ReadInputState();

        UpdateGameState(&gameState, &inputState, deltaTime);

        Draw(&gameState);
    }

    CloseWindow();

    return 0;
}

InputState ReadInputState() {
    InputState inputState = {
        .moveLeft = IsKeyDown(KEY_LEFT),
        .moveRight = IsKeyDown(KEY_RIGHT),
        .jump = IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP),
        .resetPositionAndCamera = IsKeyPressed(KEY_R),
        .zoomDelta = GetMouseWheelMove(),
    };
    return inputState;
}

void Draw(GameState* gameState) {
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        BeginMode2D(gameState->camera);
        {
            for (int i = 0; i < gameState->environmentItemsLength; i++) {
                DrawRectangleRec(gameState->environmentItems[i].rectangle, gameState->environmentItems[i].color);
            }

            DrawPlayer(&gameState->player);
        }
        EndMode2D();

        DrawText("Controls:", 20, 20, 10, BLACK);
        DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
        DrawText("- Space or up to jump", 40, 60, 10, DARKGRAY);
        DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
    }
    EndDrawing();
}

void UpdateGameState(GameState* gameState, InputState* inputState, float deltaTime) {
    UpdatePlayer(gameState, inputState, deltaTime);

    gameState->camera.zoom += inputState->zoomDelta * 0.05f;
    gameState->camera.zoom = Clamp(gameState->camera.zoom, 0.25f, 3.0f);

    if (inputState->resetPositionAndCamera) {
        gameState->camera.zoom = 1.0f;
        gameState->player.position = (Vector2) {HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT};
        gameState->player.speed = 0;
    }

    UpdateCameraCenterInsideMap(gameState);
}

void DrawPlayer(Player* player) {
    Rectangle playerRect = {
        player->position.x - PLAYER_RECTANGLE_SIZE / 2.0,
        player->position.y - PLAYER_RECTANGLE_SIZE,
        PLAYER_RECTANGLE_SIZE,
        PLAYER_RECTANGLE_SIZE,
    };
    DrawRectangleRec(playerRect, RED);
    DrawCircle(player->position.x, player->position.y, 5, GOLD);
}

void UpdatePlayer(GameState* gameState, InputState* inputState, float deltaTime) {
    if (inputState->moveLeft) {
        gameState->player.position.x -= PLAYER_HORIZONTAL_SPEED * deltaTime;
    }
    if (inputState->moveRight) {
        gameState->player.position.x += PLAYER_HORIZONTAL_SPEED * deltaTime;
    }
    if (inputState->jump && gameState->player.canJump) {
        gameState->player.speed = -PLAYER_JUMP_SPEED;
        gameState->player.canJump = false;
    }

    bool hitObstacle = false;
    for (int i = 0; i < gameState->environmentItemsLength; i++) {
        EnvironmentItem* item = gameState->environmentItems + i;
        Vector2* playerPosition = &(gameState->player.position);
        if (item->blocking && item->rectangle.x <= playerPosition->x
            && item->rectangle.x + item->rectangle.width >= playerPosition->x && item->rectangle.y >= playerPosition->y
            && item->rectangle.y <= playerPosition->y + gameState->player.speed * deltaTime) {
            hitObstacle = true;
            gameState->player.speed = 0.0f;
            playerPosition->y = item->rectangle.y;
            break;
        }
    }

    if (!hitObstacle) {
        gameState->player.position.y += gameState->player.speed * deltaTime;
        gameState->player.speed += G * deltaTime;
        gameState->player.canJump = false;
    } else {
        gameState->player.canJump = true;
    }
}

BoundingBox2D CalculateEnvironmentBoundingBox(EnvironmentItem* envItems, int envItemsLength) {
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;
    for (int i = 0; i < envItemsLength; i++) {
        EnvironmentItem* ei = envItems + i;
        minX = fminf(ei->rectangle.x, minX);
        maxX = fmaxf(ei->rectangle.x + ei->rectangle.width, maxX);
        minY = fminf(ei->rectangle.y, minY);
        maxY = fmaxf(ei->rectangle.y + ei->rectangle.height, maxY);
    }
    BoundingBox2D boundingBox2D = {
        .max = (Vector2) {maxX, maxY},
        .min = (Vector2) {minX, minY},
    };
    return boundingBox2D;
}

void UpdateCameraCenterInsideMap(GameState* gameState) {
    gameState->camera.target = gameState->player.position;
    gameState->camera.offset = (Vector2) {HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT};

    Vector2 max = GetWorldToScreen2D(gameState->environmentBoundingBox.max, gameState->camera);
    Vector2 min = GetWorldToScreen2D(gameState->environmentBoundingBox.min, gameState->camera);

    gameState->camera.offset = (Vector2) {
        Clamp(gameState->camera.offset.x, SCREEN_WIDTH - (max.x - HALF_SCREEN_WIDTH), HALF_SCREEN_WIDTH - min.x),
        Clamp(gameState->camera.offset.y, SCREEN_HEIGHT - (max.y - HALF_SCREEN_HEIGHT), HALF_SCREEN_HEIGHT - min.y),
    };
}
