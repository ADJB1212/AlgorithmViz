#include "graph.h"
#include "visualizer.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

Graph *graph_create(int capacity) {
  Graph *graph = malloc(sizeof(Graph));
  memset(graph, 0, sizeof(Graph));

  graph->numNodes = 0;
  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;

  for (int i = 0; i < MAX_NODES; i++) {
    for (int j = 0; j < MAX_NODES; j++) {
      graph->adjacencyMatrix[i][j] = 0;
    }
  }

  graph->queue = queue_create();
  graph->stack = stack_create();
  graph->pq = priority_queue_create();

  return graph;
}

void graph_destroy(Graph *graph) {
  if (graph) {
    queue_destroy(graph->queue);
    stack_destroy(graph->stack);
    priority_queue_destroy(graph->pq);
    free(graph);
  }
}

void graph_clear(Graph *graph) {
  graph->numNodes = 0;
  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;

  for (int i = 0; i < MAX_NODES; i++) {
    for (int j = 0; j < MAX_NODES; j++) {
      graph->adjacencyMatrix[i][j] = 0;
    }
  }

  queue_clear(graph->queue);
  stack_clear(graph->stack);
  priority_queue_clear(graph->pq);
}

void graph_add_node(Graph *graph, int id, int x, int y) {
  if (graph->numNodes < MAX_NODES) {
    graph->nodes[graph->numNodes].id = id;
    graph->nodes[graph->numNodes].x = x;
    graph->nodes[graph->numNodes].y = y;
    graph->nodes[graph->numNodes].state = NODE_UNVISITED;
    graph->nodes[graph->numNodes].distance = INT_MAX;
    graph->nodes[graph->numNodes].prev = -1;
    graph->numNodes++;
  }
}

void graph_add_edge(Graph *graph, int from, int to, int weight) {
  if (from >= 0 && from < graph->numNodes && to >= 0 && to < graph->numNodes) {
    graph->adjacencyMatrix[from][to] = weight;
  }
}

bool graph_has_edge(Graph *graph, int from, int to) {
  return (from >= 0 && from < graph->numNodes && to >= 0 &&
          to < graph->numNodes && graph->adjacencyMatrix[from][to] > 0);
}

int graph_get_edge_weight(Graph *graph, int from, int to) {
  if (from >= 0 && from < graph->numNodes && to >= 0 && to < graph->numNodes) {
    return graph->adjacencyMatrix[from][to];
  }
  return 0;
}

void graph_reset_state(Graph *graph) {
  for (int i = 0; i < graph->numNodes; i++) {
    graph->nodes[i].state = NODE_UNVISITED;
    graph->nodes[i].distance = INT_MAX;
    graph->nodes[i].prev = -1;
  }

  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;

  queue_clear(graph->queue);
  stack_clear(graph->stack);
  priority_queue_clear(graph->pq);
}

Queue *queue_create() {
  Queue *queue = malloc(sizeof(Queue));
  queue->size = 0;
  return queue;
}

void queue_destroy(Queue *queue) { free(queue); }

void queue_clear(Queue *queue) { queue->size = 0; }

void queue_enqueue(Queue *queue, int value) {
  if (queue->size < MAX_NODES) {
    queue->items[queue->size++] = value;
  }
}

int queue_dequeue(Queue *queue) {
  if (queue->size <= 0) {
    return -1;
  }

  int value = queue->items[0];

  for (int i = 0; i < queue->size - 1; i++) {
    queue->items[i] = queue->items[i + 1];
  }

  queue->size--;
  return value;
}

bool queue_is_empty(Queue *queue) { return queue->size == 0; }

Stack *stack_create() {
  Stack *stack = malloc(sizeof(Stack));
  stack->size = 0;
  return stack;
}

void stack_destroy(Stack *stack) { free(stack); }

void stack_clear(Stack *stack) { stack->size = 0; }

void stack_push(Stack *stack, int value) {
  if (stack->size < MAX_NODES) {
    stack->items[stack->size++] = value;
  }
}

int stack_pop(Stack *stack) {
  if (stack->size <= 0) {
    return -1;
  }

  return stack->items[--stack->size];
}

bool stack_is_empty(Stack *stack) { return stack->size == 0; }

PriorityQueue *priority_queue_create() {
  PriorityQueue *pq = malloc(sizeof(PriorityQueue));
  pq->size = 0;
  return pq;
}

void priority_queue_destroy(PriorityQueue *pq) { free(pq); }

void priority_queue_clear(PriorityQueue *pq) { pq->size = 0; }

void priority_queue_insert(PriorityQueue *pq, int vertex, int priority) {
  if (pq->size >= MAX_NODES) {
    return;
  }

  int i = pq->size;
  pq->vertices[i] = vertex;
  pq->priorities[i] = priority;
  pq->size++;

  while (i > 0) {
    int parent = (i - 1) / 2;
    if (pq->priorities[i] >= pq->priorities[parent]) {
      break;
    }

    int temp_vertex = pq->vertices[i];
    int temp_priority = pq->priorities[i];
    pq->vertices[i] = pq->vertices[parent];
    pq->priorities[i] = pq->priorities[parent];
    pq->vertices[parent] = temp_vertex;
    pq->priorities[parent] = temp_priority;

    i = parent;
  }
}

int priority_queue_extract_min(PriorityQueue *pq) {
  if (pq->size <= 0) {
    return -1;
  }

  int min_vertex = pq->vertices[0];

  pq->vertices[0] = pq->vertices[pq->size - 1];
  pq->priorities[0] = pq->priorities[pq->size - 1];
  pq->size--;

  int i = 0;
  while (true) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->priorities[left] < pq->priorities[smallest]) {
      smallest = left;
    }

    if (right < pq->size && pq->priorities[right] < pq->priorities[smallest]) {
      smallest = right;
    }

    if (smallest == i) {
      break;
    }

    int temp_vertex = pq->vertices[i];
    int temp_priority = pq->priorities[i];
    pq->vertices[i] = pq->vertices[smallest];
    pq->priorities[i] = pq->priorities[smallest];
    pq->vertices[smallest] = temp_vertex;
    pq->priorities[smallest] = temp_priority;

    i = smallest;
  }

  return min_vertex;
}

void priority_queue_decrease_key(PriorityQueue *pq, int vertex, int priority) {

  int i;
  for (i = 0; i < pq->size; i++) {
    if (pq->vertices[i] == vertex) {
      break;
    }
  }

  if (i == pq->size || priority >= pq->priorities[i]) {
    return;
  }

  pq->priorities[i] = priority;

  while (i > 0) {
    int parent = (i - 1) / 2;
    if (pq->priorities[i] >= pq->priorities[parent]) {
      break;
    }

    int temp_vertex = pq->vertices[i];
    int temp_priority = pq->priorities[i];
    pq->vertices[i] = pq->vertices[parent];
    pq->priorities[i] = pq->priorities[parent];
    pq->vertices[parent] = temp_vertex;
    pq->priorities[parent] = temp_priority;

    i = parent;
  }
}

bool priority_queue_is_empty(PriorityQueue *pq) { return pq->size == 0; }

void render_graph(VisualizerData *data) {
  SDL_Renderer *renderer = data->app->renderer;
  Graph *graph = data->graph;

  for (int i = 0; i < graph->numNodes; i++) {
    for (int j = 0; j < graph->numNodes; j++) {
      if (graph_has_edge(graph, i, j)) {
        Node *from = &graph->nodes[i];
        Node *to = &graph->nodes[j];

        if (i == graph->currentEdge.from && j == graph->currentEdge.to) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else if ((from->state == NODE_PROCESSED &&
                    to->state == NODE_PROCESSED) ||
                   (from->state == NODE_PROCESSED &&
                    to->state == NODE_VISITED)) {
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        } else if ((from->state == NODE_VISITED && to->state == NODE_VISITED) ||
                   (from->state == NODE_VISITED &&
                    to->state == NODE_UNVISITED)) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        } else {
          SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        }

        SDL_RenderDrawLine(renderer, from->x, from->y, to->x, to->y);

        if (data->app->selectedAlgorithm == ALGO_DIJKSTRA) {
          char weight_str[10];
          sprintf(weight_str, "%d", graph->adjacencyMatrix[i][j]);

          SDL_Color textColor = {200, 200, 200, 255};
          SDL_Surface *textSurface =
              TTF_RenderText_Solid(data->app->font, weight_str, textColor);
          SDL_Texture *textTexture =
              SDL_CreateTextureFromSurface(renderer, textSurface);

          int midX = (from->x + to->x) / 2;
          int midY = (from->y + to->y) / 2;

          SDL_Rect textRect = {midX - textSurface->w / 2,
                               midY - textSurface->h / 2, textSurface->w,
                               textSurface->h};
          SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

          SDL_FreeSurface(textSurface);
          SDL_DestroyTexture(textTexture);
        }
      }
    }
  }

  for (int i = 0; i < graph->numNodes; i++) {
    Node *node = &graph->nodes[i];

    if (node->state == NODE_PROCESSED) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else if (node->state == NODE_VISITED) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    int radius = 20;
    for (int y = -radius; y <= radius; y++) {
      for (int x = -radius; x <= radius; x++) {
        if (x * x + y * y <= radius * radius) {
          SDL_RenderDrawPoint(renderer, node->x + x, node->y + y);
        }
      }
    }

    char id_str[10];
    sprintf(id_str, "%d", node->id);

    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface *textSurface =
        TTF_RenderText_Solid(data->app->font, id_str, textColor);
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {node->x - textSurface->w / 2,
                         node->y - textSurface->h / 2, textSurface->w,
                         textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    if (data->app->selectedAlgorithm == ALGO_DIJKSTRA &&
        node->distance < INT_MAX) {
      char dist_str[10];
      sprintf(dist_str, "d:%d", node->distance);

      SDL_Color distColor = {200, 200, 255, 255};
      SDL_Surface *distSurface =
          TTF_RenderText_Solid(data->app->font, dist_str, distColor);
      SDL_Texture *distTexture =
          SDL_CreateTextureFromSurface(renderer, distSurface);

      SDL_Rect distRect = {node->x - distSurface->w / 2, node->y + radius + 5,
                           distSurface->w, distSurface->h};
      SDL_RenderCopy(renderer, distTexture, NULL, &distRect);

      SDL_FreeSurface(distSurface);
      SDL_DestroyTexture(distTexture);
    }
  }
}
