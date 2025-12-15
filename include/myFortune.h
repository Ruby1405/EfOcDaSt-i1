#include <float.h>
#include <stdio.h>

typedef struct Circumcircle
{
    sint16 pointIndices[3];
    Vector2 center;
    float radius;
    
    struct Circumcircle *next;
    struct Circumcircle *prev;
} Circumcircle;

Circumcircle * CCCreate(sint16 p0, sint16 p1, sint16 p2, Vector2 pos, float radius)
{
    Circumcircle * c = (Circumcircle *)malloc(sizeof(Circumcircle));
    c->pointIndices[0] = p0;
    c->pointIndices[1] = p1;
    c->pointIndices[2] = p2;
    c->center = pos;
    c->radius = radius;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

char CCcontainsPoint(Circumcircle * c, Vector2 p)
{
    float dist = Vector2Distance(c->center, p);
    return dist < c->radius;
}

float CCEventTime(Circumcircle * c)
{
    return c->center.y + c->radius;
}

void CCAddToList(Circumcircle ** head, Circumcircle * c)
{
    if (*head == NULL)
    {
        *head = c;
        return;
    }
    Circumcircle * current = *head;
    if (CCEventTime(c) < CCEventTime(current))
    {
        c->next = current;
        current->prev = c;
        *head = c;
        return;
    }
    while (current->next != NULL && CCEventTime(c) > CCEventTime(current->next))
    {
        current = current->next;
    }
    c->next = current->next;
    if (c->next != NULL)
        c->next->prev = c;
    current->next = c;
    c->prev = current;
}

/// @brief Finds the center of the circumcircle defined by three points
/// @param v0 
/// @param v1 
/// @param v2 
/// @return 
Vector2 CCCenter(Vector2 v0, Vector2 v1, Vector2 v2)
{
    if (Vector2Equals(v0, v1) || Vector2Equals(v1, v2) || Vector2Equals(v2, v0))
    {
        puts("Error: Identical points in CCCenter");
        exit(1);
    }

    float ax = (v0.x + v2.x) * 0.5f;
    float ay = (v0.y + v2.y) * 0.5f;

    float cx = (v1.x + v2.x) * 0.5f;
    float cy = (v1.y + v2.y) * 0.5f;

    float bx, by, dx, dy;

    if (v0.x == v2.x)
    {
        bx = 0;
        by = 1;
    }
    else
    {
        bx = (v2.y - v0.y) / (v0.x - v2.x);
        by = 1;
    }

    if (v1.x == v2.x)
    {
        dx = -1;
        dy = 0;
    }
    else
    {
        dx = (v1.y - v2.y) / (v1.x - v2.x);
        dy = -1;
    }

    float a = (dy * (cx - ax) - dx * (cy - ay)) / (bx * dy - dx * by);

    return (Vector2){
        .x = ax + a * bx,
        .y = ay + a * by
    };
}

typedef struct uint16List
{
    uint16 * values;
    uint16 size;
    uint16 capacity;
} uint16List;

void uint16ListInit(uint16List * list, uint16 capacity)
{
    list->size = 0;
    list->capacity = capacity;
    list->values = (uint16 *)malloc(sizeof(uint16) * capacity);
}

void uint16ListAdd(uint16List * list, uint16 value)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->values = (uint16 *)realloc(list->values, sizeof(uint16) * list->capacity);
    }
    list->values[list->size] = value;
    list->size++;
    return;
}

void uint16ListRemoveAt(uint16List * list, uint16 index)
{
    if (index >= list->size)
    {
        return;
    }
    for (uint16 i = index; i < list->size - 1; i++)
    {
        list->values[i] = list->values[i + 1];
    }
    list->size--;
    return;
}

void uint16ListClear(uint16List * list)
{
    list->size = 0;
}

void uint16ListFree(uint16List * list)
{
    free(list->values);
    list->values = NULL;
    list->size = 0;
    list->capacity = 0;
}

// 3 seed indices and position
typedef struct VorVertex{
    uint16 s0, s1, s2;
    Vector2 position;
} VorVertex;

typedef struct VorVertexList{
    VorVertex * verts;
    uint16 size;
    uint16 capacity;
} VorVertexList;

void VorVertexListInit(VorVertexList * vvl, uint16 capacity)
{
    vvl->size = 0;
    vvl->capacity = capacity;
    vvl->verts = (VorVertex *)malloc(sizeof(VorVertex) * vvl->capacity);
}

VorVertex CCToVorVertex(Circumcircle c)
{
    return (VorVertex)
    {
        .s0 = c.pointIndices[0],
        .s1 = c.pointIndices[1],
        .s2 = c.pointIndices[2],
        .position = c.center
    };
}

void VorVertexListAdd(VorVertexList * vvl, VorVertex vv)
{
    if (vvl->size >= vvl->capacity)
    {
        vvl->capacity *= 2;
        vvl->verts = (VorVertex *)realloc(vvl->verts, sizeof(VorVertex) * vvl->capacity);
    }
    vvl->verts[vvl->size] = vv;
    vvl->size++;
}

void VorVertexListClear(VorVertexList * vvl)
{
    vvl->size = 0;
}

void VorVertexListFree(VorVertexList * vvl)
{
    free(vvl->verts);
    vvl->verts = NULL;
    vvl->size = 0;
    vvl->capacity = 0;
}

float GetArcHeightAtX(Vector2 focus, float x, float directrixY)
{
    float a = 1.0f / (4.0f * (focus.y - directrixY));
    float c = (focus.y + directrixY) * 0.5f;

    float w = x - focus.x;
    return a * w * w + c;
}

void DrawParabola(Vector2 focus, float directrixY, uint16 boardWidth, uint16 boardHeight, Color col)
{
    float x = 0.0f;
    Vector2 curvePts[boardWidth];

    for (uint16 i = 0; i < boardWidth; i++)
    {
        curvePts[i] = (Vector2){
            x,
            GetArcHeightAtX(focus, x, directrixY)
        };
        
        // curvePts[i].y = boardHeight - curvePts[i].y;
        x += 1.0f;
    }

    for (uint16 i = 1; i < boardWidth; i++)
    {
        DrawLineV(curvePts[i - 1], curvePts[i], col);
    }
}
#pragma region VoronoiStructures
typedef struct Edge
{
    Vector2 start;
    Vector2 direction;

    char infinite;
} Edge;
typedef struct CompleteEdge
{
    Vector2 start;
    Vector2 end;
} CompleteEdge;
typedef struct Arc
{
    uint16 seed;
    Vector2 focus;
} Arc;
#pragma endregion

#pragma region BeachLineStructures
typedef enum BeachLineItemType
{
    ARC,
    EDGE
} BeachLineItemType;
typedef struct BeachLineItem
{
    BeachLineItemType type;

    struct BeachLineItem * parent;
    struct BeachLineItem * left;
    struct BeachLineItem * right;

    union data{
        Arc arc;
        Edge edge;
    } data;
} BeachLineItem;

#pragma endregion
#pragma region BeachLineFunctions
void BLDelete(BeachLineItem ** root)
{
    if (*root == NULL) return;

    BeachLineItem * left = (*root)->left;
    BeachLineItem * right = (*root)->right;

    free(*root);
    *root = NULL;

    BLDelete(&left);
    BLDelete(&right);
}
BeachLineItem * BLFindArcAbovePoint(BeachLineItem * root, Vector2 point)
{
    if (root == NULL) return NULL;
    BeachLineItem * current = root;
    
    while (current->type != ARC)
    {
        // First leaf on left
        // BeachLineItem * left = 
    }
}
void BLInsertArc(BeachLineItem ** root, uint16 seedIndex)
{

}
void DrawBeachLine(uint16 seedCount, BeachLineItem * item, float directrix, uint16 boardWidth, uint16 boardHeight)
{
    if (item == NULL) return;

    if (item->type == ARC)
    {
        DrawParabola(
            item->data.arc.focus,
            directrix,
            boardWidth,
            boardHeight,
            ColorFromHSV(((float)item->data.arc.seed / (float)seedCount) * 360.0f, 1.0f, 0.5f)
        );
    }

    DrawBeachLine(seedCount, item->left, directrix, boardWidth, boardHeight);
    DrawBeachLine(seedCount, item->right, directrix, boardWidth, boardHeight);
}