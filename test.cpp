#include "ValRef.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

using namespace anim;

template<typename T>
struct Test{
    ValRef<T> v;
    std::string msg=std::string("Non");

    void print()
    {
        T o = v;
        std::cout << o << " ";
    }

    void print2()
    {
        T o = v;
        std::cout << msg << ":" << o << " ";
    }
};


void val_ref_test()
{
    std::string s="Test";

    std::vector<Test<std::string>> test;

    for(auto i : std::ranges::views::iota(1,10)){
        test.emplace_back(std::to_string(i));
    }

    test.emplace_back(&s);

    for(int i=0; i<10; i++){
        for(auto t : test){
            t.print();
        }
        std::cout << std::endl;
        s+="a";
    }

    Test<int> t(3);
    Test<int> t2 = {.v=t.v, .msg=std::string("Yeah")};

    for(int i=0; i<10; i++){
        t.print2();
        t2.print2();
        std::cout << std::endl;
        t.v+=1;
        t2.v+=1;
    }
}

#define MAX_POINTS  11

void DrawTexturePoly(Texture2D texture, Vector2 center, Vector2 *points, Vector2 *texcoords, int pointCount, Color tint);
//------------------------------------------------------------------------------------
int main(void)
{
      //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [textures] example - textured polygon");

    // Define texture coordinates to map our texture to poly
    Vector2 texcoords[MAX_POINTS] = {
        (Vector2){ 0.375f, 0.875f},
        (Vector2){ 0.625f, 0.875f},
        (Vector2){ 0.75f, 1.0f},
        (Vector2){ 1.0f, 0.75f},
        (Vector2){ 2.0f, 0.5f},
        (Vector2){ 0.75f, 0.0f}  // Close the poly
    };

    // Define the base poly vertices from the UV's
    // NOTE: They can be specified in any other way
    Vector2 points[MAX_POINTS] = { 0 };
    for (int i = 0; i < MAX_POINTS; i++)
    {
        points[i].x = (texcoords[i].x - 0.5f)*256.0f;
        points[i].y = (texcoords[i].y - 0.5f)*256.0f;
    }

    // Define the vertices drawing position
    // NOTE: Initially same as points but updated every frame
    Vector2 positions[MAX_POINTS] = { 0 };
    for (int i = 0; i < MAX_POINTS; i++) positions[i] = points[i];

    Image red = GenImageGradientLinear(500,500,45,RED,BLUE);
    // Load texture to be mapped to poly
    Texture texture = LoadTextureFromImage(red);
    UnloadImage(red);

    float angle = 0.0f;             // Rotation angle (in degrees)

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Update points rotation with an angle transform
        // NOTE: Base points position are not modified
        angle++;
        for (int i = 0; i < MAX_POINTS; i++) positions[i] = Vector2Rotate(points[i], angle*DEG2RAD);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("textured polygon", 20, 20, 20, DARKGRAY);

            DrawTexturePoly(texture, (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f },
                            positions, texcoords, MAX_POINTS, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Unload texture

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Draw textured polygon, defined by vertex and texture coordinates
// NOTE: Polygon center must have straight line path to all points
// without crossing perimeter, points must be in anticlockwise order
void DrawTexturePoly(Texture2D texture, Vector2 center, Vector2 *points, Vector2 *texcoords, int pointCount, Color tint)
{
    rlSetTexture(texture.id);

    // Texturing is only supported on RL_QUADS
    rlBegin(RL_QUADS);

        rlColor4ub(tint.r, tint.g, tint.b, tint.a);

        for (int i = 0; i < pointCount - 1; i++)
        {
            rlTexCoord2f(0.5f, 0.5f);
            rlVertex2f(center.x, center.y);

            rlTexCoord2f(texcoords[i].x, texcoords[i].y);
            rlVertex2f(points[i].x + center.x, points[i].y + center.y);

            rlTexCoord2f(texcoords[i + 1].x, texcoords[i + 1].y);
            rlVertex2f(points[i + 1].x + center.x, points[i + 1].y + center.y);

            rlTexCoord2f(texcoords[i + 1].x, texcoords[i + 1].y);
            rlVertex2f(points[i + 1].x + center.x, points[i + 1].y + center.y);
        }
    rlEnd();

    rlSetTexture(0);
}
