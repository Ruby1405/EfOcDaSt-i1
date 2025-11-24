/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define uint16 unsigned short
#define uint32 unsigned int
#define sint16 short
#define sint32 int

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	const uint16 BOARD_WIDTH = 1980;
	const uint16 BOARD_HEIGHT = 1020;

	InitWindow(BOARD_WIDTH, BOARD_HEIGHT, "Voronoi");
	ToggleFullscreen();

	// // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	// SearchAndSetResourceDir("resources");

	// // Load a texture from the resources directory
	// Texture wabbit = LoadTexture("wabbit_alpha.png");

	// ----------- my stuff ------------
	const sint16 SEED_COUNT = 20;
	Vector2 seeds[SEED_COUNT];
	for (sint16 i = 0; i < SEED_COUNT; i++)
	{
		seeds[i].x = (float)(GetRandomValue(0, 1980));
		seeds[i].y = (float)(GetRandomValue(0, 1020));
	}
	Vector2 seedVels[SEED_COUNT];
	for (sint16 i = 0; i < SEED_COUNT; i++)
	{
		seedVels[i].x = (float)(GetRandomValue(-2, 2));
		seedVels[i].y = (float)(GetRandomValue(-2, 2));
	}
	
	while (!WindowShouldClose())
	{	
		BeginDrawing();
		ClearBackground(BLACK);

		for (sint16 i = 0; i < SEED_COUNT; i++)
		{
			DrawCircleV(seeds[i], 2.0, WHITE);
		}
		
		
		EndDrawing();

		// Move seeds and bounce off walls
		for (sint16 i = 0; i < SEED_COUNT; i++)
		{
			seeds[i] = Vector2Add(seeds[i], seedVels[i]);

			seedVels[i].x = (
				(-seedVels[i].x * (0 > seeds[i].x || BOARD_WIDTH <= seeds[i].x)) +
				(seedVels[i].x  * (0 <= seeds[i].x && BOARD_WIDTH > seeds[i].x))
			);

			seedVels[i].y = (
				(-seedVels[i].y * (0 > seeds[i].y || BOARD_HEIGHT <= seeds[i].y)) +
				(seedVels[i].y  * (0 <= seeds[i].y && BOARD_HEIGHT > seeds[i].y))
			);

			seeds[i].x = (
				(-seeds[i].x * (0 > seeds[i].x)) +
				(seeds[i].x  * (0 <= seeds[i].x))
			);

			seeds[i].y = (
				(-seeds[i].y * (0 > seeds[i].y)) +
				(seeds[i].y  * (0 <= seeds[i].y))
			);

			seeds[i].x = (
				((BOARD_WIDTH - (seeds[i].x - BOARD_WIDTH)) * (BOARD_WIDTH <= seeds[i].x)) +
				(seeds[i].x 				  * (BOARD_WIDTH > seeds[i].x))
			);

			seeds[i].y = (
				((BOARD_HEIGHT - (seeds[i].y - BOARD_HEIGHT)) * (BOARD_HEIGHT <= seeds[i].y)) +
				(seeds[i].y 				  * (BOARD_HEIGHT > seeds[i].y))
			);
		}
	}

	// // cleanup
	// // unload our texture so it can be cleaned up
	// UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
