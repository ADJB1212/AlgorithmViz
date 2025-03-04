#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define MAX_ARRAY_SIZE 1000
#define MIN_ARRAY_SIZE 10
#define DEFAULT_ARRAY_SIZE 100

typedef enum {
  ALGO_BUBBLE_SORT,
  ALGO_INSERTION_SORT,
  ALGO_SELECTION_SORT,
  ALGO_MERGE_SORT,
  ALGO_QUICK_SORT,
  ALGO_BFS,
  ALGO_DFS,
  ALGO_DIJKSTRA,
  ALGO_COUNT
} AlgorithmType;

typedef enum { STATE_MENU, STATE_VISUALIZING, STATE_PAUSED } AppState;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  TTF_Font *titleFont;
  int arraySize;
  AlgorithmType selectedAlgorithm;
  AppState state;
  bool running;
  bool isGraphAlgorithm;
} AppContext;

#endif
