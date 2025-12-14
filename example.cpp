#include "raylib.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
struct Point
{
    int x, y;
    bool operator==(Point const &point)
    {
        return x == point.x && y == point.y;
    }
};
enum Direction
{
    Up,
    Down,
    Left,
    Right
};

Point getApplePoint(int gameFieldWidth, int gameFieldHeight, std::vector<Point> const &snake)
{
    static std::minstd_rand gen(
        std::random_device{}());
    std::uniform_int_distribution<int> widthDistr(0, gameFieldWidth-1), heightDistr(0, gameFieldHeight-1);
    return Point{
        widthDistr(gen), heightDistr(gen)};
}

int main(int argc, char **argv)
{
    int gameFieldWidth = 20, gameFieldHeight = 20;
    int screenWidth = 800, screenHeight = 600;

    std::vector<Point> snake;
    snake.push_back(Point{gameFieldWidth / 2, gameFieldHeight / 2});
    snake.push_back(Point{gameFieldWidth / 2 - 1, gameFieldHeight / 2});
    snake.push_back(Point{gameFieldWidth / 2 - 2, gameFieldHeight / 2});

    Point apple = getApplePoint(gameFieldWidth, gameFieldHeight, snake);

    Direction snakeDirection = Up, oldSnakeDirection = Up;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - logo raylib");
    SetTargetFPS(60);

    auto counter = std::chrono::steady_clock::now();

    while (!WindowShouldClose())
    {
        bool updateSnake = false;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - counter) > std::chrono::milliseconds(200))
        {
            updateSnake = true;
            counter = std::chrono::steady_clock::now();
        }

        if (IsKeyDown(KEY_A) && oldSnakeDirection != Right)
            snakeDirection = Left;
        else if (IsKeyDown(KEY_S) && oldSnakeDirection != Up)
            snakeDirection = Down;
        else if (IsKeyDown(KEY_D) && oldSnakeDirection != Left)
            snakeDirection = Right;
        else if (IsKeyDown(KEY_W) && oldSnakeDirection != Down)
            snakeDirection = Up;

        if (updateSnake)
        {
            { // snake movement
                Point nextPosition = snake[0];
                std::cout << nextPosition.x << " " << nextPosition.y << std::endl;
                if (snakeDirection == Left)
                    nextPosition.x = nextPosition.x == 0 ? gameFieldWidth - 1 : nextPosition.x - 1;
                else if (snakeDirection == Right)
                    nextPosition.x = nextPosition.x == gameFieldWidth - 1 ? 0 : nextPosition.x + 1;
                else if (snakeDirection == Up)
                    nextPosition.y = nextPosition.y == gameFieldHeight - 1 ? 0 : nextPosition.y + 1;
                else if (snakeDirection == Down)
                    nextPosition.y = nextPosition.y == 0 ? gameFieldHeight - 1 : nextPosition.y - 1;
                for (int i = 0; i < snake.size(); i++)
                {
                    Point currentPosition = snake[i];
                    snake[i] = nextPosition;
                    nextPosition = currentPosition;
                }
            }
            if (snake[0] == apple)
            {
                apple = getApplePoint(gameFieldWidth, gameFieldHeight, snake);
            }

            oldSnakeDirection = snakeDirection;
        }

        ClearBackground(RAYWHITE);
        for (int i = 0; i < snake.size(); i++)
        {
            DrawRectangle(snake[i].x * screenWidth / gameFieldWidth, (gameFieldHeight - snake[i].y - 1) * screenHeight / gameFieldHeight, screenWidth / gameFieldWidth, screenHeight / gameFieldHeight, BLACK);
        }
        DrawRectangle(apple.x * screenWidth / gameFieldWidth, (gameFieldHeight - apple.y - 1) * screenHeight / gameFieldHeight, screenWidth / gameFieldWidth, screenHeight / gameFieldHeight, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}