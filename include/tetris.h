#ifndef TETRIS_H
#define TETRIS_H
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 600
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define STARTING_CAPACITY 10

typedef enum piece_color {BLUE, GREEN, PURPLE, RED, YELLOW} piece_color;
typedef enum shape_type {L, T, Z, LINE} shape_type;

typedef struct application {
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Surface **pieces;
} application;

typedef struct coordinate {
    int x;
    int y;
} coordinate;

coordinate Lpos[] = {{0, 0}, {0, 25}, {0, 50}, {25, 50}};
coordinate L1pos[] = {{0, 0}, {0, 25}, {25, 0}, {50, 0}};
coordinate L2pos[] = {{0, 0}, {25, 0}, {25, 25}, {25, 50}};
coordinate L3pos[] = {{0, 25}, {25, 25}, {50, 25}, {50, 0}};
coordinate Tpos[] = {{25, 0}, {0, 25}, {25, 25}, {50, 25}};
coordinate T1pos[] = {{0, 0}, {0, 25}, {0, 50}, {25, 25}};
coordinate T2pos[] = {{0, 0}, {25, 0}, {50, 0}, {25, 25}};
coordinate T3pos[] = {{25, 0}, {25, 25}, {25, 50}, {0, 25}};
coordinate Zpos[] = {{0, 0}, {0, 25}, {25, 25}, {25, 50}};
coordinate Z1pos[] = {{0, 25}, {25, 25}, {25, 0}, {50, 0}};
coordinate LINEpos[] = {{0, 0}, {25, 0}, {50, 0}, {75, 0}};
coordinate LINE1pos[] = {{0, 0}, {0, 25}, {0, 50}, {0, 75}};

typedef struct shape {
    piece_color color;
    coordinate pos;
    coordinate *offsets;
    shape_type type;
} shape;

typedef struct all_shapes {
    shape **shapes;
    int capacity;
    int size;
} all_shapes;

int is_left_pressed = 0;
int is_right_pressed = 0;
int is_down_pressed = 0;
int is_up_pressed = 0;
int stick_shape = 0;


void load_pieces(SDL_Surface ***pieces);
void delete_pieces(SDL_Surface **pieces);
void handle_keydown(SDL_Keycode key, int *to_quit);
void handle_keyup(SDL_Keycode key);
void create_shape(shape **shape, piece_color color, coordinate pos, shape_type which_shape);
void render_shape(shape *shape, application *app);
void render_all_shapes(all_shapes *shapes, application *app);
void add_shape(shape *shape, all_shapes *shapes);
void delete_all_shapes(all_shapes *shapes);
void update_shape_pos(shape *current_shape, all_shapes *shapes);
int check_shape_collision(shape *current_shape, all_shapes *shapes);

#endif