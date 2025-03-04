#include "algorithms.h"
#include "visualizer.h"
#include <limits.h>
#include <stdlib.h>

void bubble_sort_step(VisualizerData *data) {
  int *arr = data->array;
  int n = data->arraySize;
  static int i = 0;
  static int j = 0;

  if (data->algoState == ALGO_STATE_INITIAL) {
    i = 0;
    j = 0;
    data->algoState = ALGO_STATE_RUNNING;
  }

  if (data->algoState == ALGO_STATE_RUNNING) {
    if (i >= n - 1) {
      data->algoState = ALGO_STATE_FINISHED;
      data->comparingIndices[0] = data->comparingIndices[1] = -1;
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
      return;
    }

    if (j >= n - i - 1) {
      j = 0;
      i++;
      return;
    }

    data->comparingIndices[0] = j;
    data->comparingIndices[1] = j + 1;

    if (arr[j] > arr[j + 1]) {

      data->swappingIndices[0] = j;
      data->swappingIndices[1] = j + 1;

      int temp = arr[j];
      arr[j] = arr[j + 1];
      arr[j + 1] = temp;
    } else {
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
    }

    j++;
  }
}

void selection_sort_step(VisualizerData *data) {
  int *arr = data->array;
  int n = data->arraySize;
  static int i = 0;
  static int j = 0;
  static int min_idx = 0;

  if (data->algoState == ALGO_STATE_INITIAL) {
    i = 0;
    j = 1;
    min_idx = 0;
    data->algoState = ALGO_STATE_RUNNING;
  }

  if (data->algoState == ALGO_STATE_RUNNING) {
    if (i >= n - 1) {
      data->algoState = ALGO_STATE_FINISHED;
      data->comparingIndices[0] = data->comparingIndices[1] = -1;
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
      return;
    }

    if (j == i + 1) {
      min_idx = i;
    }

    data->comparingIndices[0] = j;
    data->comparingIndices[1] = min_idx;

    if (j < n) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
      j++;
      return;
    }

    if (min_idx != i) {
      data->swappingIndices[0] = i;
      data->swappingIndices[1] = min_idx;

      int temp = arr[i];
      arr[i] = arr[min_idx];
      arr[min_idx] = temp;
    } else {
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
    }

    i++;
    j = i + 1;
  }
}

void insertion_sort_step(VisualizerData *data) {
  int *arr = data->array;
  int n = data->arraySize;
  static int i = 1;
  static int j = 0;
  static int key = 0;
  static bool new_element = true;

  if (data->algoState == ALGO_STATE_INITIAL) {
    i = 1;
    new_element = true;
    data->algoState = ALGO_STATE_RUNNING;
  }

  if (data->algoState == ALGO_STATE_RUNNING) {
    if (i >= n) {
      data->algoState = ALGO_STATE_FINISHED;
      data->comparingIndices[0] = data->comparingIndices[1] = -1;
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
      return;
    }

    if (new_element) {
      key = arr[i];
      j = i - 1;
      new_element = false;

      data->highlightedIndices[0] = i;
      data->numHighlighted = 1;
    }

    if (j >= 0) {
      data->comparingIndices[0] = j;
      data->comparingIndices[1] = j + 1;

      if (arr[j] > key) {
        arr[j + 1] = arr[j];
        data->swappingIndices[0] = j;
        data->swappingIndices[1] = j + 1;
        j--;
      } else {
        arr[j + 1] = key;
        data->swappingIndices[0] = data->swappingIndices[1] = -1;
        i++;
        new_element = true;
      }
    } else {
      arr[j + 1] = key;
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
      i++;
      new_element = true;
    }
  }
}

typedef struct MergeSortStack {
  int l, m, r;
  int stage;
  struct MergeSortStack *next;
} MergeSortStack;

static MergeSortStack *mergeSortStack = NULL;

void merge_sort_step(VisualizerData *data) {
  int *arr = data->array;
  int n = data->arraySize;

  if (data->algoState == ALGO_STATE_INITIAL) {

    mergeSortStack = malloc(sizeof(MergeSortStack));
    mergeSortStack->l = 0;
    mergeSortStack->r = n - 1;
    mergeSortStack->stage = 0;
    mergeSortStack->next = NULL;
    data->algoState = ALGO_STATE_RUNNING;
  }

  if (data->algoState == ALGO_STATE_RUNNING) {

    if (!mergeSortStack) {
      data->algoState = ALGO_STATE_FINISHED;
      data->comparingIndices[0] = data->comparingIndices[1] = -1;
      data->swappingIndices[0] = data->swappingIndices[1] = -1;
      return;
    }

    int l = mergeSortStack->l;
    int r = mergeSortStack->r;
    int stage = mergeSortStack->stage;

    data->highlightedIndices[0] = l;
    data->highlightedIndices[1] = r;
    data->numHighlighted = 2;

    if (l >= r) {

      MergeSortStack *temp = mergeSortStack;
      mergeSortStack = mergeSortStack->next;
      free(temp);
      return;
    }

    int m = l + (r - l) / 2;

    if (stage == 0) {

      MergeSortStack *newStack = malloc(sizeof(MergeSortStack));
      newStack->l = l;
      newStack->r = m;
      newStack->stage = 0;
      newStack->next = mergeSortStack;
      mergeSortStack->stage = 1;
      mergeSortStack = newStack;
    } else if (stage == 1) {

      MergeSortStack *newStack = malloc(sizeof(MergeSortStack));
      newStack->l = m + 1;
      newStack->r = r;
      newStack->stage = 0;
      newStack->next = mergeSortStack;
      mergeSortStack->stage = 2;
      mergeSortStack = newStack;
    } else {

      int n1 = m - l + 1;
      int n2 = r - m;
      int *L = malloc(n1 * sizeof(int));
      int *R = malloc(n2 * sizeof(int));

      for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
      for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

      int i = 0, j = 0, k = l;
      while (i < n1 && j < n2) {
        data->comparingIndices[0] = l + i;
        data->comparingIndices[1] = m + 1 + j;

        if (L[i] <= R[j]) {
          arr[k] = L[i];
          data->swappingIndices[0] = k;
          i++;
        } else {
          arr[k] = R[j];
          data->swappingIndices[0] = k;
          j++;
        }
        k++;
      }

      while (i < n1) {
        arr[k] = L[i];
        k++;
        i++;
      }

      while (j < n2) {
        arr[k] = R[j];
        k++;
        j++;
      }

      free(L);
      free(R);

      MergeSortStack *temp = mergeSortStack;
      mergeSortStack = mergeSortStack->next;
      free(temp);
    }
  }
}

typedef struct QuickSortStack {
  int low, high;
  int stage;

  int i, j;
  struct QuickSortStack *next;
} QuickSortStack;

static QuickSortStack *quickSortStack = NULL;

void quick_sort_step(VisualizerData *data) {

  if (data->algoState == ALGO_STATE_FINISHED) {
    return;
  }

  if (data->algoState == ALGO_STATE_RUNNING && !quickSortStack) {
    data->algoState = ALGO_STATE_FINISHED;
    data->comparingIndices[0] = data->comparingIndices[1] = -1;
    data->swappingIndices[0] = data->swappingIndices[1] = -1;
    return;
  }

  if (data->algoState == ALGO_STATE_INITIAL) {
    reset_quicksort_state();

    if (data->arraySize <= 1) {
      data->algoState = ALGO_STATE_FINISHED;
      return;
    }

    QuickSortStack *newStack = malloc(sizeof(QuickSortStack));
    newStack->low = 0;
    newStack->high = data->arraySize - 1;
    newStack->stage = 0;
    newStack->next = NULL;
    quickSortStack = newStack;

    data->algoState = ALGO_STATE_RUNNING;
    return;
  }

  if (!quickSortStack) {
    data->algoState = ALGO_STATE_FINISHED;
    return;
  }

  int *arr = data->array;
  QuickSortStack *current = quickSortStack;

  if (current->low >= current->high) {
    quickSortStack = current->next;
    free(current);
    return;
  }

  if (current->stage == 0) {
    current->i = current->low - 1;
    current->j = current->low;
    current->stage = 1;

    data->highlightedIndices[0] = current->low;
    data->highlightedIndices[1] = current->high;
    data->numHighlighted = 2;
    return;
  }

  if (current->stage == 1) {
    if (current->j < current->high) {

      data->comparingIndices[0] = current->j;
      data->comparingIndices[1] = current->high;

      if (arr[current->j] <= arr[current->high]) {
        current->i++;

        if (current->i != current->j) {
          data->swappingIndices[0] = current->i;
          data->swappingIndices[1] = current->j;

          int temp = arr[current->i];
          arr[current->i] = arr[current->j];
          arr[current->j] = temp;
        }
      }

      current->j++;
      return;
    }

    int pivot_pos = current->i + 1;

    data->swappingIndices[0] = pivot_pos;
    data->swappingIndices[1] = current->high;

    int temp = arr[pivot_pos];
    arr[pivot_pos] = arr[current->high];
    arr[current->high] = temp;

    current->stage = 2;
    return;
  }

  if (current->stage == 2) {
    int pivot_pos = current->i + 1;

    bool created_left = false;

    if (pivot_pos + 1 < current->high) {
      QuickSortStack *right = malloc(sizeof(QuickSortStack));
      right->low = pivot_pos + 1;
      right->high = current->high;
      right->stage = 0;
      right->next = quickSortStack->next;
      quickSortStack->next = right;
    }

    if (current->low < pivot_pos - 1) {
      current->high = pivot_pos - 1;
      current->stage = 0;
      created_left = true;
    }

    if (!created_left) {
      quickSortStack = current->next;
      free(current);
    }

    data->comparingIndices[0] = data->comparingIndices[1] = -1;
    data->swappingIndices[0] = data->swappingIndices[1] = -1;
  }
}

void bfs_init(VisualizerData *data) {
  graph_reset_state(data->graph);

  data->graph->nodes[0].state = NODE_VISITED;

  queue_clear(data->graph->queue);
  queue_enqueue(data->graph->queue, 0);
}

void bfs_step(VisualizerData *data) {
  Graph *graph = data->graph;

  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;

  if (queue_is_empty(graph->queue)) {
    data->algoState = ALGO_STATE_FINISHED;
    return;
  }

  int current = queue_dequeue(graph->queue);

  if (graph->nodes[current].state == NODE_PROCESSED) {
    return;
  }

  graph->nodes[current].state = NODE_PROCESSED;

  for (int i = 0; i < graph->numNodes; i++) {
    if (graph_has_edge(graph, current, i) &&
        graph->nodes[i].state == NODE_UNVISITED) {

      graph->nodes[i].state = NODE_VISITED;
      queue_enqueue(graph->queue, i);

      graph->currentEdge.from = current;
      graph->currentEdge.to = i;

      return;
    }
  }
}

void dfs_init(VisualizerData *data) {
  printf("Initializing DFS\n");

  graph_reset_state(data->graph);

  data->graph->nodes[0].state = NODE_VISITED;

  stack_clear(data->graph->stack);
  stack_push(data->graph->stack, 0);

  data->algoState = ALGO_STATE_RUNNING;
}

void dfs_step(VisualizerData *data) {
  Graph *graph = data->graph;

  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;

  if (stack_is_empty(graph->stack)) {
    data->algoState = ALGO_STATE_FINISHED;
    printf("DFS completed\n");
    return;
  }

  static int current = -1;
  static int next_neighbor_idx = 0;

  if (current == -1) {
    current = stack_pop(graph->stack);
    printf("DFS processing node: %d\n", current);

    if (graph->nodes[current].state == NODE_PROCESSED) {
      current = -1;
      return;
    }

    graph->nodes[current].state = NODE_PROCESSED;
    next_neighbor_idx = graph->numNodes - 1;
  }

  bool found_neighbor = false;
  while (next_neighbor_idx >= 0 && !found_neighbor) {
    if (graph_has_edge(graph, current, next_neighbor_idx) &&
        graph->nodes[next_neighbor_idx].state == NODE_UNVISITED) {

      int neighbor = next_neighbor_idx;
      printf("DFS found neighbor: %d -> %d\n", current, neighbor);

      graph->nodes[neighbor].state = NODE_VISITED;

      stack_push(graph->stack, neighbor);

      graph->currentEdge.from = current;
      graph->currentEdge.to = neighbor;

      found_neighbor = true;
    }
    next_neighbor_idx--;
  }

  if (next_neighbor_idx < 0 || found_neighbor) {
    current = -1;
  }
}

void dijkstra_init(VisualizerData *data) {
  printf("Initializing Dijkstra's algorithm\n");
  Graph *graph = data->graph;
  graph_reset_state(data->graph);

  for (int i = 0; i < graph->numNodes; i++) {
    graph->nodes[i].distance = INT_MAX;
    graph->nodes[i].prev = -1;
  }

  graph->nodes[0].distance = 0;
  graph->nodes[0].state = NODE_VISITED;

  priority_queue_clear(graph->pq);
  priority_queue_insert(graph->pq, 0, 0);

  data->algoState = ALGO_STATE_RUNNING;

  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;
}

void dijkstra_step(VisualizerData *data) {
  Graph *graph = data->graph;

  graph->currentEdge.from = -1;
  graph->currentEdge.to = -1;

  if (priority_queue_is_empty(graph->pq)) {
    data->algoState = ALGO_STATE_FINISHED;
    printf("Dijkstra completed\n");
    return;
  }

  static int current_node = -1;
  static int next_neighbor_idx = 0;

  if (current_node == -1) {
    current_node = priority_queue_extract_min(graph->pq);
    printf("Dijkstra processing node: %d (distance: %d)\n", current_node,
           graph->nodes[current_node].distance);

    if (graph->nodes[current_node].state == NODE_PROCESSED) {
      current_node = -1;
      return;
    }

    graph->nodes[current_node].state = NODE_PROCESSED;
    next_neighbor_idx = 0;
  }

  bool found_neighbor = false;
  while (next_neighbor_idx < graph->numNodes && !found_neighbor) {
    if (graph_has_edge(graph, current_node, next_neighbor_idx)) {
      int neighbor = next_neighbor_idx;
      int weight = graph_get_edge_weight(graph, current_node, neighbor);
      int potential_dist = graph->nodes[current_node].distance + weight;

      if (potential_dist < graph->nodes[neighbor].distance) {
        printf("Dijkstra relaxing edge: %d -> %d (new distance: %d)\n",
               current_node, neighbor, potential_dist);

        graph->nodes[neighbor].distance = potential_dist;
        graph->nodes[neighbor].prev = current_node;

        if (graph->nodes[neighbor].state == NODE_UNVISITED) {
          graph->nodes[neighbor].state = NODE_VISITED;
          priority_queue_insert(graph->pq, neighbor, potential_dist);
        } else {
          priority_queue_decrease_key(graph->pq, neighbor, potential_dist);
        }

        graph->currentEdge.from = current_node;
        graph->currentEdge.to = neighbor;

        found_neighbor = true;
      }
    }
    next_neighbor_idx++;
  }

  if (next_neighbor_idx >= graph->numNodes) {
    current_node = -1;
  }
}

void reset_quicksort_state(void) {

  while (quickSortStack != NULL) {
    QuickSortStack *temp = quickSortStack;
    quickSortStack = quickSortStack->next;
    free(temp);
  }
  quickSortStack = NULL;
}

void reset_mergesort_state(void) {

  while (mergeSortStack != NULL) {
    MergeSortStack *temp = mergeSortStack;
    mergeSortStack = mergeSortStack->next;
    free(temp);
  }
  mergeSortStack = NULL;
}
