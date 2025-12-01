/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include <stdlib.h>     
#include <stdio.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "definitions.h"
#include "polygon.h"
#include "myMath.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Time profiling file
	FILE * timeFile;
	timeFile = fopen("output/time_profile.csv", "w");

	// Detect screen size and use that for the window
	// uint16 BOARD_WIDTH = GetScreenWidth();
	// uint16 BOARD_HEIGHT = GetScreenHeight();

	const uint16 BOARD_WIDTH = 1000;
	const uint16 BOARD_HEIGHT = 1000;

	InitWindow(BOARD_WIDTH, BOARD_HEIGHT, "Voronoi");
	
	// ToggleFullscreen();

	// // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	// SearchAndSetResourceDir("resources");

	// // Load a texture from the resources directory
	// Texture wabbit = LoadTexture("wabbit_alpha.png");

	// ----------- my stuff ------------
	const uint16 MAX_SEED_COUNT = 1000;
	uint16 SEED_COUNT = 1;
	const float SEED_SPEED = 20.0f;
	Vector2 seeds[MAX_SEED_COUNT];
	for (uint16 i = 0; i < MAX_SEED_COUNT; i++)
	{
		seeds[i].x = (float)(GetRandomValue(0, BOARD_WIDTH));
		seeds[i].y = (float)(GetRandomValue(0, BOARD_HEIGHT));
	}
	Vector2 seedVels[MAX_SEED_COUNT];
	for (uint16 i = 0; i < MAX_SEED_COUNT; i++)
	{
		seedVels[i].x = (float)(GetRandomValue(-2, 2));
		seedVels[i].y = (float)(GetRandomValue(-2, 2));
	}

	char manualSeedControl = 0;

	Polygon polygons[MAX_SEED_COUNT];

	for (uint16 i = 0; i < MAX_SEED_COUNT; i++)
	{
		PolygonInit(&polygons[i]);
	}

	Polygon workagon0;
	PolygonInit(&workagon0);
	Polygon workagon1;
	PolygonInit(&workagon1);

	const uint16 SAMPLES_PER_COUNT = 20;
	uint16 sampleCounter = 0;
	
	while (!WindowShouldClose())
	{	
		SEED_COUNT += (sampleCounter >= SAMPLES_PER_COUNT);
		sampleCounter = sampleCounter + 1 - (sampleCounter >= SAMPLES_PER_COUNT) * SAMPLES_PER_COUNT;
		if (SEED_COUNT > MAX_SEED_COUNT) break;

		// struct timespec startTime;
		// timespec_get(&startTime, 1);
		clock_t startTime = clock();

		// Polygon construction attempt

		// uint16 s0 = 0;
		for (uint16 s0 = 0; s0 < SEED_COUNT; s0++)
		{

			PolygonClear(&polygons[s0]);
			PolygonAddPoint(&polygons[s0], (Vector2){0,0});
			PolygonAddPoint(&polygons[s0], (Vector2){0,(float)BOARD_HEIGHT});
			PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH,(float)BOARD_HEIGHT});
			PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH,0});

			// PolygonAddPoint(&polygons[s0], (Vector2){3,3});
			// PolygonAddPoint(&polygons[s0], (Vector2){3,(float)BOARD_HEIGHT - 3});
			// PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH - 3,(float)BOARD_HEIGHT - 3});
			// PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH - 3, 3});

			for (uint16 s1 = 0; s1 < SEED_COUNT; s1++)
			{
				if (s0 == s1) continue;

				
				Vector2 midpoint = Vector2Scale(Vector2Add(seeds[s0], seeds[s1]), 0.5f);
				Vector2 disectV = {seeds[s0].y - seeds[s1].y, - seeds[s0].x + seeds[s1].x};
				
				// Vector2 intersects[2] = {(Vector2){0,0}, (Vector2){0,0}};
				// uint16 didIntersects[2] = {0,0};
				
				PolygonClear(&workagon0);
				
				uint16 stage = 0;
				for (uint16 v = 0; v < polygons[s0].pointsCount; v++)
				{
					switch (stage)
					{
						case 0:
						{
							Vector2 v0 = polygons[s0].points[v];
							Vector2 v1 = polygons[s0].points[(v + 1) % polygons[s0].pointsCount];
							Vector2 intersect;

							PolygonAddPoint(&workagon0, v0);
							
							if (CollisionLineLineSegmentPVPP(midpoint, disectV, v0, v1, &intersect))
							{
								// didIntersects[0] = 1;

								stage++;
								PolygonAddPoint(&workagon0, intersect);
								PolygonClear(&workagon1);
								PolygonAddPoint(&workagon1, intersect);
								
								// intersects[0] = intersect;
							}
							break;
						}
					case 1:
						{
							Vector2 v0 = polygons[s0].points[v];
							Vector2 v1 = polygons[s0].points[(v + 1) % polygons[s0].pointsCount];

							PolygonAddPoint(&workagon1, v0);

							Vector2 intersect;
							if (CollisionLineLineSegmentPVPP(midpoint, disectV, v0, v1, &intersect))
							{
								// didIntersects[1] = 1;

								stage++;
								PolygonAddPoint(&workagon0, intersect);
								PolygonAddPoint(&workagon1, intersect);
								
								// intersects[1] = intersect;
							}
							break;
						}
					case 2:
						{
							PolygonAddPoint(&workagon0, polygons[s0].points[v]);
							break;
						}
					default:
						break;
					}
				}
				if (CheckCollisionPointPoly(seeds[s0], workagon0.points, workagon0.pointsCount))
				{
					Vector2 * temppoints = polygons[s0].points;
					uint16 temppointsCount = polygons[s0].pointsCount;
					uint16 temppointsCap = polygons[s0].pointsCap;
					polygons[s0].points = workagon0.points;
					polygons[s0].pointsCount = workagon0.pointsCount;
					polygons[s0].pointsCap = workagon0.pointsCap;
					workagon0.points = temppoints;
					workagon0.pointsCount = temppointsCount;
					workagon0.pointsCap = temppointsCap;
				}
				else
				{
					Vector2 * temppoints = polygons[s0].points;
					uint16 temppointsCount = polygons[s0].pointsCount;
					uint16 temppointsCap = polygons[s0].pointsCap;
					polygons[s0].points = workagon1.points;
					polygons[s0].pointsCount = workagon1.pointsCount;
					polygons[s0].pointsCap = workagon1.pointsCap;
					workagon1.points = temppoints;
					workagon1.pointsCount = temppointsCount;
					workagon1.pointsCap = temppointsCap;
				}
			}
		}

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
	

		// Color colC = {255, 255, 0, 100};
		// PolygonDraw(&polygons[1], colC);
		
		// Draw Polygons
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			Color col = ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f);
			col.a = 100;
			PolygonDraw(&polygons[i], col);
			// PolygonDrawLines(&polygons[i], col);
		}

		// struct timespec endTime;
		// timespec_get(&endTime, 1);
		clock_t endTime = clock();
		
		// Draw seeds
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			// Dots
			DrawCircleV(seeds[i], 2.0, WHITE);

			// Coloured numbers
			// DrawCircleV(seeds[i], 7.0, BLACK);
			// Color col = ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f);
			// DrawText(TextFormat("%d", i + 1), (sint32)seeds[i].x - 3, (sint32)seeds[i].y - 5, 10, col);
		}
		
		DrawFPS(10, 10);

		EndDrawing();

		if (IsKeyPressed(KEY_SPACE))
		{
			manualSeedControl = !manualSeedControl;
		}
		if (manualSeedControl)
		{
			// Manual seed control
			// Control over up to 22 seeds via keyboard
			Vector2 mousePos = GetMousePosition();
			sint32 key = GetKeyPressed();
			sint32 selectedSeed = -1;
			// numeric keys 1-9
			if (key >= KEY_ONE && key <= KEY_NINE)
			{
				selectedSeed = key - KEY_ONE;
			}
			// numeric key 0 for seed 10
			if (key == KEY_ZERO)
			{
				selectedSeed = 9;
			}
			// function keys F1-F12
			if (key >= KEY_F1 && key <= KEY_F12)
			{
				selectedSeed = key - KEY_F1 + 10;
			}
			// if valid seed move it to mouse position
			if (selectedSeed >= 0 && selectedSeed < SEED_COUNT)
			{
				seeds[selectedSeed] = mousePos;
			}
		}
		else
		{
			// Automatic seed movement
			for (uint16 i = 0; i < SEED_COUNT; i++)
			{
				seeds[i] = Vector2Add(seeds[i], Vector2Scale(seedVels[i], GetFrameTime() * SEED_SPEED));
	
				seedVels[i].x = (
					(-seedVels[i].x * (0 > seeds[i].x || BOARD_WIDTH <= seeds[i].x)) +
					(seedVels[i].x * (0 <= seeds[i].x && BOARD_WIDTH > seeds[i].x))
				);
	
				seedVels[i].y = (
					(-seedVels[i].y * (0 > seeds[i].y || BOARD_HEIGHT <= seeds[i].y)) +
					(seedVels[i].y * (0 <= seeds[i].y && BOARD_HEIGHT > seeds[i].y))
				);
	
				seeds[i].x = (
					(-seeds[i].x * (0 > seeds[i].x)) +
					(seeds[i].x * (0 <= seeds[i].x))
				);
	
				seeds[i].y = (
					(-seeds[i].y * (0 > seeds[i].y)) +
					(seeds[i].y * (0 <= seeds[i].y))
				);
	
				seeds[i].x = (
					((BOARD_WIDTH - (seeds[i].x - BOARD_WIDTH)) * (BOARD_WIDTH <= seeds[i].x)) +
					(seeds[i].x * (BOARD_WIDTH > seeds[i].x))
				);
	
				seeds[i].y = (
					((BOARD_HEIGHT - (seeds[i].y - BOARD_HEIGHT)) * (BOARD_HEIGHT <= seeds[i].y)) +
					(seeds[i].y * (BOARD_HEIGHT > seeds[i].y))
				);
			}
		}

		// Time profiling
		// fprintf(timeFile, "%u,%llu,%llu\n",
		// 	SEED_COUNT,
		// 	((endComputeTime.tv_sec - startTime.tv_sec) * 1000000000ULL) + (endComputeTime.tv_nsec - startTime.tv_nsec),
		// 	((endTime.tv_sec - startTime.tv_sec) * 1000000000ULL) + (endTime.tv_nsec - startTime.tv_nsec)
		// );
		fprintf(timeFile, "%u,%llu\n",
			SEED_COUNT,
			(endTime - startTime)
		);
	}

	// // cleanup
	// // unload our texture so it can be cleaned up
	// UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();

	fclose(timeFile);

	return 0;
}
