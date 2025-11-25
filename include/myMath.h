#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "definitions.h"

inline float MaxFloat(float a, float b)
{
    return (a * (a >= b)) + (b * (b > a));
}

inline float MinFloat(float a, float b)
{
    return (a * (a <= b)) + (b * (b < a));
}

// uint16 CollisionLineLine(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2, Vector2 *intersect)
// {
//     float dot = Vector2DotProduct(Vector2Subtract(a2, a1), Vector2Subtract(b2, b1));
//     if (dot == 0) return 0; // parallel lines

//     // Find intersection point
//     float t = ( (b1.x - a1.x) * (b2.y - b1.y) - (b1.y - a1.y) * (b2.x - b1.x) ) /
//               ( (a2.x - a1.x) * (b2.y - b1.y) - (a2.y - a1.y) * (b2.x - b1.x) );
//     *intersect = Vector2Add(a1, Vector2Scale(Vector2Subtract(a2, a1), t));
//     return 1;
// }

// uint16 CollisionRayLine(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineA, Vector2 lineB, Vector2 *intersect)
// {
//     Vector2 lineDirection = Vector2Subtract(lineB, lineA);
//     Vector2 lineNormal = { -lineDirection.y, lineDirection.x };
//     float dotOrigin = Vector2DotProduct(Vector2Subtract(rayOrigin, lineA), lineNormal);
//     float dotDirection = Vector2DotProduct(rayDirection, lineNormal);

//     // Check if ray is parallel to line or pointing away from line
//     if (dotDirection == 0 || (dotOrigin > 0 && dotDirection > 0) || (dotOrigin < 0 && dotDirection < 0))
//     {
//         return 0;
//     }
//     if (dotDirection < 0)
//     {
//         lineNormal = (Vector2){-lineNormal.x, -lineNormal.y };
//     }

//     // Find intersection point
//     float rayLength = dotOrigin / Vector2DotProduct(rayDirection, lineNormal);
//     *intersect = Vector2Add(rayOrigin, Vector2Scale(rayDirection, rayLength));
//     return 1;
// }

// uint16 CollisionRayLineSegment(Vector2 rayOrigin, Vector2 rayDirection, Vector2 lineA, Vector2 lineB, Vector2 *intersect)
// {
//     // Broad phase unsegmented line check
//     Vector2 broadIntersect;
//     if (!CollisionRayLine(rayOrigin, rayDirection, lineA, lineB, &broadIntersect))
//     {
//         return 0;
//     }

//     // Narrow phase check to see if the intersection is within the line segment
//     if (
//         (broadIntersect.x < MinFloat(lineA.x, lineB.x)) ||
//         (broadIntersect.x > MaxFloat(lineA.x, lineB.x)) ||
//         (broadIntersect.y < MinFloat(lineA.y, lineB.y)) ||
//         (broadIntersect.y > MaxFloat(lineA.y, lineB.y))
//     )
//     {
//         return 0;
//     }

//     *intersect = broadIntersect;
//     return 1;
// }