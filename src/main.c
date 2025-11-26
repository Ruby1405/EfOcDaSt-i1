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

	Polygon workagon0;
	PolygonInit(&workagon0);
	Polygon workagon1;
	PolygonInit(&workagon1);
	
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

		// Polygon construction attempt

		uint16 s0 = 0;

		PolygonClear(&polygons[s0]);
		PolygonAddPoint(&polygons[0], (Vector2){0,0});
		PolygonAddPoint(&polygons[0], (Vector2){0,(float)BOARD_HEIGHT});
		PolygonAddPoint(&polygons[0], (Vector2){(float)BOARD_WIDTH,(float)BOARD_HEIGHT});
		PolygonAddPoint(&polygons[0], (Vector2){(float)BOARD_WIDTH,0});

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

		Color colC = {255, 255, 0, 100};
		PolygonDraw(&polygons[0], colC);
		
		
		// Draw seeds
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			DrawCircleV(seeds[i], 2.0, WHITE);
		}
		
		DrawFPS(10, 10);

		EndDrawing();

		// Get mouse input
		// Vector2 mousePos = GetMousePosition();
		// if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		// {
		// 	seeds[0] = mousePos;
		// }
		// if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		// {
		// 	seeds[1] = mousePos;
		// }

		// Move seeds and bounce off walls
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			seeds[i] = Vector2Add(seeds[i], Vector2Scale(seedVels[i], GetFrameTime() * 20));

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

	// // cleanup
	// // unload our texture so it can be cleaned up
	// UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
