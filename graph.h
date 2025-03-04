#ifndef GRAPH_H
#define GRAPH_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

struct VisualizerData;
typedef struct VisualizerData VisualizerData;

#define MAX_NODES 100

typedef enum { NODE_UNVISITED, NODE_VISITED, NODE_PROCESSED } NodeState;

typedef struct {
  int id;
  int x, y;
  NodeState state;
  int distance;
  int prev;
} Node;

typedef struct {
  int from;
  int to;
} Edge;

typedef struct {
  int items[MAX_NODES];
  int size;
} Queue;

typedef struct {
  int items[MAX_NODES];
  int size;
} Stack;

typedef struct {
  int vertices[MAX_NODES];
  int priorities[MAX_NODES];
  int size;
} PriorityQueue;

typedef struct {
  Node nodes[MAX_NODES];
  int adjacencyMatrix[MAX_NODES][MAX_NODES];
  int numNodes;
  Edge currentEdge;
  Queue *queue;
  Stack *stack;
  PriorityQueue *pq;
} Graph;

Graph *graph_create(int capacity);
void graph_destroy(Graph *graph);
void graph_clear(Graph *graph);
void graph_add_node(Graph *graph, int id, int x, int y);
void graph_add_edge(Graph *graph, int from, int to, int weight);
bool graph_has_edge(Graph *graph, int from, int to);
int graph_get_edge_weight(Graph *graph, int from, int to);
void graph_reset_state(Graph *graph);

Queue *queue_create();
void queue_destroy(Queue *queue);
void queue_clear(Queue *queue);
void queue_enqueue(Queue *queue, int value);
int queue_dequeue(Queue *queue);
bool queue_is_empty(Queue *queue);

Stack *stack_create();
void stack_destroy(Stack *stack);
void stack_clear(Stack *stack);
void stack_push(Stack *stack, int value);
int stack_pop(Stack *stack);
bool stack_is_empty(Stack *stack);

PriorityQueue *priority_queue_create();
void priority_queue_destroy(PriorityQueue *pq);
void priority_queue_clear(PriorityQueue *pq);
void priority_queue_insert(PriorityQueue *pq, int vertex, int priority);
int priority_queue_extract_min(PriorityQueue *pq);
void priority_queue_decrease_key(PriorityQueue *pq, int vertex, int priority);
bool priority_queue_is_empty(PriorityQueue *pq);

void render_graph(VisualizerData *data);

#endif
