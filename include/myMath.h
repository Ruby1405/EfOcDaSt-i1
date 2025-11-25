#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "definitions.h"

// inline float MaxFloat(float a, float b)
// {
//     return (a * (a >= b)) + (b * (b > a));
// }

// inline float MinFloat(float a, float b)
// {
//     return (a * (a <= b)) + (b * (b < a));
// }

uint16 CollisionLineLinePV(Vector2 aP, Vector2 aV, Vector2 bP, Vector2 bV, Vector2 *intersect)
{
    float dot = Vector2DotProduct(aV, bV);
    if (dot == 1 || dot == -1) return 0; // parallel lines

    // Find intersection point

    // # The intersection point of the example below should be (0,0)

    // # Vertices for the first line
    // p1_start    = np.asarray([-5,   0])
    // p1_end      = np.asarray([-3,   0])

    // # Vertices for the second line
    // p2_start    = np.asarray([0,    4])
    // p2_end      = np.asarray([0,    2])

    // p       = p1_start
    // r       = (p1_end-p1_start)

    // q       = p2_start
    // s       = (p2_end-p2_start)

    // t       = np.cross(q - p,s)/(np.cross(r,s))

    // # This is the intersection point
    // i       = p + t*r

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