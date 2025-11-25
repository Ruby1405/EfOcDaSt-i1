/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include <stdlib.h>     

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define uint16 unsigned short
#define uint32 unsigned int
#define sint16 short
#define sint32 int

typedef struct Disector
{
	uint16 seedA;
	uint16 seedB;
	Vector2 pointA;
	Vector2 pointB;
} Disector;

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
	const uint16 SEED_COUNT = 20;
	Vector2 seeds[SEED_COUNT];
	for (uint16 i = 0; i < SEED_COUNT; i++)
	{
		seeds[i].x = (float)(GetRandomValue(0, 1980));
		seeds[i].y = (float)(GetRandomValue(0, 1020));
	}
	Vector2 seedVels[SEED_COUNT];
	for (uint16 i = 0; i < SEED_COUNT; i++)
	{
		seedVels[i].x = (float)(GetRandomValue(-2, 2));
		seedVels[i].y = (float)(GetRandomValue(-2, 2));
	}

	const uint16 DISECTORS_CAP_INIT = SEED_COUNT * 2;
	uint16 disectorsCap = DISECTORS_CAP_INIT;
	uint16 disectorsCount = 0;
	Disector *disectors = (Disector *)malloc(sizeof(Disector) * DISECTORS_CAP_INIT);

	const uint16 POLYGON_CAP_INIT = 8;
	typedef struct Polygon
	{
		uint16 pointsCap;
		uint16 pointsCount;
		Vector2 *points;
	} Polygon;
	Polygon polygons[SEED_COUNT];
	
	while (!WindowShouldClose())
	{	
		BeginDrawing();
		ClearBackground(BLACK);

		// Pixel by pixel draw voronoi
		// for (uint16 y = 0; y < BOARD_HEIGHT; y++)
		// {
		// 	for (uint16 x = 0; x < BOARD_WIDTH; x++)
		// 	{
		// 		uint16 closestSeed = 0;
		// 		float closestDist = (BOARD_WIDTH + BOARD_HEIGHT) * (BOARD_WIDTH + BOARD_HEIGHT);
		// 		for (uint16 i = 0; i < SEED_COUNT; i++)
		// 		{
		// 			float dist = Vector2DistanceSqr((Vector2){ (float)x, (float)y }, seeds[i]);
		// 			if (dist < closestDist)
		// 			{
		// 				closestDist = dist;
		// 				closestSeed = i;
		// 			}
		// 		}
		// 		float hue = (float)closestSeed / (float)SEED_COUNT;
		// 		Color col = ColorFromHSV(hue * 360.0f, 1.0f, 1.0f);
		// 		col.a = 100;
		// 		DrawPixel(x, y, col);
		// 	}
		// }

		// Draw all disectors
		// for (uint16 i = SEED_COUNT - 1; i > 0; i--)
		// {
		// 	uint16 ii = i - 1;
		// 	do
		// 	{
		// 		Vector2 disectV = {seeds[i].y - seeds[ii].y, - seeds[i].y - seeds[ii].y};
		// 		Vector2 disectP = Vector2Scale(Vector2Add(seeds[i],seeds[ii]),0.5f);
		// 		// DrawLineV(seeds[i], seeds[ii], WHITE);
		// 		DrawLineV(Vector2Subtract(disectP,disectV),Vector2Add(disectP, disectV), WHITE);
		// 	}
		// 	while (ii-- > 0);
		// }

		for (uint16 i = SEED_COUNT - 1; i > 0; i--)
		{
			uint16 ii = i - 1;
			do
			{
				Vector2 disectV = {seeds[i].y - seeds[ii].y, - seeds[i].y - seeds[ii].y};
				Vector2 disectP = Vector2Scale(Vector2Add(seeds[i],seeds[ii]),0.5f);
				// DrawLineV(seeds[i], seeds[ii], WHITE);
				DrawLineV(Vector2Subtract(disectP,disectV),Vector2Add(disectP, disectV), WHITE);
			}
			while (ii-- > 0);
		}
		
		
		// Draw seeds
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			DrawCircleV(seeds[i], 2.0, WHITE);
		}
		
		DrawFPS(10, 10);

		EndDrawing();

		// Move seeds and bounce off walls
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			seeds[i] = Vector2Add(seeds[i], Vector2Scale(seedVels[i], GetFrameTime() * 20));

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
