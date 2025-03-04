#ifndef UI_H
#define UI_H

#include "main.h"
#include "visualizer.h"

typedef struct {
  SDL_Rect rect;
  char *text;
  bool hover;
  bool selected;
} Button;

void handle_event(AppContext *app, VisualizerData *visualizer, SDL_Event *e);
void draw_menu(AppContext *app);
Button create_button(int x, int y, int width, int height, char *text);
bool is_point_in_rect(int x, int y, SDL_Rect rect);
void draw_button(SDL_Renderer *renderer, TTF_Font *font, Button button);
void draw_slider(SDL_Renderer *renderer, TTF_Font *font, int x, int y,
                 int width, int height, int min_value, int max_value,
                 int *value, char *label);

#endif
