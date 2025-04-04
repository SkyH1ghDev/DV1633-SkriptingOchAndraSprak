#include <raylib/raylib.h>
#include <limits>
#include <cstdint>

int main()
{
    InitWindow(400, 400, "Test");
    SetTargetFPS(std::numeric_limits<std::int32_t>::max());
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Hello World!", 100, 100, 20, LIGHTGRAY);
        }
        EndDrawing();
    }

    CloseWindow();
}