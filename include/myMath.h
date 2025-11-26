#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "definitions.h"

float MaxFloat(float a, float b)
{
    return (a * (a >= b)) + (b * (b > a));
}

float MinFloat(float a, float b)
{
    return (a * (a <= b)) + (b * (b < a));
}

uint16 CollisionLineLinePV(Vector2 aP, Vector2 aV, Vector2 bP, Vector2 bV, Vector2 *intersect)
{
    float dot = Vector2DotProduct(aV, bV);
    if (dot == 1 || dot == -1) return 0; // parallel lines

    float t = Vector2CrossProduct(Vector2Subtract(bP, aP), bV) / Vector2CrossProduct(aV, bV);
    *intersect = Vector2Add(aP, Vector2Scale(aV, t));

    return 1;
}

uint16 CollisionLineLinePP(Vector2 aA, Vector2 aB, Vector2 bA, Vector2 bB, Vector2 *intersect)
{
    Vector2 aV = Vector2Subtract(aB, aA);
    Vector2 bV = Vector2Subtract(bB, bA);
    return CollisionLineLinePV(aA, aV, bA, bV, intersect);
}

uint16 CollisionLineLineSegmentPVPP(Vector2 aP, Vector2 aV, Vector2 bA, Vector2 bB, Vector2 *intersect)
{
    // Broad phase unsegmented line check
    Vector2 broadIntersect;
    if (!CollisionLineLinePV(aP, aV, bA, Vector2Subtract(bB, bA), &broadIntersect))
    {
        return 0;
    }

    // Narrow phase check to see if the intersection is within the line segment
    if (
        (broadIntersect.x < (bA.x * (bA.x <= bB.x)) + (bB.x * (bB.x < bA.x))) ||
        (broadIntersect.x > (bA.x * (bA.x >= bB.x)) + (bB.x * (bB.x > bA.x))) ||
        (broadIntersect.y < (bA.y * (bA.y <= bB.y)) + (bB.y * (bB.y < bA.y))) ||
        (broadIntersect.y > (bA.y * (bA.y >= bB.y)) + (bB.y * (bB.y > bA.y)))
    )
    {
        return 0;
    }

    *intersect = broadIntersect;
    return 1;
}

uint16 CollisionRayLine(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineA, Vector2 lineB, Vector2 *intersect)
{
    Vector2 lineDirection = Vector2Subtract(lineB, lineA);
    Vector2 lineNormal = { -lineDirection.y, lineDirection.x };
    float dotOrigin = Vector2DotProduct(Vector2Subtract(rayOrigin, lineA), lineNormal);
    float dotDirection = Vector2DotProduct(rayDirection, lineNormal);

    // Check if ray is parallel to line or pointing away from line
    if (dotDirection == 0 || (dotOrigin > 0 && dotDirection > 0) || (dotOrigin < 0 && dotDirection < 0))
    {
        return 0;
    }
    if (dotDirection < 0)
    {
        lineNormal = (Vector2){-lineNormal.x, -lineNormal.y };
    }

    // Find intersection point
    float rayLength = dotOrigin / Vector2DotProduct(rayDirection, lineNormal);
    *intersect = Vector2Add(rayOrigin, Vector2Scale(rayDirection, rayLength));
    return 1;
}

uint16 CollisionRayLineSegment(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineA, Vector2 lineB, Vector2 *intersect)
{
    // Broad phase unsegmented line check
    Vector2 broadIntersect;
    if (!CollisionRayLine(rayOrigin, rayDirection, lineA, lineB, &broadIntersect))
    {
        return 0;
    }

    // Narrow phase check to see if the intersection is within the line segment
    if (
        (broadIntersect.x < (lineA.x * (lineA.x <= lineB.x)) + (lineB.x * (lineB.x < lineA.x))) ||
        (broadIntersect.x > (lineA.x * (lineA.x >= lineB.x)) + (lineB.x * (lineB.x > lineA.x))) ||
        (broadIntersect.y < (lineA.y * (lineA.y <= lineB.y)) + (lineB.y * (lineB.y < lineA.y))) ||
        (broadIntersect.y > (lineA.y * (lineA.y >= lineB.y)) + (lineB.y * (lineB.y > lineA.y)))
    )
    {
        return 0;
    }

    *intersect = broadIntersect;
    return 1;
}