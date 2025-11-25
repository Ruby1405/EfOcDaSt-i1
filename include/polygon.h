#include <stdlib.h>
#include <stdio.h>

#include "raylib.h"

#include "definitions.h"

const uint16 POLYGON_CAP_INIT = 8;
typedef struct Polygon
{
	uint16 pointsCap;
	uint16 pointsCount;
	Vector2 *points;
} Polygon;

void PolygonInit(Polygon *poly)
{
    poly->pointsCap = POLYGON_CAP_INIT;
    poly->pointsCount = 0;
    poly->points = (Vector2 *)malloc(sizeof(Vector2) * POLYGON_CAP_INIT);
    return;
}

void PolygonAddPoint(Polygon *poly, Vector2 point)
{
	if (poly->pointsCount >= poly->pointsCap)
	{
		poly->pointsCap *= 2;
		poly->points = (Vector2 *)realloc(poly->points, sizeof(Vector2) * poly->pointsCap);
	}
    printf("Adding point (%f, %f) at index %d\n", point.x, point.y, poly->pointsCount);
	poly->points[poly->pointsCount++] = point;
	return;
}

void PolygonAddPointAt(Polygon *poly, Vector2 point, uint16 index)
{
    if (poly->pointsCount >= poly->pointsCap)
    {
        poly->pointsCap *= 2;
        poly->points = (Vector2 *)realloc(poly->points, sizeof(Vector2) * poly->pointsCap);
    }
    for (uint16 i = poly->pointsCount; i > index; i--)
    {
        poly->points[i] = poly->points[i - 1];
    }
    poly->points[index] = point;
    poly->pointsCount++;
    return;
}

void PolygonClear(Polygon *poly)
{
    poly->pointsCount = 0;
    return;
}

void PolygonFree(Polygon *poly)
{
    free(poly->points);
    poly->points = NULL;
    poly->pointsCap = 0;
    poly->pointsCount = 0;
    return;
}

void PolygonDrawLines(Polygon *poly, Color col)
{
    for (uint16 i = 0; i < poly->pointsCount; i++)
    {
        Vector2 a = poly->points[i];
        Vector2 b = poly->points[(i + 1) % poly->pointsCount];
        DrawLineV(a, b, col);
    }
    return;
}