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
// #include "fortune.h"
#include "myFortune.h"
#include "priorityQueue.h"
#include "myMath.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	// SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Time profiling file
	// FILE * timeFile;
	// timeFile = fopen("output/time_profile.csv", "w");

	// Detect screen size and use that for the window
	// uint16 BOARD_WIDTH = GetScreenWidth();
	// uint16 BOARD_HEIGHT = GetScreenHeight();

	const uint16 BOARD_WIDTH = 1000;
	const uint16 BOARD_HEIGHT = 1000;

	InitWindow(BOARD_WIDTH, BOARD_HEIGHT, "Voronoi");
	
	// ToggleFullscreen();

	// ----------- my stuff ------------
	const uint16 MAX_SEED_COUNT = 6;
	uint16 SEED_COUNT = 6;
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

	#pragma region PolygonSollution
	// ------------- polygon sollution --------------
	// Polygon polygons[MAX_SEED_COUNT];

	// for (uint16 i = 0; i < MAX_SEED_COUNT; i++)
	// {
	// 	PolygonInit(&polygons[i]);
	// }

	// Polygon workagon0;
	// PolygonInit(&workagon0);
	// Polygon workagon1;
	// PolygonInit(&workagon1);
	// ---------------------------------------------
	#pragma endregion

	#pragma region FortuneSollution
	// ------------- Fortune's algorithm sollution --------------
	BeachLineItem * beachLineRoot = NULL;
	uint16 bLSeedIndex = 0;

	// uint16List activeSeedBuffer;
	// uint16ListInit(&activeSeedBuffer, 16);
	// VorVertexList vorVerts;
	// VorVertexListInit(&vorVerts, 16);
	// Polygon voronoiPolygons[MAX_SEED_COUNT];

	// for (uint16 i = 0; i < MAX_SEED_COUNT; i++)
	// {
	// 	PolygonInit(&voronoiPolygons[i]);
	// }

	// EdgeList edgeAlloc;
	// EdgeListInit(&edgeAlloc, 256);
	// NodeList nodeAlloc;
	// NodeListInit(&nodeAlloc, 256);
	// EventList eventAlloc;
	// EventListInit(&eventAlloc, 256);
	// BinaryPriorityQueue PQ;
	// BPQInit(&PQ, 256);
	// PNodeList circleCheckList;
	// PNodeListInit(&circleCheckList, 256);
	// // Hashtable CurrentArcs;
	// CircleList currentCircles;
	// CircleListInit(&currentCircles, 256);
	// VoronoiGraph voronoiGraph;
	// VGInit(&voronoiGraph, 256);
	// ----------------------------------------------------------
	#pragma endregion

	const uint16 SAMPLES_PER_COUNT = 20;
	// uint16 sampleCounter = 0;
	
	while (!WindowShouldClose())
	{	
		// SEED_COUNT += (sampleCounter >= SAMPLES_PER_COUNT);
		// sampleCounter = sampleCounter + 1 - (sampleCounter >= SAMPLES_PER_COUNT) * SAMPLES_PER_COUNT;
		// if (SEED_COUNT > MAX_SEED_COUNT) break;

		// clock_t startTime = clock();

		#pragma region PolygonSollution
		// ------------- polygon sollution --------------
		// for (uint16 s0 = 0; s0 < SEED_COUNT; s0++)
		// {

		// 	PolygonClear(&polygons[s0]);
		// 	PolygonAddPoint(&polygons[s0], (Vector2){0,0});
		// 	PolygonAddPoint(&polygons[s0], (Vector2){0,(float)BOARD_HEIGHT});
		// 	PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH,(float)BOARD_HEIGHT});
		// 	PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH,0});

		// 	// PolygonAddPoint(&polygons[s0], (Vector2){3,3});
		// 	// PolygonAddPoint(&polygons[s0], (Vector2){3,(float)BOARD_HEIGHT - 3});
		// 	// PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH - 3,(float)BOARD_HEIGHT - 3});
		// 	// PolygonAddPoint(&polygons[s0], (Vector2){(float)BOARD_WIDTH - 3, 3});

		// 	for (uint16 s1 = 0; s1 < SEED_COUNT; s1++)
		// 	{
		// 		if (s0 == s1) continue;

				
		// 		Vector2 midpoint = Vector2Scale(Vector2Add(seeds[s0], seeds[s1]), 0.5f);
		// 		Vector2 disectV = {seeds[s0].y - seeds[s1].y, - seeds[s0].x + seeds[s1].x};
				
		// 		PolygonClear(&workagon0);
				
		// 		uint16 stage = 0;
		// 		for (uint16 v = 0; v < polygons[s0].pointsCount; v++)
		// 		{
		// 			switch (stage)
		// 			{
		// 				case 0:
		// 				{
		// 					Vector2 v0 = polygons[s0].points[v];
		// 					Vector2 v1 = polygons[s0].points[(v + 1) % polygons[s0].pointsCount];
		// 					Vector2 intersect;

		// 					PolygonAddPoint(&workagon0, v0);
							
		// 					if (CollisionLineLineSegmentPVPP(midpoint, disectV, v0, v1, &intersect))
		// 					{

		// 						stage++;
		// 						PolygonAddPoint(&workagon0, intersect);
		// 						PolygonClear(&workagon1);
		// 						PolygonAddPoint(&workagon1, intersect);
		// 					}
		// 					break;
		// 				}
		// 			case 1:
		// 				{
		// 					Vector2 v0 = polygons[s0].points[v];
		// 					Vector2 v1 = polygons[s0].points[(v + 1) % polygons[s0].pointsCount];

		// 					PolygonAddPoint(&workagon1, v0);

		// 					Vector2 intersect;
		// 					if (CollisionLineLineSegmentPVPP(midpoint, disectV, v0, v1, &intersect))
		// 					{
		// 						stage++;
		// 						PolygonAddPoint(&workagon0, intersect);
		// 						PolygonAddPoint(&workagon1, intersect);
		// 					}
		// 					break;
		// 				}
		// 			case 2:
		// 				{
		// 					PolygonAddPoint(&workagon0, polygons[s0].points[v]);
		// 					break;
		// 				}
		// 			default:
		// 				break;
		// 			}
		// 		}
		// 		if (CheckCollisionPointPoly(seeds[s0], workagon0.points, workagon0.pointsCount))
		// 		{
		// 			Vector2 * temppoints = polygons[s0].points;
		// 			uint16 temppointsCount = polygons[s0].pointsCount;
		// 			uint16 temppointsCap = polygons[s0].pointsCap;
		// 			polygons[s0].points = workagon0.points;
		// 			polygons[s0].pointsCount = workagon0.pointsCount;
		// 			polygons[s0].pointsCap = workagon0.pointsCap;
		// 			workagon0.points = temppoints;
		// 			workagon0.pointsCount = temppointsCount;
		// 			workagon0.pointsCap = temppointsCap;
		// 		}
		// 		else
		// 		{
		// 			Vector2 * temppoints = polygons[s0].points;
		// 			uint16 temppointsCount = polygons[s0].pointsCount;
		// 			uint16 temppointsCap = polygons[s0].pointsCap;
		// 			polygons[s0].points = workagon1.points;
		// 			polygons[s0].pointsCount = workagon1.pointsCount;
		// 			polygons[s0].pointsCap = workagon1.pointsCap;
		// 			workagon1.points = temppoints;
		// 			workagon1.pointsCount = temppointsCount;
		// 			workagon1.pointsCap = temppointsCap;
		// 		}
		// 	}
		// }
		// ---------------------------------------------
		#pragma endregion

		#pragma region FortuneSollution
		// ------------- Fortune's algorithm sollution --------------
		// Reset data structures

		// Sort seeds by y value ascending
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			for (uint16 ii = i; ii < SEED_COUNT - 1; ii++)
			{
				if (seeds[ii].y > seeds[ii + 1].y)
				{
					Vector2 temp = seeds[ii];
					seeds[ii] = seeds[ii + 1];
					seeds[ii + 1] = temp;

					Vector2 tempV = seedVels[ii];
					seedVels[ii] = seedVels[ii + 1];
					seedVels[ii + 1] = tempV;
				}
			}
		}

		// uint16ListClear(&activeSeedBuffer);
		// VorVertexListClear(&vorVerts);

		// // Sort seeds by x value descending
		// for (uint16 i = 0; i < SEED_COUNT; i++)
		// {
		// 	for (uint16 ii = i; ii < SEED_COUNT - 1; ii++)
		// 	{
		// 		if (seeds[ii].x < seeds[ii + 1].x)
		// 		{
		// 			Vector2 temp = seeds[ii];
		// 			seeds[ii] = seeds[ii + 1];
		// 			seeds[ii + 1] = temp;

		// 			Vector2 tempV = seedVels[ii];
		// 			seedVels[ii] = seedVels[ii + 1];
		// 			seedVels[ii + 1] = tempV;
		// 		}
		// 	}
		// }

		// float sweepLine = 0.0f;
		// Circumcircle * currentCirclesHead = NULL;
		// for (uint16 seedI = 0; seedI < SEED_COUNT; seedI++)
		// {
		// 	Vector2 seed = seeds[seedI];
		// 	sweepLine = seed.y;

		// 	// If the sweep line has left a circle, close the circle (process circle event)
		// 	while (
		// 		currentCirclesHead != NULL &&
		// 		CCEventTime(currentCirclesHead) < sweepLine
		// 	)
		// 	{
		// 		// Add circle to voronoi graph
		// 		VorVertexListAdd(&vorVerts, CCToVorVertex(*currentCirclesHead));

		// 		// Remove circle from active list
		// 		Circumcircle * toDelete = currentCirclesHead;
		// 		currentCirclesHead = currentCirclesHead->next;
		// 		free(toDelete);
		// 	}

		// 	// Process seed event
		// 	// Check if seed lies within any existing circumcircles
		// 	{
		// 		Circumcircle * c = currentCirclesHead;
		// 		while (c != NULL)
		// 		{
		// 			if (CCcontainsPoint(c, seed))
		// 			{
		// 				// Handle case where seed is inside circumcircle
		// 				// Find the closest point of the circumcircle
		// 				float minDist = FLT_MAX;
		// 				char minIndex = 0;
		// 				for (char p = 0; p < 3; p++)
		// 				{
		// 					float dist = Vector2Distance(
		// 						seed,
		// 						seeds[c->pointIndices[p]]
		// 					);
		// 					if (dist < minDist)
		// 					{
		// 						minDist = dist;
		// 						minIndex = p;
		// 					}
		// 				}
		// 				// Split the circle between the the seed and the closest point
		// 				// Circle one
		// 				c->pointIndices[(minIndex + 1) % 3] = seedI;
		// 				c->center = CCCenter(
		// 					seeds[c->pointIndices[0]],
		// 					seeds[c->pointIndices[1]],
		// 					seeds[c->pointIndices[2]]
		// 				);
		// 				c->radius = Vector2Distance(
		// 					c->center,
		// 					seeds[seedI]
		// 				);
		// 				// Remove circle from list
		// 				if (currentCirclesHead == c)
		// 					currentCirclesHead = c->next;
		// 				else
		// 					c->prev->next = c->next;
		// 				if (c->next != NULL)
		// 					c->next->prev = c->prev;
		// 				CCAddToList(&currentCirclesHead, c);
						
		// 				// Circle two
		// 				Vector2 center = CCCenter(
		// 					seeds[c->pointIndices[(minIndex + 2) % 3]],
		// 					seeds[c->pointIndices[minIndex]],
		// 					seeds[seedI]
		// 				);
		// 				Circumcircle * c2 = CCCreate(
		// 					c->pointIndices[(minIndex + 2) % 3],
		// 					c->pointIndices[minIndex],
		// 					seedI,
		// 					center,
		// 					Vector2Distance(
		// 						center,
		// 						seeds[seedI]
		// 					)
		// 				);
		// 				CCAddToList(&currentCirclesHead, c2);
		// 			}
		// 			c = c->next;
		// 		}
		// 	}

		// 	// If there are at least two active seeds, create new circumcircles
		// 	if (activeSeedBuffer.size >= 2)
		// 	{
		// 		// Create all possible circles with the new seed and any two active seeds
		// 		uint16 p0 = seedI;
		// 		for (uint16 i = 0; i < activeSeedBuffer.size - 1; i++)
		// 		{
		// 			uint16 p1 = activeSeedBuffer.values[i];
		// 			if (p1 == p0) continue;
		// 			for (uint16 ii = i + 1; ii < activeSeedBuffer.size; ii++)
		// 			{
		// 				uint16 p2 = activeSeedBuffer.values[ii];
		// 				if (p2 == p0) continue;

		// 				Vector2 center = CCCenter(
		// 					seeds[p0],
		// 					seeds[p1],
		// 					seeds[p2]
		// 				);
		// 				float radius = Vector2Distance(
		// 					center,
		// 					seeds[p0]
		// 				);

		// 				// Check if any other seed is inside the circumcircle
		// 				char validCircle = 1;
		// 				for (uint16 p3 = 0; p3 < SEED_COUNT; p3++)
		// 				{
		// 					if (p3 == p0 || p3 == p1 || p3 == p2) continue;
		// 					float dist = Vector2Distance(
		// 						center,
		// 						seeds[p3]
		// 					);
		// 					if (dist < radius)
		// 					{
		// 						validCircle = 0;
		// 						break;
		// 					}
		// 				}
		// 				if (!validCircle) continue;

		// 				// Check if the circle already exists
		// 				char circleExists = 0;
		// 				Circumcircle * c = currentCirclesHead;
		// 				while (c != NULL)
		// 				{
		// 					if (
		// 						(c->pointIndices[0] == p0 ||
		// 						c->pointIndices[1] == p0 ||
		// 						c->pointIndices[2] == p0) &&
		// 						(c->pointIndices[0] == p1 ||
		// 						c->pointIndices[1] == p1 ||
		// 						c->pointIndices[2] == p1) &&
		// 						(c->pointIndices[0] == p2 ||
		// 						c->pointIndices[1] == p2 ||
		// 						c->pointIndices[2] == p2)
		// 					)
		// 					{
		// 						circleExists = 1;
		// 						break;
		// 					}
		// 					c = c->next;
		// 				}
		// 				if (circleExists) continue;

		// 				// Create the circumcircle and add it to the list
		// 				Circumcircle * newCircle = CCCreate(
		// 					p0,
		// 					p1,
		// 					p2,
		// 					center,
		// 					radius
		// 				);
		// 				CCAddToList(&currentCirclesHead, newCircle);
		// 			}
		// 		}
		// 	}
		// 	uint16ListAdd(&activeSeedBuffer, seedI);
		// }
		// // Close any remaining circumcircles
		// while (
		// 	currentCirclesHead != NULL
		// )
		// {
		// 	// Add circle to voronoi graph
		// 	VorVertexListAdd(&vorVerts, CCToVorVertex(*currentCirclesHead));

		// 	// Remove circle from active list
		// 	Circumcircle * toDelete = currentCirclesHead;
		// 	currentCirclesHead = currentCirclesHead->next;
		// 	free(toDelete);
		// }

		// // Construct voronoi polygons from voronoi vertices
		// for (uint16 i = 0; i < SEED_COUNT; i++)
		// {
		// 	PolygonClear(&voronoiPolygons[i]);

		// 	for (uint16 v = 0; v < vorVerts.size; v++)
		// 	{
		// 		uint16 insertionIndex = 0;
		// 		for (uint16 polyV = 0; polyV < voronoiPolygons[i].pointsCount; polyV++)
		// 		{
		// 			float angleA = atan2f(
		// 				voronoiPolygons[i].points[polyV].y - seeds[i].y,
		// 				voronoiPolygons[i].points[polyV].x - seeds[i].x
		// 			);
		// 			float angleB = atan2f(
		// 				vorVerts.verts[v].position.y - seeds[i].y,
		// 				vorVerts.verts[v].position.x - seeds[i].x
		// 			);
		// 			if (angleA < angleB)
		// 			{
		// 				insertionIndex = polyV + 1;
		// 			}
		// 		}
		// 		PolygonAddPointAt(
		// 			&voronoiPolygons[i],
		// 			vorVerts.verts[v].position,
		// 			insertionIndex
		// 		);
		// 	}
		// 	PolygonAddPoint(
		// 		&voronoiPolygons[i],
		// 		seeds[i]
		// 	);
		// }
		
		
		
		// Node * rootNode = NULL;
		// EdgeListClear(&edgeAlloc);
		// NodeListClear(&nodeAlloc);
		// EventListClear(&eventAlloc);
		// BPQClear(&PQ);
		// PNodeListClear(&circleCheckList);
		// CircleListClear(&currentCircles);
		// VGClear(&voronoiGraph);
		
		// for (uint16 i = 0; i < MAX_SEED_COUNT; i++)
		// {
		// 	Event * e = EventListAdd(&eventAlloc, (Event){
		// 		.type = 'S',
		// 		.point = seeds[i],
		// 		.nodeP = NULL,
		// 		.nodeL = NULL,
		// 		.nodeR = NULL,
		// 		.valid = 1
		// 	});
		// 	BPQPush(&PQ, e);
		// }
		// while (PQ.size > 0)
		// {
		// 	Event * nextEvent = BPQPop(&PQ);
		// 	if (nextEvent->type == 'S')
		// 	{
		// 		// Handle seed event
		// 		rootNode = ProcessSeedEvent(
		// 			&edgeAlloc,
		// 			&nodeAlloc,
		// 			&voronoiGraph,
		// 			nextEvent->point,
		// 			rootNode,
		// 			nextEvent->point.y,
		// 			&circleCheckList
		// 		);
		// 	}
		// 	else if(nextEvent->type == 'C')
		// 	{
		// 		// Handle circle event
		// 		// Current circles
		// 		CircleListRemoveAt(&currentCircles, CircleListContainsNode(&currentCircles, nextEvent->nodeP));
		// 		if(!nextEvent->valid) continue;
		// 		rootNode = ProcessCircleEvent(
		// 			&edgeAlloc,
		// 			&nodeAlloc,
		// 			&voronoiGraph,
		// 			*nextEvent,
		// 			rootNode,
		// 			CircleEventY(nextEvent),
		// 			&circleCheckList
		// 		);
		// 	}
		// 	else
		// 	{
		// 		// Invalid event type
		// 		printf("Invalid event type encountered: %c\n", nextEvent->type);
		// 		continue;
		// 	}
			
		// 	for (uint8 i = 0; i < circleCheckList.size; i++)
		// 	{
		// 		Node * node = circleCheckList.nodes[i];
		// 		sint16 cIndex = CircleListContainsNode(&currentCircles, node);
		// 		if (cIndex >= 0)
		// 		{
		// 			currentCircles.events[cIndex]->valid = 0;
		// 			CircleListRemoveAt(&currentCircles, cIndex);
		// 		}
		// 		Event * nE = CircleCheckSeedNode(
		// 			&eventAlloc,
		// 			node,
		// 			nextEvent->type == 'S' ? nextEvent->point.y : CircleEventY(nextEvent)
		// 		);
		// 		if (nE != NULL)
		// 		{
		// 			BPQPush(&PQ, nE);
		// 			CircleListAdd(&currentCircles, node, nE);
		// 		}
		// 	}

		// 	if (nextEvent->type == 'S')
		// 	{
		// 		Vector2 point = nextEvent->point;
		// 		for (uint16 i = 0; i < currentCircles.size; i++)
		// 		{
		// 			float dist1 = Vector2Distance(
		// 				point,
		// 				currentCircles.events[i]->center
		// 			);
		// 			float dist2 = CircleEventY(currentCircles.events[i]) - currentCircles.events[i]->center.y;
		// 			if (
		// 				dist1 < dist2 &&
		// 				fabsf(dist1 - dist2) > FEM
		// 			) currentCircles.events[i]->valid = 0;
		// 		}
				
		// 	}
		// }
		// CleanUpTree(rootNode);

		// for (uint16 i = 0; i < voronoiGraph.edges.size; i++)
		// {
		// 	Edge * edge = voronoiGraph.edges.edges[i];
		// 	if (edge->done) continue;
		// 	if (Vector2Equals(edge->vertexB, VERTEX_UNKNOWN))
		// 	{
		// 		EdgeAddVertex(edge, VERTEX_INFINITE);

		// 		if (fabsf(edge->leftPoint.y - edge->rightPoint.y) < FEM && 
		// 			edge->leftPoint.x < edge->rightPoint.x)
		// 		{
		// 			Vector2 temp = edge->leftPoint;
		// 			edge->leftPoint = edge->rightPoint;
		// 			edge->rightPoint = temp;
		// 		}
		// 	}
		// }

		// uint16 minuteEdgeCount = 0;
		// uint16 minuteEdgeCap = 16;
		// Edge ** minuteEdges = (Edge **)malloc(sizeof(Edge *) * minuteEdgeCap);
		// uint16 * minuteEdgeIndices = (uint16 *)malloc(sizeof(uint16) * minuteEdgeCap);
		// for (uint16 i = 0; i < voronoiGraph.edges.size; i++)
		// {
		// 	Edge * edge = voronoiGraph.edges.edges[i];
		// 	if (!EdgeIsPartlyInfinite(edge) && Vector2Equals(edge->vertexA, edge->vertexB))
		// 	{
		// 		// Add edge to minuteEdges
		// 		if (minuteEdgeCount >= minuteEdgeCap)
		// 		{
		// 			minuteEdgeCap *= 2;
		// 			minuteEdges = (Edge **)realloc(minuteEdges, sizeof(Edge *) * minuteEdgeCap);

		// 			minuteEdgeIndices = (uint16 *)realloc(minuteEdgeIndices, sizeof(uint16) * minuteEdgeCap);
		// 		}
		// 		minuteEdges[minuteEdgeCount] = edge;
		// 		minuteEdgeIndices[minuteEdgeCount] = i;
		// 		minuteEdgeCount++;

		// 		// Prevent rounding errors
		// 		for (uint16 ii = 0; ii < voronoiGraph.edges.size; ii++)
		// 		{
		// 			Edge * edge2 = voronoiGraph.edges.edges[ii];

		// 			if (Vector2Equals(edge->vertexA, edge2->vertexA))
		// 				edge2->vertexA = edge->vertexA;
		// 			if (Vector2Equals(edge->vertexA, edge2->vertexB))
		// 				edge2->vertexB = edge->vertexA;
		// 		}
		// 	}
		// }
		
		// for (uint16 i = 0; i < minuteEdgeCount; i++)
		// {
		// 	VGRemoveEdgeAt(&voronoiGraph, minuteEdgeIndices[i] - i);
		// }

		// free(minuteEdges);
		// free(minuteEdgeIndices);
		
		// ----------------------------------------------------------
		#pragma endregion

		BeginDrawing();
		ClearBackground(BLACK);

		#pragma region PixelSollution
		// ------------- pixel sollution --------------
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
		// ---------------------------------------------
		#pragma endregion

		#pragma region PolygonSollution
		// ------------- polygon sollution --------------
		// for (uint16 i = 0; i < SEED_COUNT; i++)
		// {
		// 	Color col = ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f);
		// 	col.a = 100;
		// 	PolygonDraw(&polygons[i], col);
		// 	// PolygonDrawLines(&polygons[i], col);
		// }
		// ----------------------------------------------
		#pragma endregion

		#pragma region FortuneSollution
		// ------------- Fortune's algorithm sollution --------------

		// for (uint16 i = 0; i < SEED_COUNT; i++)
		// {
		// 	Color col = ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f);
		// 	col.a = 100;
		// 	PolygonDraw(&voronoiPolygons[i], col);
		// 	PolygonDrawLines(&voronoiPolygons[i], col);
		// }
		
		float directrix = GetMousePosition().y;
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			if (seeds[i].y > directrix) continue;
			DrawParabola(
				seeds[i],
				directrix,
				BOARD_WIDTH,
				BOARD_HEIGHT,
				ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f)
			);
		}
		DrawBeachLine(seeds, SEED_COUNT, beachLineRoot, directrix, BOARD_WIDTH, BOARD_HEIGHT);
		
		DrawLine(0, directrix, BOARD_WIDTH, directrix, WHITE);

		// for (uint16 i = 0; i < vorVerts.size; i++)
		// {
		// 	DrawCircleV(vorVerts.verts[i].position, 2.0, BLUE);
		// 	DrawLineV(
		// 		seeds[vorVerts.verts[i].s0],
		// 		vorVerts.verts[i].position,
		// 		GREEN
		// 	);
		// 	DrawLineV(
		// 		seeds[vorVerts.verts[i].s1],
		// 		vorVerts.verts[i].position,
		// 		GREEN
		// 	);
		// 	DrawLineV(
		// 		seeds[vorVerts.verts[i].s2],
		// 		vorVerts.verts[i].position,
		// 		GREEN
		// 	);
		// }
		

		// for (uint16 i = 0; i < voronoiGraph.edges.size; i++)
		// {
		// 	Edge * vEdge = voronoiGraph.edges.edges[i];
		// 	DrawLineV(vEdge->leftPoint, vEdge->rightPoint, RED);
		// 	DrawLineV(vEdge->vertexA, (Vector2){100,100}, GREEN);
		// 	DrawLineV(vEdge->vertexB, (Vector2){100,100}, GREEN);
		// }
		// for (uint16 i = 0; i < voronoiGraph.vertices.size; i++)
		// {
		// 	Vector2 v = voronoiGraph.vertices.points[i];
		// 	DrawCircleV(v, 2.0, BLUE);
		// }
		
		// ----------------------------------------------------------
		#pragma endregion

		// clock_t endTime = clock();
		
		// Draw seeds
		for (uint16 i = 0; i < SEED_COUNT; i++)
		{
			// Dots
			// DrawCircleV(seeds[i], 2.0, WHITE);

			// Coloured numbers
			DrawCircleV(seeds[i], 7.0, BLACK);
			Color col = ColorFromHSV(((float)i / (float)SEED_COUNT) * 360.0f, 1.0f, 1.0f);
			DrawText(TextFormat("%d", i + 1), (sint32)seeds[i].x - 3, (sint32)seeds[i].y - 5, 10, col);
		}
		
		DrawFPS(10, 10);

		EndDrawing();

		if (IsKeyPressed(KEY_A))
		{
			// Process Seed event
			BeachLineItem * newItem = (BeachLineItem *)malloc(sizeof(BeachLineItem));
			newItem->type = ARC;
			newItem->data.arc.seed = bLSeedIndex;
			newItem->parent = NULL;
			newItem->left = NULL;
			newItem->right = NULL;

			bLSeedIndex++;

			if (beachLineRoot == NULL)
			{
				beachLineRoot = newItem;
			}
			else
			{
				BeachLineItem * leaf = beachLineRoot;
				while (leaf->right != NULL)
				{
					leaf = leaf->right;
				}
				leaf->right = newItem;
				newItem->parent = leaf;
			}
		}
		if (IsKeyPressed(KEY_D))
		{
			// clear
			BLDelete(&beachLineRoot);
			bLSeedIndex = 0;
		}

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

		// fprintf(timeFile, "%u,%llu\n",
		// 	SEED_COUNT,
		// 	(endTime - startTime)
		// );
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();

	// fclose(timeFile);

	return 0;
}
