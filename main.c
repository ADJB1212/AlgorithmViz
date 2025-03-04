
#include "main.h"
#include "ui.h"
#include "visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  if (TTF_Init() < 0) {
    printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
    SDL_Quit();
    return -1;
  }

  AppContext app;
  app.running = true;
  app.state = STATE_MENU;
  app.arraySize = DEFAULT_ARRAY_SIZE;
  app.selectedAlgorithm = ALGO_BUBBLE_SORT;
  app.isGraphAlgorithm = false;

  app.window = SDL_CreateWindow("Algorithm Visualizer", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!app.window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    TTF_Quit();
    SDL_Quit();
    return -1;
  }

  app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
  if (!app.renderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(app.window);
    TTF_Quit();
    SDL_Quit();
    return -1;
  }

  app.font = TTF_OpenFont("arial.ttf", 24);
  app.titleFont = TTF_OpenFont("arial.ttf", 36);
  if (!app.font || !app.titleFont) {
    printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
    printf("Make sure arial.ttf is in the executable directory\n");
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    TTF_Quit();
    SDL_Quit();
    return -1;
  }

  srand(time(NULL));

  VisualizerData *visualizer = visualizer_init(&app);

  SDL_Event e;
  while (app.running) {

    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        app.running = false;
      } else {
        handle_event(&app, visualizer, &e);
      }
    }

    SDL_SetRenderDrawColor(app.renderer, 20, 20, 30, 255);
    SDL_RenderClear(app.renderer);

    if (app.state == STATE_MENU) {
      draw_menu(&app);
    } else {
      visualizer_update(visualizer);
      visualizer_render(visualizer);
    }

    SDL_RenderPresent(app.renderer);

    SDL_Delay(16);
  }

  visualizer_destroy(visualizer);
  TTF_CloseFont(app.font);
  TTF_CloseFont(app.titleFont);
  SDL_DestroyRenderer(app.renderer);
  SDL_DestroyWindow(app.window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
