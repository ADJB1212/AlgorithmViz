#include "ui.h"
#include <stdio.h>
#include <string.h>

char *algorithm_names[ALGO_COUNT] = {
    "Bubble Sort",        "Insertion Sort",      "Selection Sort",
    "Merge Sort",         "Quick Sort",          "Breadth-First Search",
    "Depth-First Search", "Dijkstra's Algorithm"};

Button create_button(int x, int y, int width, int height, char *text) {
  Button button;
  button.rect.x = x;
  button.rect.y = y;
  button.rect.w = width;
  button.rect.h = height;
  button.text = text;
  button.hover = false;
  button.selected = false;
  return button;
}

bool is_point_in_rect(int x, int y, SDL_Rect rect) {
  return (x >= rect.x && x < rect.x + rect.w && y >= rect.y &&
          y < rect.y + rect.h);
}

void draw_button(SDL_Renderer *renderer, TTF_Font *font, Button button) {

  if (button.selected) {
    SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
  } else if (button.hover) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 50, 50, 150, 255);
  }
  SDL_RenderFillRect(renderer, &button.rect);

  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderDrawRect(renderer, &button.rect);

  SDL_Color textColor = {255, 255, 255, 255};
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, button.text, textColor);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_Rect textRect = {button.rect.x + (button.rect.w - textSurface->w) / 2,
                       button.rect.y + (button.rect.h - textSurface->h) / 2,
                       textSurface->w, textSurface->h};

  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

void draw_slider(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                 int width, int height, int min_value, int max_value,
                 int *value, char *label) {

  SDL_Color textColor = {255, 255, 255, 255};
  SDL_Surface *labelSurface = TTF_RenderText_Solid(font, label, textColor);
  SDL_Texture *labelTexture =
      SDL_CreateTextureFromSurface(renderer, labelSurface);

  SDL_Rect labelRect = {x, y - labelSurface->h - 5, labelSurface->w,
                        labelSurface->h};

  SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);
  SDL_Rect trackRect = {x, y, width, height};
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  SDL_RenderFillRect(renderer, &trackRect);
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderDrawRect(renderer, &trackRect);

  float percentage = (*value - min_value) / (float)(max_value - min_value);
  int handlePos = x + (int)(percentage * width);

  SDL_Rect handleRect = {handlePos - 5, y - 5, 10, height + 10};
  SDL_SetRenderDrawColor(renderer, 150, 150, 255, 255);
  SDL_RenderFillRect(renderer, &handleRect);
  SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
  SDL_RenderDrawRect(renderer, &handleRect);

  char valueText[10];
  sprintf(valueText, "%d", *value);

  SDL_Surface *valueSurface = TTF_RenderText_Solid(font, valueText, textColor);
  SDL_Texture *valueTexture =
      SDL_CreateTextureFromSurface(renderer, valueSurface);

  SDL_Rect valueRect = {x + width + 10, y + (height - valueSurface->h) / 2,
                        valueSurface->w, valueSurface->h};

  SDL_RenderCopy(renderer, valueTexture, NULL, &valueRect);

  SDL_FreeSurface(labelSurface);
  SDL_DestroyTexture(labelTexture);
  SDL_FreeSurface(valueSurface);
  SDL_DestroyTexture(valueTexture);
}

void handle_event(AppContext *app, VisualizerData *visualizer, SDL_Event *e) {
  static int draggingSlider = 0;

  if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
    case SDLK_SPACE:
      if (app->state == STATE_VISUALIZING) {
        app->state = STATE_PAUSED;
      } else if (app->state == STATE_PAUSED) {
        app->state = STATE_VISUALIZING;
      }
      break;
    case SDLK_ESCAPE:
      if (app->state == STATE_VISUALIZING || app->state == STATE_PAUSED) {
        app->state = STATE_MENU;
      }
      break;
    case SDLK_RETURN:
      if (app->state == STATE_MENU) {
        app->state = STATE_VISUALIZING;
        visualizer_reset(visualizer);
        visualizer_start_algorithm(visualizer);
      }
      break;
    }
  } else if (e->type == SDL_MOUSEBUTTONDOWN) {
    int x = e->button.x;
    int y = e->button.y;

    if (app->state == STATE_MENU) {

      for (int i = 0; i < ALGO_COUNT; i++) {
        SDL_Rect buttonRect = {150, 150 + i * 50, 300, 40};

        if (is_point_in_rect(x, y, buttonRect)) {
          app->selectedAlgorithm = i;
          app->isGraphAlgorithm = (i >= ALGO_BFS);
        }
      }

      SDL_Rect startButtonRect = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 100,
                                  200, 50};

      if (is_point_in_rect(x, y, startButtonRect)) {
        app->state = STATE_VISUALIZING;
        visualizer_reset(visualizer);
        visualizer_start_algorithm(visualizer);
      }

      SDL_Rect sliderRect = {150, WINDOW_HEIGHT - 150, 300, 20};

      if (is_point_in_rect(x, y, sliderRect) ||
          is_point_in_rect(x, y,
                           (SDL_Rect){sliderRect.x - 5, sliderRect.y - 5,
                                      sliderRect.w + 10, sliderRect.h + 10})) {
        draggingSlider = 1;

        float percentage = (x - sliderRect.x) / (float)sliderRect.w;
        app->arraySize = MIN_ARRAY_SIZE +
                         (int)(percentage * (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE));

        if (app->arraySize < MIN_ARRAY_SIZE)
          app->arraySize = MIN_ARRAY_SIZE;
        if (app->arraySize > MAX_ARRAY_SIZE)
          app->arraySize = MAX_ARRAY_SIZE;
      }
    }
  } else if (e->type == SDL_MOUSEBUTTONUP) {
    draggingSlider = 0;
  } else if (e->type == SDL_MOUSEMOTION) {
    int x = e->motion.x;
    int y = e->motion.y;

    if (draggingSlider == 1) {
      SDL_Rect sliderRect = {150, WINDOW_HEIGHT - 150, 300, 20};

      float percentage = (x - sliderRect.x) / (float)sliderRect.w;
      app->arraySize = MIN_ARRAY_SIZE +
                       (int)(percentage * (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE));

      if (app->arraySize < MIN_ARRAY_SIZE)
        app->arraySize = MIN_ARRAY_SIZE;
      if (app->arraySize > MAX_ARRAY_SIZE)
        app->arraySize = MAX_ARRAY_SIZE;
    }
  }
}

void draw_menu(AppContext *app) {
  SDL_Renderer *renderer = app->renderer;

  SDL_Color titleColor = {255, 255, 255, 255};
  SDL_Surface *titleSurface =
      TTF_RenderText_Solid(app->titleFont, "Algorithm Visualizer", titleColor);
  SDL_Texture *titleTexture =
      SDL_CreateTextureFromSurface(renderer, titleSurface);

  SDL_Rect titleRect = {WINDOW_WIDTH / 2 - titleSurface->w / 2, 50,
                        titleSurface->w, titleSurface->h};

  SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

  for (int i = 0; i < ALGO_COUNT; i++) {
    Button button =
        create_button(150, 150 + i * 50, 300, 40, algorithm_names[i]);

    if (i == app->selectedAlgorithm) {
      button.selected = true;
    }

    draw_button(renderer, app->font, button);
  }

  SDL_Color labelColor = {200, 200, 255, 255};

  draw_slider(renderer, app->font, 150, WINDOW_HEIGHT - 150, 300, 20,
              MIN_ARRAY_SIZE, MAX_ARRAY_SIZE, &app->arraySize,
              "Array Size / Graph Nodes:");

  Button startButton =
      create_button(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 100, 200, 50,
                    "Start Visualization");

  draw_button(renderer, app->font, startButton);

  SDL_FreeSurface(titleSurface);
  SDL_DestroyTexture(titleTexture);
}
