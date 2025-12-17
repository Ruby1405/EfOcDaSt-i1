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
    // Broken or something
    // float a = 1.0f / (4.0f * (focus.y - directrixY));
    // float c = (focus.y + directrixY) * 0.5f;

    // float w = x - focus.x;

    // return a * w * w + c;


    // Formula for distance to directrix at x
    // v = (f_y - d) / 2
    // height = ((x - f_x)^2 / 4v) + v

    // float v = (focus.y - directrixY) * 0.5f;
    // float height = ((x - focus.x) * (x - focus.x)) / (4.0f * v) + v;
    // return height;

    // Formula for height of parabola from origin at x
    // v =\frac{\left(f_{y}-d_{y}\right)}{2}
    // h_{y} =\frac{\left(h_{x}-f_{x}\right)^{2}}{4v}+f_{y}-v
    // v = (focus.y - directrixY) / 2
    // height = ((x - focus.x)^2 / 4v) + focus.y - v
    float v = (focus.y - directrixY) * 0.5f;
    float height = ((x - focus.x) * (x - focus.x)) / (4.0f * v) + focus.y - v;
    return height;
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

    // for (uint8 i = 0; i < 21; i++)
    // {
    //     float x = focus.x - 200.0f + (400.0f / 20.0f) * i;
    //     DrawCircleV(
    //         (Vector2){
    //             x,
    //             GetArcHeightAtX(focus, x, directrixY)
    //         },
    //         1.0f,
    //         col
    //     );
    // }
    
}

#pragma region GeoBLStructures
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
typedef struct SweepEvent SweepEvent;
typedef struct Arc
{
    uint16 seed;
    Vector2 focus;
    SweepEvent * circleEvent;
} Arc;
#pragma endregion

#pragma region GeoBLFunctions
char GetEdgeArcIntersect(Edge edge, Arc arc, float directrixY, Vector2 * intersect)
{
    // Case 1: vertical edge
    if (edge.direction.x == 0)
    {
        // Vertical arc
        if (directrixY == arc.focus.y)
        {
            if (edge.start.x == arc.focus.x)
            {
                *intersect = arc.focus;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        float arcY = GetArcHeightAtX(arc.focus, edge.start.x, directrixY);
        *intersect = (Vector2){ edge.start.x, arcY};
        return 1;
    }

    // y = mx + b
    float m = edge.direction.y / edge.direction.x;
    float b = edge.start.y - m * edge.start.x;

    // Case 2: vertical arc
    if (arc.focus.x == (directrixY))
    {
        // Is arc in direction of edge
        float intersectXOffset = arc.focus.x - edge.start.x;
        if (intersectXOffset * edge.direction.x < 0)
        {
            return 0;
        }

        intersect->x = arc.focus.x;
        intersect->y = m * intersect->x + b;
        return 1;
    }

    // General case
    // y = a0 + a1*x + a2 * x^2
    float a2 = 1.0f / (2.0f * (arc.focus.y - directrixY));
    float a1 = -m - 2.0f * a2 * arc.focus.x;
    float a0 = a2 * arc.focus.x * arc.focus.x + (arc.focus.y + directrixY) * 0.5f - b;

    float discriminant = a1 * a1 - 4.0f * a2 * a0;
    if (discriminant < 0)
    {
        return 0;
    }

    float rootDisc = sqrtf(discriminant);
    float x1 = (-a1 + rootDisc) / (2.0f * a2);
    float x2 = (-a1 - rootDisc) / (2.0f * a2);

    float x1Offset = x1 - edge.start.x;
    float x2Offset = x2 - edge.start.x;
    float x1Dot = x1Offset * edge.direction.x;
    float x2Dot = x2Offset * edge.direction.x;

    float x;
    if (x1Dot >= 0.0f && x2Dot < 0.0f) x = x1;
    else if (x1Dot < 0.0f && x2Dot >= 0.0f) x = x2;
    else if (x1Dot >= 0.0f && x2Dot >= 0.0f)
    {
        if (x1Dot < x2Dot) x = x1;
        else x = x2;
    }
    else // both dots are negative
    {
        if (x1Dot < x2Dot) x = x2;
        else x = x1;
    }

    float y = GetArcHeightAtX(arc.focus, x, directrixY);
    *intersect = (Vector2){ x, y };
    return 1;
}
#pragma endregion

#pragma region BinTreeBLStructures
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

    union {
        Arc arc;
        Edge edge;
    } data;
} BeachLineItem;
#pragma endregion

#pragma region SweepLineStructures
typedef enum SweepEventType
{
    SEED_EVENT,
    CIRCLE_EVENT
} SweepEventType;

typedef struct SeedEvent
{
    uint16 seedIndex;
    Vector2 position;
} SeedEvent;

typedef struct CircleEvent
{
    Vector2 center;
    BeachLineItem * arc;
    char valid;
} CircleEvent;

typedef struct SweepEvent
{
    SweepEventType type;
    float yValue;
    union {
        SeedEvent seedEvent;
        CircleEvent circleEvent;
    } data;
} SweepEvent;

#pragma endregion

#pragma region BinTreeFunctions
BeachLineItem * CreateArc(Vector2 focus, uint16 seedIndex)
{
    BeachLineItem * newItem = (BeachLineItem *)malloc(sizeof(BeachLineItem));
    newItem->type = ARC;
    newItem->data.arc.seed = seedIndex;
    newItem->data.arc.focus = focus;
    newItem->parent = NULL;
    newItem->left = NULL;
    newItem->right = NULL;
    newItem->data.arc.circleEvent = NULL;
    return newItem;
}
BeachLineItem * CreateEdge(Vector2 start, Vector2 direction)
{
    BeachLineItem * newItem = (BeachLineItem *)malloc(sizeof(BeachLineItem));
    newItem->type = EDGE;
    newItem->data.edge.start = start;
    newItem->data.edge.direction = direction;
    newItem->data.edge.infinite = 1;
    newItem->parent = NULL;
    newItem->left = NULL;
    newItem->right = NULL;
    return newItem;
}
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
BeachLineItem * BLGetFirstLeftParent(BeachLineItem * item)
{
    BeachLineItem * current = item;
    while (NULL != current->parent && current->parent->left == current)
    {
        current = current->parent;
    }
    return current->parent;
}
BeachLineItem * BLGetFirstRightParent(BeachLineItem * item)
{
    BeachLineItem * current = item;
    while (current->parent != NULL && current->parent->right == current)
    {
        current = current->parent;
    }
    return current->parent;
}
BeachLineItem * BLGetFirstLeftLeaf(BeachLineItem * item)
{
    if (NULL == item->left) return NULL;

    BeachLineItem * current = item->left;
    while (NULL != current->right)
    {
        current = current->right;
    }
    return current;
}
BeachLineItem * BLGetFirstRightLeaf(BeachLineItem * item)
{
    if (NULL == item->right) return NULL;

    BeachLineItem * current = item->right;
    while (NULL != current->left)
    {
        current = current->left;
    }
    return current;
}
void BLSetLeftChild(BeachLineItem * parent, BeachLineItem * child)
{
    parent->left = child;
    if (NULL != child)
    {
        child->parent = parent;
    }
}
void BLSetRightChild(BeachLineItem * parent, BeachLineItem * child)
{
    parent->right = child;
    if (NULL != child)
    {
        child->parent = parent;
    }
}
void BLSetParentFromTemplate(BeachLineItem * item, BeachLineItem * template)
{
    if (NULL == template->parent)
    {
        item->parent = NULL;
        return;
    }

    if (template->parent->left == template)
    {
        BLSetLeftChild(template->parent, item);
    }
    else if (template->parent->right == template)
    {
        BLSetRightChild(template->parent, item);
    }
}
BeachLineItem * BLFindArcAbovePoint(BeachLineItem * root, float point, float directrixY)
{
    if (root == NULL) return NULL;
    BeachLineItem * current = root;
    
    while (current->type != ARC)
    {
        BeachLineItem * left = BLGetFirstLeftLeaf(current->right);
        BeachLineItem * right = BLGetFirstRightLeaf(current->left);

        BeachLineItem * fromLeft = BLGetFirstRightParent(left);
        BeachLineItem * fromRight = BLGetFirstLeftParent(right);

        Edge separatingEdge = fromLeft->data.edge;

        Vector2 leftIntersect, rightIntersect;
        char leftHit = GetEdgeArcIntersect(
            separatingEdge,
            left->data.arc,
            directrixY,
            &leftIntersect
        );
        char rightHit = GetEdgeArcIntersect(
            separatingEdge,
            right->data.arc,
            directrixY,
            &rightIntersect
        );

        float intersectX = leftIntersect.x;
        if (!leftHit && rightHit) intersectX = rightIntersect.x;
        if (point < intersectX) current = current->left;
        else current = current->right;
    }

    return current;
}
BeachLineItem * BLInsertArc(BeachLineItem * root, SweepEvent event, float directrixY)
{
    // Case 0: invalid event
    if (event.type != SEED_EVENT)
    {
        return NULL;
    }

    // Case 1: empty tree
    if (root == NULL)
    {
        BeachLineItem * newItem = (BeachLineItem *)malloc(sizeof(BeachLineItem));
        newItem->type = ARC;
        newItem->data.arc.seed = event.data.seedEvent.seedIndex;
        newItem->data.arc.focus = event.data.seedEvent.position;
        newItem->parent = NULL;
        newItem->left = NULL;
        newItem->right = NULL;

        root = newItem;
        return newItem;
    }

    // Default case: non-empty tree
    Vector2 point = event.data.seedEvent.position;
    uint16 seedIndex = event.data.seedEvent.seedIndex;
    BeachLineItem * arcAbove = BLFindArcAbovePoint(root, point.x, directrixY);
    if (arcAbove == NULL)
    {
        puts("Error: Could not find arc above point in BLInsertArc");
        return NULL;
    }

    BeachLineItem * splitArcLeft = CreateArc(arcAbove->data.arc.focus, arcAbove->data.arc.seed);
    BeachLineItem * splitArcRight = CreateArc(arcAbove->data.arc.focus, arcAbove->data.arc.seed);
    BeachLineItem * newArc = CreateArc(point, seedIndex);

    float intersectionY = GetArcHeightAtX(arcAbove->data.arc.focus, point.x, directrixY);

    Vector2 edgeStart = (Vector2){ point.x, intersectionY };
    Vector2 focusOffset = Vector2Subtract(point, arcAbove->data.arc.focus);
    Vector2 edgeDir = Vector2Normalize((Vector2){ focusOffset.y, -focusOffset.x });
    BeachLineItem * edgeLeft = CreateEdge(edgeStart, edgeDir);
    BeachLineItem * edgeRight = CreateEdge(edgeStart, Vector2Negate(edgeDir));

    BLSetParentFromTemplate(edgeLeft, arcAbove);
    BLSetLeftChild(edgeLeft, splitArcLeft);
    BLSetRightChild(edgeLeft, edgeRight);
    BLSetLeftChild(edgeRight, newArc);
    BLSetRightChild(edgeRight, splitArcRight);

    BeachLineItem * newRoot = root;
    if (root == arcAbove)
    {
        newRoot = edgeLeft;
    }
    if (NULL != arcAbove->data.arc.circleEvent)
    {
        // arcAbove->data.arc.circleEvent->data.circleEvent.valid = 0;
    }

    free(arcAbove);

    // Add circle events for splitArcLeft and splitArcRight here

    return newRoot;
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
    if (item->type == EDGE)
    {
        Vector2 edgeStart = item->data.edge.start;
        Vector2 edgeEnd;
        if (item->data.edge.infinite)
        {
            edgeEnd = (Vector2){
                edgeStart.x + item->data.edge.direction.x * 1000.0f,
                edgeStart.y + item->data.edge.direction.y * 1000.0f
            };
        }
        else
        {
            edgeEnd = (Vector2){
                edgeStart.x + item->data.edge.direction.x,
                edgeStart.y + item->data.edge.direction.y
            };
        }
        DrawLineV(edgeStart, edgeEnd, WHITE);
    }

    DrawBeachLine(seedCount, item->left, directrix, boardWidth, boardHeight);
    DrawBeachLine(seedCount, item->right, directrix, boardWidth, boardHeight);
}

#pragma endregion