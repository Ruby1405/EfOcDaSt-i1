#include <stdio.h>
#include <float.h>

#include "raylib.h"

#include "myMath.h"
#include "definitions.h"

#pragma region EdgeStructures

typedef struct Edge
{
    char done;
    Vector2 rightPoint, leftPoint;
    Vector2 vertexA, vertexB;
} Edge;

void EdgeAddVertex(Edge * e, Vector2 v)
{
    if (Vector2Equals(e->vertexA, VERTEX_UNKNOWN))
    {
        e->vertexA = v;
    }
    else if (Vector2Equals(e->vertexB, VERTEX_UNKNOWN))
    {
        e->vertexB = v;
    }
}

char EdgeIsPartlyInfinite(Edge * e)
{
    return Vector2Equals(e->vertexA, VERTEX_INFINITE) || Vector2Equals(e->vertexB, VERTEX_INFINITE);
}

typedef struct EdgeList
{
    Edge * edges;
    uint16 size;
    uint16 capacity;
} EdgeList;

void EdgeListInit(EdgeList * el, uint16 capacity)
{
    el->size = 0;
    el->capacity = capacity;
    el->edges = (Edge *)malloc(sizeof(Edge) * el->capacity);
}

Edge * EdgeListAdd(EdgeList * el, Edge e)
{
    if (el->size >= el->capacity)
    {
        el->capacity *= 2;
        el->edges = (Edge *)realloc(el->edges, sizeof(Edge) * el->capacity);
    }
    el->edges[el->size] = e;
    el->size++;
    return &el->edges[el->size - 1];
}

void EdgeListClear(EdgeList * el)
{
    el->size = 0;
}

void EdgeListFree(EdgeList * el)
{
    free(el->edges);
    el->edges = NULL;
    el->size = 0;
    el->capacity = 0;
}

typedef struct PEdgeList
{
    Edge ** edges;
    uint16 size;
    uint16 capacity;
} PEdgeList;

void PEdgeListInit(PEdgeList * el, uint16 capacity)
{
    el->size = 0;
    el->capacity = capacity;
    el->edges = (Edge **)malloc(sizeof(Edge *) * el->capacity);
}

void PEdgeListAdd(PEdgeList * el, Edge * e)
{
    if (el->size >= el->capacity)
    {
        el->capacity *= 2;
        el->edges = (Edge **)realloc(el->edges, sizeof(Edge *) * el->capacity);
    }
    el->edges[el->size] = e;
    el->size++;
}

void PEdgeListClear(PEdgeList * el)
{
    el->size = 0;
}

void PEdgeListFree(PEdgeList * el)
{
    free(el->edges);
    el->edges = NULL;
    el->size = 0;
    el->capacity = 0;
}
#pragma endregion
#pragma region NodeStructures

typedef struct Node
{
    char type; // 'S' for seed, 'E' for edge
    struct Node * left;
    struct Node * right;
    struct Node * parent;
    
    // Only for seed nodes
    Vector2 point;
    // only for edge nodes
    Edge * edge;
    char flipped;
} Node;

typedef struct NodeList
{
    Node * nodes;
    uint16 size;
    uint16 capacity;
} NodeList;

void NodeListInit(NodeList * nl, uint16 capacity)
{
    nl->size = 0;
    nl->capacity = capacity;
    nl->nodes = (Node *)malloc(sizeof(Node) * nl->capacity);
}

Node * NodeListAdd(NodeList * nl, Node n)
{
    if (nl->size >= nl->capacity)
    {
        nl->capacity *= 2;
        nl->nodes = (Node *)realloc(nl->nodes, sizeof(Node) * nl->capacity);
    }
    nl->nodes[nl->size] = n;
    nl->size++;
    return &nl->nodes[nl->size - 1];
}

void NodeListClear(NodeList * nl)
{
    nl->size = 0;
}

void NodeListFree(NodeList * nl)
{
    free(nl->nodes);
    nl->nodes = NULL;
    nl->size = 0;
    nl->capacity = 0;
}

typedef struct PNodeList
{
    Node ** nodes;
    uint16 size;
    uint16 capacity;
} PNodeList;

void PNodeListInit(PNodeList * nl, uint16 capacity)
{
    nl->size = 0;
    nl->capacity = capacity;
    nl->nodes = (Node **)malloc(sizeof(Node *) * nl->capacity);
}

void PNodeListAdd(PNodeList * nl, Node * n)
{
    if (nl->size >= nl->capacity)
    {
        nl->capacity *= 2;
        nl->nodes = (Node **)realloc(nl->nodes, sizeof(Node *) * nl->capacity);
    }
    nl->nodes[nl->size] = n;
    nl->size++;
}

void PNodeListClear(PNodeList * nl)
{
    nl->size = 0;
}

void PNodeListFree(PNodeList * nl)
{
    free(nl->nodes);
    nl->nodes = NULL;
    nl->size = 0;
    nl->capacity = 0;
}
#pragma endregion
#pragma region EventStructures
typedef struct Event {
    char type; // 'S' for seed, 'C' for circle
    Vector2 point;

    // For circle events
    Node * nodeP, * nodeL, * nodeR;
    char valid;
    Vector2 center;
} Event;

float CircleEventY(Event * e)
{
    return e->center.y + Vector2Distance(e->point, e->center);
}

float CircleEventX(Event * e)
{
    return e->center.x;
}

typedef struct EventList {
    Event * events;
    uint16 size;
    uint16 capacity;
} EventList;

void EventListInit(EventList * el, uint16 capacity){
    el->size = 0;
    el->capacity = capacity;
    el->events = (Event *)malloc(sizeof(Event) * el->capacity);
}

Event * EventListAdd(EventList * el, Event e){
    if (el->size >= el->capacity){
        el->capacity *= 2;
        el->events = (Event *)realloc(el->events, sizeof(Event) * el->capacity);
    }
    el->events[el->size] = e;
    el->size++;
    return &el->events[el->size - 1];
}

void EventListClear(EventList * el){
    el->size = 0;
}

void EventListFree(EventList * el){
    free(el->events);
    el->events = NULL;
    el->size = 0;
    el->capacity = 0;
}

#pragma endregion
#pragma region BPQueueStructures

typedef struct BinaryPriorityQueue {
    Event **queue;
    uint16 capacity;
    uint16 size;
} BinaryPriorityQueue;

uint16 BPQInit(BinaryPriorityQueue *pq, uint16 capacity){
    pq->capacity = capacity;
    pq->size = 0;
    pq->queue = (Event **)malloc(sizeof(Event *) * capacity);
    return pq->capacity;
}

uint16 BPQAdd(BinaryPriorityQueue *pq, Event * e){
    if (pq->size >= pq->capacity){
        pq->capacity *= 2;
        pq->queue = (Event **)realloc(pq->queue, sizeof(Event *) * pq->capacity);
    }
    pq->queue[pq->size] = e;
    pq->size++;
    return pq->size - 1;
}

uint16 BPQPush(BinaryPriorityQueue *pq, Event * e){
    uint16 p = pq->size;
    uint16 p2;
    BPQAdd(pq, e);
    while (1)
    {
        if (p == 0) break;
        p2 = (p - 1) / 2;
        if (Compare(p, p2) < 0){
            Event * temp = pq->queue[p];
            pq->queue[p] = pq->queue[p2];
            pq->queue[p2] = temp;
            p = p2;
        }
        else break;
    }
    return p;
}

/// @brief Pop the smalles element from the priority queue
/// @param pq 
/// @return 
Event * BPQPop(BinaryPriorityQueue *pq){
    Event * result = pq->queue[0];
    pq->size--;
    pq->queue[0] = pq->queue[pq->size];
    uint16 p = 0;
    uint16 p1, p2, pn;
    while (1)
    {
        pn = p;
        p1 = 2 * p + 1;
        p2 = 2 * p + 2;
        if (p1 < pq->size && Compare(p, p1) > 0) p = p1; // left child is smaller
        if (p2 < pq->size && Compare(p, p2) > 0) p = p2; // right child is even smaller

        if (pn == p) break;

        Event * temp = pq->queue[p];
        pq->queue[p] = pq->queue[pn];
        pq->queue[pn] = temp;
    }
    return result;
}

void BPQClear(BinaryPriorityQueue *pq){
    pq->size = 0;
}

void BPQFree(BinaryPriorityQueue *pq){
    free(pq->queue);
    pq->queue = NULL;
    pq->size = 0;
    pq->capacity = 0;
}
#pragma endregion
#pragma region VoronoiGraphStructures

typedef struct Vector2List
{
    Vector2 * points;
    uint16 size;
    uint16 capacity;
} Vector2List;

void Vector2ListInit(Vector2List * vl, uint16 capacity)
{
    vl->size = 0;
    vl->capacity = capacity;
    vl->points = (Vector2 *)malloc(sizeof(Vector2) * vl->capacity);
}

void Vector2ListAdd(Vector2List * vl, Vector2 v)
{
    if (vl->size >= vl->capacity)
    {
        vl->capacity *= 2;
        vl->points = (Vector2 *)realloc(vl->points, sizeof(Vector2) * vl->capacity);
    }
    vl->points[vl->size] = v;
    vl->size++;
}

void Vector2ListFree(Vector2List * vl)
{
    free(vl->points);
    vl->points = NULL;
    vl->size = 0;
    vl->capacity = 0;
}

typedef struct VoronoiGraph
{
    PEdgeList edges;
    Vector2List vertices;
} VoronoiGraph;

void VGInit(VoronoiGraph * vg, uint16 capacity)
{
    PEdgeListInit(&vg->edges, capacity);
    Vector2ListInit(&vg->vertices, capacity);
}

void VGClear(VoronoiGraph * vg)
{
    PEdgeListClear(&vg->edges);
    Vector2ListFree(&vg->vertices);
}

void VGFree(VoronoiGraph * vg)
{
    PEdgeListFree(&vg->edges);
    Vector2ListFree(&vg->vertices);
}

void VGAddEdge(VoronoiGraph * vg, Edge * e)
{
    PEdgeListAdd(&vg->edges, e);
}

void VGRemoveEdgeAt(VoronoiGraph * vg, uint16 index)
{
    if (index >= vg->edges.size) return;
    for (uint16 i = index; i < vg->edges.size - 1; i++)
    {
        vg->edges.edges[i] = vg->edges.edges[i + 1];
    }
    vg->edges.size--;
}

void VGAddVertex(VoronoiGraph * vg, Vector2 v)
{
    Vector2ListAdd(&vg->vertices, v);
}

#pragma endregion
#pragma region CircleStructures
typedef struct CircleList {
    Node ** nodes;
    Event ** events;
    uint16 size;
    uint16 capacity;
} CircleList;

void CircleListInit(CircleList * cl, uint16 capacity){
    cl->size = 0;
    cl->capacity = capacity;
    cl->events = (Event **)malloc(sizeof(Event *) * cl->capacity);
    cl->nodes = (Node **)malloc(sizeof(Node *) * cl->capacity);
}

void CircleListAdd(CircleList * cl, Node * n, Event * e){
    if (cl->size >= cl->capacity){
        cl->capacity *= 2;
        cl->events = (Event **)realloc(cl->events, sizeof(Event *) * cl->capacity);
        cl->nodes = (Node **)realloc(cl->nodes, sizeof(Node *) * cl->capacity);
    }
    cl->nodes[cl->size] = n;
    cl->events[cl->size] = e;
    cl->size++;
}

void CircleListClear(CircleList * cl){
    cl->size = 0;
}

void CircleListFree(CircleList * cl){
    free(cl->events);
    free(cl->nodes);
    cl->events = NULL;
    cl->nodes = NULL;
    cl->size = 0;
    cl->capacity = 0;
}

sint16 CircleListContainsNode(CircleList * cl, Node * e){
    for (uint16 i = 0; i < cl->size; i++){
        if (cl->nodes[i] == e) return i;
    }
    return -1;
}

void CircleListRemoveAt(CircleList * cl, uint16 index){
    if (index >= cl->size) return;
    for (uint16 i = index; i < cl->size - 1; i++){
        cl->events[i] = cl->events[i + 1];
        cl->nodes[i] = cl->nodes[i + 1];
    }
    cl->size--;
}

#pragma endregion
#pragma region MinorMethods

void ReplaceChildNode(Node * parent, Node * oldChild, Node * newChild)
{
    if (parent->left == oldChild)
    {
        parent->left = newChild;
    }
    else if (parent->right == oldChild)
    {
        parent->right = newChild;
    }
    if (newChild != NULL)
    {
        newChild->parent = parent;
    }
    oldChild->parent = NULL;
}

float EdgeNodeCut(Node * node, float ys, float x)
{
    if (!node->flipped)
        return x - ParabolicCut(
            node->edge->leftPoint.x, node->edge->leftPoint.y,
            node->edge->rightPoint.x, node->edge->rightPoint.y,
            ys
        );
    return - (x - ParabolicCut(
            node->edge->rightPoint.x, node->edge->rightPoint.y,
            node->edge->leftPoint.x, node->edge->leftPoint.y,
            ys
        ));
}

Node * FindSeedNode(Node * root, float ys, float x)
{
    Node * c = root;
    while (true)
    {
        if (c->type == 'S') return c;

        if (EdgeNodeCut(c, ys, x) < 0)
        {
            c = c->left;
        }
        else
        {
            c = c->right;
        }
    }
}

Node * EdgeToRightNode(Node * edgeNode)
{
    Node * c = edgeNode;
    while (c->parent != NULL)
    {
        if (c->parent->left == c)
        {
            c = c->parent;
            break;
        }
        c = c->parent;
    }
    c->type = 'E';
    return c;
}

Node * LeftSeedNode(Node * edgeNode)
{
    Node * c = edgeNode;
    while (1)
    {
        if (c->parent == NULL) return NULL;
        if (c->parent->left == c)
        {
            c = c->parent;
            continue;
        }
        else
        {
            c = c->parent->left;
            break;
        }
    }
    while (c->right != NULL)
    {
        c = c->right;
    }
    return c;
}

Node * RightSeedNode(Node * edgeNode)
{
    Node * c = edgeNode;
    while (1)
    {
        if (c->parent == NULL) return NULL;
        if (c->parent->right == c)
        {
            c = c->parent;
            continue;
        }
        else
        {
            c = c->parent->right;
            break;
        }
    }
    while (c->left != NULL)
    {
        c = c->left;
    }
    return c;
}

Vector2 CircumCircleCenter(Vector2 v0, Vector2 v1, Vector2 v2)
{
    if (Vector2Equals(v0, v1) || Vector2Equals(v1, v2) || Vector2Equals(v2, v0))
    {
        puts("Error: Identical points in CircumCircleCenter");
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

Event * CircleCheckSeedNode(EventList * eventAlloc, Node * seedNode, float ys)
{
    Node * l = LeftSeedNode(seedNode);
    Node * r = RightSeedNode(seedNode);

    if (
        l == NULL ||
        r == NULL ||
        Vector2Equals(l->point, r->point) ||
        Vector2Equals(r->point, seedNode->point) ||
        Vector2Equals(seedNode->point, l->point)
    ) return NULL;

    if (CCW(
        l->point,
        seedNode->point,
        r->point,
        0
    ) <= 0) return NULL;

    Vector2 circumcenter = CircumCircleCenter(
        l->point,
        seedNode->point,
        r->point
    );

    Event * circleEvent = EventListAdd(eventAlloc, (Event){
        .type = 'C',
        .center = circumcenter,
        .nodeP = seedNode,
        .nodeL = l,
        .nodeR = r,
        .valid = 1
    });
    if (CircleEventY(circleEvent) > ys || fabsf(CircleEventY(circleEvent) - ys) < FEM)
        return circleEvent;
    return NULL;
}

void CleanUpTree(Node * root)
{
    if (root->type == 'S') return;

    while (Vector2Equals(root->edge->vertexB, VERTEX_UNKNOWN))
    {
        EdgeAddVertex(
            root->edge,
            VERTEX_INFINITE
        );
    }
    if (root->flipped)
    {
        Vector2 temp = root->edge->leftPoint;
        root->edge->leftPoint = root->edge->rightPoint;
        root->edge->rightPoint = temp;
    }
    root->edge->done = 1;
    CleanUpTree(root->left);
    CleanUpTree(root->right);
}
#pragma endregion


#pragma region ProcessSeedEvent
/// @brief Returns the updated root node after processing the seed event
/// @param seed 
/// @param rootIsNull 
/// @param rootNode 
/// @param  
/// @return 
Node * ProcessSeedEvent(
    EdgeList * edgeAlloc,
    NodeList * nodeAlloc,
    VoronoiGraph * voronoiGraph,
    Vector2 seed,
    Node * rootNode,
    float ys,
    PNodeList * circleCheckList
)
{
    // If tree is empty, insert the first arc
    if (rootNode == NULL)
    {
        rootNode = NodeListAdd(nodeAlloc, (Node){
            .type = 'S',
            .point = seed,
            .left = NULL,
            .right = NULL,
            .parent = NULL,
            .edge = NULL,
            .flipped = 0
        });

        // Circle check list
        PNodeListClear(circleCheckList);
        PNodeListAdd(circleCheckList, rootNode);
        return rootNode;
    }

    // Find stuff
    Node * c = FindSeedNode(
        rootNode,
        ys,
        seed.x
    );

    // Create new arc?
    Edge * newEdge = EdgeListAdd(edgeAlloc, (Edge){
        .leftPoint = c->point,
        .rightPoint = seed,
        .vertexA = VERTEX_UNKNOWN,
        .vertexB = VERTEX_UNKNOWN
    });
    VGAddEdge(voronoiGraph, newEdge);

    Node * subRoot = NodeListAdd(nodeAlloc, (Node){
        .type = 0,
        .point = {0,0},
        .edge = NULL,
        .left = NULL,
        .right = NULL,
        .parent = NULL,
        .flipped = 0
    });
    if (fabsf(newEdge->leftPoint.y - newEdge->rightPoint.y) < FEM)
    {
        if (newEdge->leftPoint.x < newEdge->rightPoint.x)
        {
            subRoot->type = 'E';
            subRoot->flipped = 0;
            subRoot->left = NodeListAdd(nodeAlloc, (Node){
                .type = 'S',
                .point = newEdge->leftPoint,
                .left = NULL,
                .right = NULL,
                .parent = NULL,
                .edge = NULL,
                .flipped = 0
            });
            subRoot->right = NodeListAdd(nodeAlloc, (Node){
                .type = 'S',
                .point = newEdge->rightPoint,
                .left = NULL,
                .right = NULL,
                .parent = NULL,
                .edge = NULL,
                .flipped = 0
            });
        }
        else
        {
            subRoot->type = 'E';
            subRoot->flipped = 1;
            subRoot->parent = NULL;
            subRoot->left = NodeListAdd(nodeAlloc, (Node){
                .type = 'S',
                .point = newEdge->rightPoint,
                .left = NULL,
                .right = NULL,
                .parent = NULL,
                .edge = NULL,
                .flipped = 0
            });
            subRoot->right = NodeListAdd(nodeAlloc, (Node){
                .type = 'S',
                .point = newEdge->leftPoint,
                .left = NULL,
                .right = NULL,
                .parent = NULL,
                .edge = NULL,
                .flipped = 0
            });
        }
        // Circle check list
        PNodeListClear(circleCheckList);
        PNodeListAdd(circleCheckList, subRoot->left);
        PNodeListAdd(circleCheckList, subRoot->right);
    }
    else
    {
        subRoot->edge = newEdge;
        subRoot->flipped = 0;
        subRoot->type = 'E';
        subRoot->parent = NULL;

        subRoot->left = NodeListAdd(nodeAlloc, (Node){
            .type = 'S',
            .point = newEdge->leftPoint,
            .left = NULL,
            .right = NULL,
            .parent = NULL,
            .edge = NULL,
            .flipped = 0
        });
        subRoot->right = NodeListAdd(nodeAlloc, (Node){
            .type = 'E',
            .edge = newEdge,
            .flipped = 1,
            .left = NodeListAdd(nodeAlloc, (Node){
                .type = 'S',
                .point = newEdge->rightPoint,
                .left = NULL,
                .right = NULL,
                .parent = NULL,
                .edge = NULL
            }),
            .right = NodeListAdd(nodeAlloc, (Node){
                .type = 'S',
                .point = newEdge->leftPoint,
                .left = NULL,
                .right = NULL,
                .parent = NULL,
                .edge = NULL
            }),
            .parent = NULL,
            .point = {0,0}
        });
        // Circle check list
        PNodeListClear(circleCheckList);
        PNodeListAdd(circleCheckList, subRoot->left);
        PNodeListAdd(circleCheckList, subRoot->right->left);
        PNodeListAdd(circleCheckList, subRoot->right->right);
    }

    // Rewire tree
    if (c->parent == NULL)
        return subRoot;
    ReplaceChildNode(c->parent, c, subRoot);
    return rootNode;
}
#pragma endregion
#pragma region ProcessCircleEvent

Node * ProcessCircleEvent(
    EdgeList * edgeAlloc,
    NodeList * nodeAlloc,
    VoronoiGraph * voronoiGraph,
    Event circleEvent,
    Node * rootNode,
    float ys,
    PNodeList * circleCheckList
)
{
    Node * sa, * sb, * sc;
    Node * ea, * eb;

    sb = circleEvent.nodeP;
    sa = sb->left;
    sc = sb->right;

    if (
        sa == NULL ||
        sb->parent == NULL ||
        sc == NULL ||
        !Vector2Equals(sa->point, circleEvent.nodeL->point) ||
        !Vector2Equals(sc->point, circleEvent.nodeR->point)
    )
    {
        // Circle check list
        PNodeListClear(circleCheckList);
        return rootNode;
    }

    ea = sb->parent;
    // Circle check list
    PNodeListClear(circleCheckList);
    PNodeListAdd(circleCheckList, sa);
    PNodeListAdd(circleCheckList, sc);

    // Create new vertex
    Vector2 vertex = circleEvent.center;

    VGAddVertex(voronoiGraph, vertex);

    if (ea->left == sb)
    {
        eb = EdgeToRightNode(sa);

        ReplaceChildNode(ea->parent, ea, ea->right);
    }
    else
    {
        eb = EdgeToRightNode(sb);

        ReplaceChildNode(ea->parent, ea, ea->left);
    }

    EdgeAddVertex(ea->edge, vertex);
    EdgeAddVertex(eb->edge, vertex);

    // Replace eb with new edge 
    Edge * newEdge = EdgeListAdd(edgeAlloc, (Edge){
        .leftPoint = sa->point,
        .rightPoint = sc->point
    });
    EdgeAddVertex(newEdge, vertex);
    VGAddEdge(voronoiGraph, newEdge);

    Node * newEdgeNode = NodeListAdd(nodeAlloc, (Node){
        .type = 'E',
        .edge = newEdge,
        .flipped = 0,
        .left = eb->left,
        .right = eb->right,
        .parent = NULL,
        .point = {0,0}
    });
    if (eb->parent == NULL) return newEdgeNode;
    ReplaceChildNode(eb->parent, eb, newEdgeNode);
    return rootNode;
}
#pragma endregion
#pragma region ComputeVoronoiDiagram
