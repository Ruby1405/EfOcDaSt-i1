
#include <stdlib.h>     
#include <stdio.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "myMath.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Time profiling file
	FILE * timeFile;
	timeFile = fopen("output/pixel.csv", "w");

	// Detect screen size and use that for the window
	// uint16 BOARD_WIDTH = GetScreenWidth();
	// uint16 BOARD_HEIGHT = GetScreenHeight();

	const uint16 BOARD_WIDTH = 1000;
	const uint16 BOARD_HEIGHT = 1000;

	InitWindow(BOARD_WIDTH, BOARD_HEIGHT, "Voronoi");
	
	// ToggleFullscreen();

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

	const uint16 SAMPLES_PER_COUNT = 20;
	uint16 sampleCounter = 0;
	
	while (!WindowShouldClose())
	{	
		SEED_COUNT += (sampleCounter >= SAMPLES_PER_COUNT);
		sampleCounter = sampleCounter + 1 - (sampleCounter >= SAMPLES_PER_COUNT) * SAMPLES_PER_COUNT;
		if (SEED_COUNT > MAX_SEED_COUNT) break;

		clock_t startTime = clock();

		BeginDrawing();
		ClearBackground(BLACK);

		#pragma region PixelSollution
		// ------------- pixel sollution --------------
		// Pixel by pixel draw voronoi
		for (uint16 y = 0; y < BOARD_HEIGHT; y++)
		{
			for (uint16 x = 0; x < BOARD_WIDTH; x++)
			{
				uint16 closestSeed = 0;
				float closestDist = (BOARD_WIDTH + BOARD_HEIGHT) * (BOARD_WIDTH + BOARD_HEIGHT);
				for (uint16 i = 0; i < SEED_COUNT; i++)
				{
					float dist = Vector2DistanceSqr((Vector2){ (float)x, (float)y }, seeds[i]);
					if (dist < closestDist)
					{
						closestDist = dist;
						closestSeed = i;
					}
				}
				float hue = (float)closestSeed / (float)SEED_COUNT;
				Color col = ColorFromHSV(hue * 360.0f, 1.0f, 1.0f);
				col.a = 100;
				DrawPixel(x, y, col);
			}
		}
		// ---------------------------------------------
		#pragma endregion

		clock_t endTime = clock();
		
		// Draw seeds
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			// Dots
			// DrawCircleV(seeds[i], 2.0, WHITE);

			// Coloured
			Color col = ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f);

			// Dots
			DrawCircleV(seeds[i], 2.0, col);

			// Numbers
			// DrawCircleV(seeds[i], 7.0, BLACK);
			// DrawText(TextFormat("%d", i + 1), (sint32)seeds[i].x - 3, (sint32)seeds[i].y - 5, 10, col);
		}
		
		DrawFPS(10, 10);

		EndDrawing();

		#pragma region SeedMovement
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
		#pragma endregion

		fprintf(timeFile, "%u,%llu\n",
			SEED_COUNT,
			(endTime - startTime)
		);
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();

	fclose(timeFile);

	return 0;
}
