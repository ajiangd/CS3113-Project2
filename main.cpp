/**
* Author: Ashley Jiang
* Assignment: Pong Clone
* Date due: 10/16/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 900,
              SCREEN_HEIGHT = 456,
              FPS           = 60,
              PADDLE_SPEED  = 350;

constexpr Vector2 ORIGIN      = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
                  TABLE_SIZE  = {static_cast<float>(SCREEN_WIDTH),
                                 static_cast<float>(SCREEN_HEIGHT)},
                  PADDLE_SIZE = {80.0f, 78.0f},
                  PUCK_SIZE   = {36.0f, 36.0f};

constexpr char BG_COLOUR[] = "#3871E0";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

// Table variables
constexpr char TABLE_FP[] = "assets/table.png";

Texture2D gTableTexture;

// Paddle 1 variables (left side: W/S)
constexpr char    PADDLE1_FP[]     = "assets/paddle1.png";
constexpr Vector2 PADDLE1_INIT_POS = {60.0f, ORIGIN.y};

Texture2D gPaddle1Texture;
Vector2   gPaddle1Position = PADDLE1_INIT_POS,
          gPaddle1Movement = {0.0f, 0.0f},
          gPaddle1Scale    = PADDLE_SIZE;

// Paddle 2 variables (right side: up/down)
constexpr char    PADDLE2_FP[]     = "assets/paddle2.png";
constexpr Vector2 PADDLE2_INIT_POS = {SCREEN_WIDTH - 60.0f, ORIGIN.y};

Texture2D gPaddle2Texture;
Vector2   gPaddle2Position = PADDLE2_INIT_POS,
          gPaddle2Movement = {0.0f, 0.0f},
          gPaddle2Scale    = PADDLE_SIZE;

// Puck variables
constexpr char PUCK_FP[] = "assets/puck.png";

Texture2D gPuckTexture;
Vector2   gPuckPosition = ORIGIN,
          gPuckMovement = {0.0f, 0.0f},
          gPuckScale    = PUCK_SIZE;


void initialise();
void processInput();
void update();
void render();
void shutdown();
void renderObject(const Texture2D *texture, const Vector2 *position, const Vector2 *scale);
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB);
void confinePaddle(Vector2 *position, const Vector2 *scale);


/**
 * @brief Checks for a box-to-box collision between two objects.
 */
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabs(positionA->x - positionB->x) -
                      ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) -
                      ((scaleA->y + scaleB->y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

void renderObject(const Texture2D *texture, const Vector2 *position,
                  const Vector2 *scale)
{
    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    Rectangle destinationArea = {
        position->x, position->y,
        scale->x, scale->y
    };

    Vector2 originOffset = {scale->x / 2.0f, scale->y / 2.0f};

    DrawTexturePro(*texture, textureArea, destinationArea, originOffset, gAngle, WHITE);
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Air Hockey Pong");

    gTableTexture   = LoadTexture(TABLE_FP);
    gPaddle1Texture = LoadTexture(PADDLE1_FP);
    gPaddle2Texture = LoadTexture(PADDLE2_FP);
    gPuckTexture    = LoadTexture(PUCK_FP);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    
    gPaddle1Movement = {0.0f, 0.0f};
    gPaddle2Movement = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) gPaddle1Movement.y = -1.0f;
    else if (IsKeyDown(KEY_S)) gPaddle1Movement.y = 1.0f;

    if (IsKeyDown(KEY_UP))   gPaddle2Movement.y = -1.0f;
    if (IsKeyDown(KEY_DOWN))   gPaddle2Movement.y = 1.0f;

}

void confinePaddle(Vector2 *position, const Vector2 *scale){
    float halfScreenHeight = scale->y/2.0f;
    if (position->y - halfScreenHeight < 0.0f)
        position->y = halfScreenHeight;
    if (position->y + halfScreenHeight > SCREEN_HEIGHT)
        position->y = SCREEN_HEIGHT - halfScreenHeight;
}

void update()
{
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gPaddle1Position.y += PADDLE_SPEED * gPaddle1Movement.y * deltaTime;
    gPaddle2Position.y += PADDLE_SPEED * gPaddle2Movement.y * deltaTime;

    confinePaddle(&gPaddle1Position, &gPaddle1Scale);
    confinePaddle(&gPaddle2Position, &gPaddle2Scale);
}

void render()
{
    BeginDrawing();

    ClearBackground(ColorFromHex(BG_COLOUR));

    // Table
    renderObject(&gTableTexture, &ORIGIN, &TABLE_SIZE);

    // Paddles
    renderObject(&gPaddle1Texture, &gPaddle1Position, &gPaddle1Scale);
    renderObject(&gPaddle2Texture, &gPaddle2Position, &gPaddle2Scale);

    // Puck
    renderObject(&gPuckTexture, &gPuckPosition, &gPuckScale);

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gTableTexture);
    UnloadTexture(gPaddle1Texture);
    UnloadTexture(gPaddle2Texture);
    UnloadTexture(gPuckTexture);

    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}