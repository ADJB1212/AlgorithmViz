#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "algorithms.h"
#include "graph.h"
#include "main.h"

typedef struct VisualizerData {
  AppContext *app;
  int *array;
  int arraySize;
  int currentStep;
  int comparingIndices[2];
  int swappingIndices[2];
  int highlightedIndices[10];
  int numHighlighted;
  AlgorithmState algoState;
  Graph *graph;
} VisualizerData;

VisualizerData *visualizer_init(AppContext *app);
void visualizer_destroy(VisualizerData *data);
void visualizer_reset(VisualizerData *data);
void visualizer_update(VisualizerData *data);
void visualizer_render(VisualizerData *data);
void visualizer_start_algorithm(VisualizerData *data);
void visualizer_generate_random_array(VisualizerData *data);
void visualizer_generate_random_graph(VisualizerData *data);

void render_array(VisualizerData *data);
void draw_algorithm_name(VisualizerData *data);
void draw_controls_info(VisualizerData *data);

#endif
