#include <raylib.h>
#include <iostream>

using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) // cpu scores
        {
            cpu_score += 5;
            ResetBall();
        }
        if (x - radius <= 0) // playerscores
        {
            player_score += 5;
            ResetBall();
        }
    }
    void ResetBall()
    {
        x = GetScreenHeight() / 2;
        y = GetScreenHeight() / 2;

        int directionChoice[2] = {-1, 1};
        speed_x *= directionChoice[GetRandomValue(0, 1)];
        speed_y *= directionChoice[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y <= 0)
            y = 0;
        if (y + height >= GetScreenHeight())
            y = GetScreenHeight() - height;
    }

public:
    float x, y;
    float width, height;
    int speed;
    void Draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y -= speed;
        }
        else
            y += speed;

        LimitMovement();
    }
};
Ball ball;
Paddle player;
CpuPaddle cpu;
int main()
{

    cout << "Hello World" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    // ball properities
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // player properties
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // cpu properties
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;
    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();

        // Checking for collision
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
            if (player.y - player.height / 2 > ball.y)
            {
                if (ball.speed_y > 0)
                {
                    ball.speed_y *= -1;
                    cout << "TRUE TRUE TRUE" << endl;
                }
            }
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
            if (cpu.y - cpu.height / 2 > ball.y)
            {
                if (ball.speed_y > 0)
                {
                    ball.speed_y *= -1;
                    cout << "TRUE TRUE TRUE" << endl;
                }
            }
        }
        cpu.Update(ball.y);

        // Drawing
        ClearBackground(BLACK);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        DrawText(TextFormat("%d", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%d", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}