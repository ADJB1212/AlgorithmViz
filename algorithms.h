#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "main.h"

struct VisualizerData;

typedef enum {
  ALGO_STATE_INITIAL,
  ALGO_STATE_RUNNING,
  ALGO_STATE_FINISHED
} AlgorithmState;

void bubble_sort_step(struct VisualizerData *data);
void insertion_sort_step(struct VisualizerData *data);
void selection_sort_step(struct VisualizerData *data);
void merge_sort_step(struct VisualizerData *data);
void quick_sort_step(struct VisualizerData *data);

void bfs_init(struct VisualizerData *data);
void bfs_step(struct VisualizerData *data);
void dfs_init(struct VisualizerData *data);
void dfs_step(struct VisualizerData *data);
void dijkstra_init(struct VisualizerData *data);
void dijkstra_step(struct VisualizerData *data);

void reset_quicksort_state(void);
void reset_mergesort_state(void);

#endif
