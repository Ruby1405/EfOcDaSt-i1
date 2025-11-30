#include <stdlib.h>
// #include <stdio.h>

#include "raylib.h"

#include "definitions.h"

const uint16 POLYGON_CAP_INIT = 8;
typedef struct Polygon
{
	uint16 pointsCap;
	uint16 pointsCount;
	Vector2 *points;
} Polygon;

/// @brief Initializes a polygon structure and allocates initial memory for its points.
/// @param poly Pointer to the polygon to initialize
void PolygonInit(Polygon *poly)
{
    poly->pointsCap = POLYGON_CAP_INIT;
    poly->pointsCount = 0;
    poly->points = (Vector2 *)malloc(sizeof(Vector2) * POLYGON_CAP_INIT);
    return;
}

/// @brief Adds a point to the polygon, resizing the points array if necessary.
/// @param poly Pointer to the polygon
/// @param point The point to add
void PolygonAddPoint(Polygon *poly, Vector2 point)
{
	if (poly->pointsCount >= poly->pointsCap)
	{
		poly->pointsCap *= 2;
		poly->points = (Vector2 *)realloc(poly->points, sizeof(Vector2) * poly->pointsCap);
	}
    // printf("Adding point (%f, %f) at index %d\n", point.x, point.y, poly->pointsCount);
	poly->points[poly->pointsCount++] = point;
	return;
}

/// @brief Adds a point to the polygon at a specified index, resizing the points array if necessary.
/// @param poly Pointer to the polygon
/// @param point The point to add
/// @param index The index at which to add the point
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

/// @brief Removes the last point from the polygon.
/// @param poly Pointer to the polygon
void PolygonRemovePoint(Polygon *poly)
{
    if (poly->pointsCount == 0)
    {
        return;
    }
    poly->pointsCount--;
    return;
}

/// @brief Removes a point from the polygon at a specified index.
/// @param poly Pointer to the polygon
/// @param index The index of the point to remove
void PolygonRemovePointAt(Polygon *poly, uint16 index)
{
    for (uint16 i = index; i < poly->pointsCount - 1; i++)
    {
        poly->points[i] = poly->points[i + 1];
    }
    poly->pointsCount--;
    return;
}

/// @brief Clears all points from the polygon. (O(1), does not free memory)
/// @param poly Pointer to the polygon
void PolygonClear(Polygon *poly)
{
    poly->pointsCount = 0;
    return;
}

/// @brief Frees the memory allocated for the polygon's points and resets its properties.
/// @param poly Pointer to the polygon
void PolygonFree(Polygon *poly)
{
    free(poly->points);
    poly->points = NULL;
    poly->pointsCap = 0;
    poly->pointsCount = 0;
    return;
}

/// @brief Draws the outline of the polygon using lines.
/// @param poly Pointer to the polygon
/// @param col Color to draw the lines
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

/// @brief Draws the filled polygon using a triangle fan.
/// @param poly Pointer to the polygon
/// @param col Color to fill the polygon
void PolygonDraw(Polygon *poly, Color col)
{
    // for (uint16 i = 1; i < poly->pointsCount - 1; i++)
    // {
    //     Vector2 a = poly->points[0];
    //     Vector2 b = poly->points[i];
    //     Vector2 c = poly->points[i + 1];
    //     DrawTriangle(a, b, c, col);
    // }
    DrawTriangleFan(poly->points, poly->pointsCount, col);
    return;
}