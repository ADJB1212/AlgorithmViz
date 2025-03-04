#include "visualizer.h"
#include <stdlib.h>
#include <string.h>

VisualizerData *visualizer_init(AppContext *app) {
  VisualizerData *data = malloc(sizeof(VisualizerData));
  data->app = app;
  data->arraySize = app->arraySize;
  data->array = malloc(sizeof(int) * MAX_ARRAY_SIZE);
  data->currentStep = 0;
  data->comparingIndices[0] = data->comparingIndices[1] = -1;
  data->swappingIndices[0] = data->swappingIndices[1] = -1;
  data->numHighlighted = 0;
  data->graph = graph_create(50);

  memset(data->highlightedIndices, -1, sizeof(data->highlightedIndices));
  visualizer_generate_random_array(data);
  visualizer_generate_random_graph(data);

  return data;
}

void visualizer_destroy(VisualizerData *data) {
  if (data) {
    free(data->array);
    graph_destroy(data->graph);
    free(data);
  }
}

void visualizer_reset(VisualizerData *data) {
  data->currentStep = 0;
  data->comparingIndices[0] = data->comparingIndices[1] = -1;
  data->swappingIndices[0] = data->swappingIndices[1] = -1;
  data->numHighlighted = 0;
  memset(data->highlightedIndices, -1, sizeof(data->highlightedIndices));
  data->arraySize = data->app->arraySize;
  data->algoState = ALGO_STATE_INITIAL;

  if (data->app->isGraphAlgorithm) {
    visualizer_generate_random_graph(data);
  } else {
    visualizer_generate_random_array(data);
  }
}

void visualizer_update(VisualizerData *data) {
  if (data->app->state != STATE_VISUALIZING) {
    return;
  }

  printf("Step %d, Algorithm: %d, IsGraph: %d, AlgoState: %d\n",
         data->currentStep, data->app->selectedAlgorithm,
         data->app->isGraphAlgorithm, data->algoState);

  if (data->algoState == ALGO_STATE_FINISHED) {
    printf("[DEBUG] visualizer_update: Algorithm already in FINISHED state, "
           "skipping update\n");
    return;
  }

  if (data->app->isGraphAlgorithm) {

  } else {
    switch (data->app->selectedAlgorithm) {
    case ALGO_BUBBLE_SORT:
      bubble_sort_step(data);
      break;
    case ALGO_INSERTION_SORT:
      insertion_sort_step(data);
      break;
    case ALGO_SELECTION_SORT:
      selection_sort_step(data);
      break;
    case ALGO_MERGE_SORT:
      merge_sort_step(data);
      break;
    case ALGO_QUICK_SORT:
      quick_sort_step(data);

      printf("[DEBUG] After quick_sort_step, algoState=%d\n", data->algoState);
      break;
    default:
      break;
    }
  }

  data->currentStep++;
}

void visualizer_render(VisualizerData *data) {
  SDL_Renderer *renderer = data->app->renderer;

  draw_algorithm_name(data);

  draw_controls_info(data);

  if (data->app->isGraphAlgorithm) {

    render_graph(data);
  } else {

    render_array(data);
  }
}

void render_array(VisualizerData *data) {
  SDL_Renderer *renderer = data->app->renderer;
  int arraySize = data->arraySize;

  float barWidth = (float)WINDOW_WIDTH / arraySize;
  int maxHeight = WINDOW_HEIGHT - 150;
  int maxValue = 0;

  for (int i = 0; i < arraySize; i++) {
    if (data->array[i] > maxValue) {
      maxValue = data->array[i];
    }
  }

  for (int i = 0; i < arraySize; i++) {
    int height = (int)((float)data->array[i] / maxValue * maxHeight);
    SDL_Rect barRect = {(int)(i * barWidth), WINDOW_HEIGHT - height - 50,
                        (int)barWidth - 1, height};

    if (i == data->comparingIndices[0] || i == data->comparingIndices[1]) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    } else if (i == data->swappingIndices[0] || i == data->swappingIndices[1]) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    } else {

      bool isHighlighted = false;
      for (int j = 0; j < data->numHighlighted; j++) {
        if (i == data->highlightedIndices[j]) {
          isHighlighted = true;
          break;
        }
      }

      if (isHighlighted) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
      }
    }

    SDL_RenderFillRect(renderer, &barRect);
  }
}

void draw_algorithm_name(VisualizerData *data) {
  char *algorithmName = "";

  switch (data->app->selectedAlgorithm) {
  case ALGO_BUBBLE_SORT:
    algorithmName = "Bubble Sort";
    break;
  case ALGO_INSERTION_SORT:
    algorithmName = "Insertion Sort";
    break;
  case ALGO_SELECTION_SORT:
    algorithmName = "Selection Sort";
    break;
  case ALGO_MERGE_SORT:
    algorithmName = "Merge Sort";
    break;
  case ALGO_QUICK_SORT:
    algorithmName = "Quick Sort";
    break;
  case ALGO_BFS:
    algorithmName = "Breadth-First Search";
    break;
  case ALGO_DFS:
    algorithmName = "Depth-First Search";
    break;
  case ALGO_DIJKSTRA:
    algorithmName = "Dijkstra's Algorithm";
    break;
  default:
    algorithmName = "Unknown Algorithm";
  }

  SDL_Color textColor = {255, 255, 255, 255};
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(data->app->titleFont, algorithmName, textColor);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(data->app->renderer, textSurface);

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;

  SDL_Rect renderRect = {10, 10, textWidth, textHeight};
  SDL_RenderCopy(data->app->renderer, textTexture, NULL, &renderRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

void draw_controls_info(VisualizerData *data) {
  char *controlsText = data->app->state == STATE_PAUSED
                           ? "PAUSED - Press SPACE to continue, ESC for menu"
                           : "Press SPACE to pause, ESC for menu";

  SDL_Color textColor = {200, 200, 200, 255};
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(data->app->font, controlsText, textColor);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(data->app->renderer, textSurface);

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;

  SDL_Rect renderRect = {WINDOW_WIDTH - textWidth - 10, 10, textWidth,
                         textHeight};
  SDL_RenderCopy(data->app->renderer, textTexture, NULL, &renderRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

void visualizer_generate_random_array(VisualizerData *data) {
  for (int i = 0; i < data->arraySize; i++) {
    data->array[i] = rand() % 1000 + 1;
  }
}

void visualizer_generate_random_graph(VisualizerData *data) {
  graph_clear(data->graph);

  int numNodes = data->app->arraySize / 2;
  if (numNodes > 50)
    numNodes = 50;
  if (numNodes < 5)
    numNodes = 5;

  for (int i = 0; i < numNodes; i++) {
    int x = rand() % (WINDOW_WIDTH - 100) + 50;
    int y = rand() % (WINDOW_HEIGHT - 200) + 100;
    graph_add_node(data->graph, i, x, y);
  }

  int avgEdges = numNodes < 10 ? 2 : 3;
  for (int i = 0; i < numNodes; i++) {
    int numEdges = rand() % avgEdges + 1;
    for (int j = 0; j < numEdges; j++) {
      int target = rand() % numNodes;
      if (target != i) {
        int weight = rand() % 10 + 1;
        graph_add_edge(data->graph, i, target, weight);
      }
    }
  }
}

void visualizer_start_algorithm(VisualizerData *data) {
  data->app->state = STATE_VISUALIZING;
  data->currentStep = 0;
  data->comparingIndices[0] = data->comparingIndices[1] = -1;
  data->swappingIndices[0] = data->swappingIndices[1] = -1;
  data->numHighlighted = 0;

  reset_quicksort_state();
  reset_mergesort_state();

  if (data->app->isGraphAlgorithm) {
    switch (data->app->selectedAlgorithm) {
    case ALGO_BFS:
      bfs_init(data);
      break;
    case ALGO_DFS:
      dfs_init(data);
      break;
    case ALGO_DIJKSTRA:
      dijkstra_init(data);
      break;
    default:
      break;
    }
  } else {
    data->algoState = ALGO_STATE_INITIAL;
  }
}
