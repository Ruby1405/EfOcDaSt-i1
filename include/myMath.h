#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "definitions.h"

#pragma region MyMathMethods
// Float Equality Margin
const float FEM = 0.0001f;

/// @brief Returns the maximum of two float values
float MaxFloat(float a, float b)
{
    return (a * (a >= b)) + (b * (b > a));
}

/// @brief Returns the minimum of two float values
float MinFloat(float a, float b)
{
    return (a * (a <= b)) + (b * (b < a));
}
#pragma endregion
#pragma region FortuneMethods

uint16 Compare(uint16 a, uint16 b){
    return a - b;
}

float ParabolicCut(float x1, float y1, float x2, float y2, float ys)
{
    if (fabsf(x1 - x2) < FEM && fabsf(y1 - y2) < FEM)
    {
        // Identical points are not allowed
        puts("Error: Identical points in ParabolicCut");
        exit(1);
    }

    if (fabsf(y1 - ys) < FEM && fabsf(y2 - ys) < FEM)
        return (x1 + x2) * 0.5f;

    if (fabsf(y1 - ys) < FEM) return x1;
    if (fabsf(y2 - ys) < FEM) return x2;

    float a1 = 1.0f / (2.0f * (y1 - ys));
    float a2 = 1.0f / (2.0f * (y2 - ys));

    if (fabsf(a1 - a2) < FEM) return (x1 + x2) * 0.5f;

    float xs1 = 
        0.5/(2*a1-2*a2) * (
            4*a1*x1-4*a2*x2+2 * sqrt(
                -8*a1*x1*a2*x2-2*a1*y1+2*a1*y2+4*a1*a2*x2*x2+2*a2*y1+4*a2*a1*x1*x1-2*a2*y2));

    float xs2 =
        0.5/(2*a1-2*a2) * (
            4*a1*x1-4*a2*x2-2 * sqrt(
                -8*a1*x1*a2*x2-2*a1*y1+2*a1*y2+4*a1*a2*x2*x2+2*a2*y1+4*a2*a1*x1*x1-2*a2*y2));
	
    if (xs1 > xs2)
    {
        float temp = xs1;
        xs1 = xs2;
        xs2 = temp;
    }
    if (y1 >= y2)
        return xs2;
    return xs1;
}

sint16 CCW(Vector2 a, Vector2 b, Vector2 c, char plusOneOnCollinear)
{
    sint32 x1, y1, x2, y2;
    x1 = (sint32)(b.x - a.x);
    y1 = (sint32)(b.y - a.y);
    x2 = (sint32)(c.x - a.x);
    y2 = (sint32)(c.y - a.y);
    if (x1 * y2 > y1 * x2) return 1;
    if (x1 * y2 < y1 * x2) return -1;
    if (x1 * x2 < 0 || y1 * y2 < 0) return -1;
    if (x1*x1 + y1*y1 < x2*x2 + y2*y2 && plusOneOnCollinear) return 1;
    return 0;
}
#pragma endregion

#pragma region CollisionMethods
/// @brief Checks for intersection between two infinite lines defined by a point and a direction vector each.
/// @param aP point on line A
/// @param aV direction vector of line A
/// @param bP point on line B
/// @param bV direction vector of line B
/// @param intersect pointer to store the intersection point if it exists
/// @return 1 if the lines intersect, 0 if they are parallel
uint16 CollisionLineLinePV(Vector2 aP, Vector2 aV, Vector2 bP, Vector2 bV, Vector2 *intersect)
{
    float dot = Vector2DotProduct(aV, bV);
    if (dot == 1 || dot == -1) return 0; // parallel lines

    float t = Vector2CrossProduct(Vector2Subtract(bP, aP), bV) / Vector2CrossProduct(aV, bV);
    *intersect = Vector2Add(aP, Vector2Scale(aV, t));

    return 1;
}


/// @brief Checks for intersection between two infinite lines defined by two points each.
/// @param aA first point on line A
/// @param aB second point on line A
/// @param bA first point on line B
/// @param bB second point on line B
/// @param intersect pointer to store the intersection point if it exists
/// @return 1 if the lines intersect, 0 if they are parallel
uint16 CollisionLineLinePP(Vector2 aA, Vector2 aB, Vector2 bA, Vector2 bB, Vector2 *intersect)
{
    Vector2 aV = Vector2Subtract(aB, aA);
    Vector2 bV = Vector2Subtract(bB, bA);
    return CollisionLineLinePV(aA, aV, bA, bV, intersect);
}

/// @brief Checks for intersection between an infinite line and a line segment.
/// @param aP point on the infinite line
/// @param aV direction vector of the infinite line
/// @param bA first end of the line segment
/// @param bB second end of the line segment
/// @param intersect pointer to store the intersection point if it exists
/// @return 1 if the line and segment intersect, 0 otherwise
uint16 CollisionLineLineSegmentPVPP(Vector2 aP, Vector2 aV, Vector2 bA, Vector2 bB, Vector2 *intersect)
{
    // Broad phase unsegmented line check
    Vector2 broadIntersect;
    if (!CollisionLineLinePV(aP, aV, bA, Vector2Subtract(bB, bA), &broadIntersect))
    {
        return 0;
    }

    // Narrow phase check to see if the intersection is within the line segment
    if ((broadIntersect.x + FEM < MinFloat(bA.x, bB.x) ||
        broadIntersect.x - FEM > MaxFloat(bA.x, bB.x)) &&
        bA.x != bB.x)
    {
        return 0;
    }
    if ((broadIntersect.y + FEM < MinFloat(bA.y, bB.y) ||
        broadIntersect.y - FEM > MaxFloat(bA.y, bB.y)) &&
        bA.y != bB.y)
    {   
        return 0;
    }

    *intersect = broadIntersect;
    return 1;
}

/// @brief Checks for intersection between a ray and an infinite line.
/// @param rayOrigin origin point of the ray
/// @param rayDirection direction vector of the ray
/// @param lineA first point on the line
/// @param lineB second point on the line
/// @param intersect pointer to store the intersection point if it exists
/// @return 1 if the ray and line intersect, 0 otherwise
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

/// @brief Checks for intersection between a ray and a line segment.
/// @param rayOrigin origin point of the ray
/// @param rayDirection direction vector of the ray
/// @param lineA first end of the line segment
/// @param lineB second end of the line segment
/// @param intersect pointer to store the intersection point if it exists
/// @return 1 if the ray and line segment intersect, 0 otherwise
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
#pragma endregion