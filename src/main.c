/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include <stdlib.h>     
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "definitions.h"
#include "polygon.h"
#include "myMath.h"

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

	const uint16 BOARD_WIDTH = 1000;
	const uint16 BOARD_HEIGHT = 1000;

	InitWindow(BOARD_WIDTH, BOARD_HEIGHT, "Voronoi");
	// ToggleFullscreen();

	// // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	// SearchAndSetResourceDir("resources");

	// // Load a texture from the resources directory
	// Texture wabbit = LoadTexture("wabbit_alpha.png");

	// ----------- my stuff ------------
	const uint16 SEED_COUNT = 20;
	Vector2 seeds[SEED_COUNT];
	for (uint16 i = 0; i < SEED_COUNT; i++)
	{
		seeds[i].x = (float)(GetRandomValue(0, BOARD_WIDTH));
		seeds[i].y = (float)(GetRandomValue(0, BOARD_HEIGHT));
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

	Polygon polygons[SEED_COUNT];
	
	for (uint16 i = 0; i < SEED_COUNT; i++)
	{
		PolygonInit(&polygons[i]);
	}

	Polygon workagon;
	PolygonInit(&workagon);
	
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

		// Polygon construction attempt

		PolygonClear(&polygons[0]);
		// PolygonAddPoint(&polygons[0], (Vector2){0,0});
		// PolygonAddPoint(&polygons[0], (Vector2){(float)BOARD_WIDTH,0});
		// PolygonAddPoint(&polygons[0], (Vector2){(float)BOARD_WIDTH,(float)BOARD_HEIGHT});
		// PolygonAddPoint(&polygons[0], (Vector2){0,(float)BOARD_HEIGHT});

		PolygonAddPoint(&polygons[0], (Vector2){3,3});
		PolygonAddPoint(&polygons[0], (Vector2){3,(float)BOARD_HEIGHT - 3});
		PolygonAddPoint(&polygons[0], (Vector2){(float)BOARD_WIDTH - 3,(float)BOARD_HEIGHT - 3});
		PolygonAddPoint(&polygons[0], (Vector2){(float)BOARD_WIDTH - 3, 3});

		Vector2 midpoint = Vector2Scale(Vector2Add(seeds[0], seeds[1]), 0.5f);
		Vector2 disectV = {seeds[0].y - seeds[1].y, - seeds[0].x + seeds[1].x};

		uint16 stage = 0;
		for (uint16 i = 0; i < polygons[0].pointsCount; i++)
		{
			switch (stage)
			{
			case 0:
				{
					Vector2 v0 = polygons[0].points[i];
					Vector2 v1 = polygons[0].points[(i + 1) % polygons[0].pointsCount];
					Vector2 intersect;
					if (CollisionLineLinePV(midpoint, disectV, v0, Vector2Subtract(v1, v0), &intersect))
					{
						stage++;
						PolygonClear(&workagon);
						PolygonAddPoint(&workagon, intersect);
						PolygonAddPointAt(&polygons[0], intersect, (i + 1) % polygons[0].pointsCount);
						i = (i + 1) % polygons[0].pointsCount;
					}
					break;
				}
			case 1:
				{
					Vector2 v0 = polygons[0].points[i];
					Vector2 v1 = polygons[0].points[(i + 1) % polygons[0].pointsCount];

					PolygonAddPoint(&workagon, v0);
					PolygonRemovePointAt(&polygons[0], i);
					i--;

					Vector2 intersect;
					if (CollisionLineLinePV(midpoint, disectV, v0, Vector2Subtract(v1, v0), &intersect))
					{
						stage++;
						PolygonAddPoint(&workagon, intersect);

						// This polygon is a bit fucked
						PolygonAddPointAt(&polygons[0], intersect, (i + 1) % polygons[0].pointsCount);

						// Check if seed is in polygon
						sint16 result = CheckCollisionPointPoly(seeds[0], workagon.points, workagon.pointsCount);
						printf("%d\n",result);
						if (result)
						{
							// Seed 0 is in polygon, keep it
							// Do nothing
						}
						else
						{
							// // Seed 0 is not in polygon, swap to seed 1
							// PolygonClear(&workagon);
							// stage = 0;
							// i = -1; // will become 0 at top of loop
						}
					}
					break;
				}
			default:
				break;
			}
		}

		Color colA = {0, 0, 255, 100};
		Color colB = {255, 0, 0, 100};

		PolygonDraw(&polygons[0], colA);
		// PolygonDrawLines(&polygons[0], BLUE);
		PolygonDraw(&workagon, colB);
		// PolygonDrawLines(&workagon, RED);
		

		// Line-line collision test
		// Vector2 a, b, c, d;
		// a = seeds[0];
		// b = seeds[1];
		// c = seeds[2];
		// d = seeds[3];

		// DrawLineV(a, b, WHITE);
		// DrawLineV(c, d, WHITE);

		// Vector2 intersect;
		
		// if (CollisionLineLinePP(a, b, c, d, &intersect))
		// {
		// 	DrawCircleV(intersect, 10.0, RED);
		// }
		
		
		// Draw seeds
		for (uint16 i = 0; i < 2; i++)
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
