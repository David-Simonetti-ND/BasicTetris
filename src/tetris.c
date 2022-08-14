#include "../include/tetris.h"

int main() 
{
    srand((unsigned int)time(0));
    application app = {0, 0, NULL};
    load_pieces(&app.pieces);
    SDL_Init(SDL_INIT_VIDEO); 
    app.window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    app.window_surface = SDL_GetWindowSurface(app.window);

    all_shapes *shapes = malloc(sizeof(all_shapes));
    shapes->capacity = STARTING_CAPACITY;
    shapes->size = 0;
    shapes->shapes = malloc(sizeof(shape *) * STARTING_CAPACITY);
    
    shape *current_shape = NULL;
    create_shape(&current_shape, (piece_color) (random() % 5), (coordinate) {100, 50}, (shape_type) (random() % 4));
    add_shape(current_shape, shapes);

    render_all_shapes(shapes, &app);
    SDL_UpdateWindowSurface(app.window);

    int to_quit = 0;
	SDL_Event event;
	while (!to_quit) {
		if (SDL_PollEvent( & event)) {
            switch (event.type) {
                case SDL_QUIT:
                    to_quit = 1;
                    break;
                case SDL_KEYDOWN:
                    handle_keydown(event.key.keysym.sym, &to_quit);
                    break;
                case SDL_KEYUP:
                    handle_keyup(event.key.keysym.sym);
                    break;
            }            
            SDL_FillRect(app.window_surface, NULL, 0);
            update_shape_pos(current_shape, shapes);
            if (stick_shape) {
                create_shape(&current_shape, (piece_color) random() % 5, (coordinate) {100, 50}, (shape_type) random() % 4);
                add_shape(current_shape, shapes);
                stick_shape = 0;
            }
            render_all_shapes(shapes, &app);
            SDL_UpdateWindowSurface(app.window);
		}
	}

    SDL_DestroyWindow(app.window);
    SDL_Quit();

    delete_all_shapes(shapes);
}

void load_pieces(SDL_Surface ***pieces) {
    *pieces = malloc(sizeof(SDL_Surface *) * 5);
    (*pieces)[0] = IMG_Load("data/blue.png");
    (*pieces)[1] = IMG_Load("data/green.png");
    (*pieces)[2] = IMG_Load("data/purple.png");
    (*pieces)[3] = IMG_Load("data/red.png");
    (*pieces)[4] = IMG_Load("data/yellow.png");
}

void delete_pieces(SDL_Surface **pieces) {
    for (int i = 0; i < 5; i++) {
        SDL_FreeSurface(pieces[i]);
    }
    free(pieces);
}

void create_shape(shape **sshape, piece_color color, coordinate pos, shape_type which_shape) {
    *sshape = malloc(sizeof(shape));
    (*sshape)->color = color;
    (*sshape)->pos = pos;
    switch (which_shape) {
        case L:
            (*sshape)->offsets = Lpos;
            break;
        case T:
            (*sshape)->offsets = Tpos;
            break;
        case LINE:
            (*sshape)->offsets = LINEpos;
            break;
        case Z:
            (*sshape)->offsets = Zpos;
            break;
    }
}

void render_shape(shape *shape, application *app) {
    SDL_Rect base_position = {0, 0, 25, 25};
    SDL_Rect render_position = {0, 0, 25, 25};
    base_position.x = shape->pos.x;
    base_position.y = shape->pos.y;
    for (int i = 0; i < 4; i++) {
        render_position = base_position;
        render_position.x += shape->offsets[i].x;
        render_position.y += shape->offsets[i].y;
        SDL_BlitSurface(app->pieces[shape->color], NULL, app->window_surface, &render_position);
    }
    
}

void render_all_shapes(all_shapes *shapes, application *app) {
    for (int i = 0; i < shapes->size; i++) {
        render_shape(shapes->shapes[i], app);
    }
}

void add_shape(shape *shp, all_shapes *shapes) {
    if (shapes->capacity == (shapes->size + 1)) {
        shapes->shapes = realloc(shapes->shapes, sizeof(shape *) * (unsigned long)shapes->capacity * 2);
        shapes->capacity *= 2;
    }
    shapes->shapes[shapes->size] = shp;
    shapes->size++;
}

void delete_all_shapes(all_shapes *shapes) {
    for (int i = 0; i < shapes->size; i++) {
        free(shapes->shapes[i]);
    }
    free(shapes->shapes);
    free(shapes);
}

void update_shape_pos(shape *current_shape, all_shapes *shapes) {
    if (is_up_pressed) {
        if (current_shape->offsets == Lpos) {
            current_shape->offsets = L1pos;
        }
        else if (current_shape->offsets == L1pos) {
            current_shape->offsets = L2pos;
        }
        else if (current_shape->offsets == L2pos) {
            current_shape->offsets = L3pos;
        }
        else if (current_shape->offsets == L3pos) {
            current_shape->offsets = Lpos;
        }
        if (current_shape->offsets == Tpos) {
            current_shape->offsets = T1pos;
        }
        else if (current_shape->offsets == T1pos) {
            current_shape->offsets = T2pos;
        }
        else if (current_shape->offsets == T2pos) {
            current_shape->offsets = T3pos;
        }
        else if (current_shape->offsets == T3pos) {
            current_shape->offsets = Tpos;
        }
        if (current_shape->offsets == Zpos) {
            current_shape->offsets = Z1pos;
        }
        else if (current_shape->offsets == Z1pos) {
            current_shape->offsets = Zpos;
        }
        if (current_shape->offsets == LINEpos) {
            current_shape->offsets = LINE1pos;
        }
        else if (current_shape->offsets == LINE1pos) {
            current_shape->offsets = LINEpos;
        }
        is_up_pressed = 0;
    }
    if (is_down_pressed) {
        current_shape->pos.y += 25;
        for (int i = 0; i < 4; i++) {
            int ypos = current_shape->offsets[i].y + current_shape->pos.y;
            if (ypos > ((BOARD_HEIGHT + 3) * 25)) {
                current_shape->pos.y -= 25;
                stick_shape = 1;
            }
        }
        if (check_shape_collision(current_shape, shapes)) {
            current_shape->pos.y -= 25;
            stick_shape = 1;
        }
        is_down_pressed = 0;
    }
    if (is_left_pressed) {
        current_shape->pos.x -= 25;
        for (int i = 0; i < 4; i++) {
            int xpos = current_shape->offsets[i].x + current_shape->pos.x;
            if (xpos < 50) {
                current_shape->pos.x += 25;
            }
        }
        if (check_shape_collision(current_shape, shapes)) {
            current_shape->pos.x += 25;
        }
        is_left_pressed = 0;
    }
    if (is_right_pressed) {
        current_shape->pos.x += 25;
        for (int i = 0; i < 4; i++) {
            int xpos = current_shape->offsets[i].x + current_shape->pos.x;
            if (xpos > ((BOARD_WIDTH + 3) * 25)) {
                current_shape->pos.x -= 25;
            }
        }
        if (check_shape_collision(current_shape, shapes)) {
            current_shape->pos.x -= 25;
        }
        is_right_pressed = 0;
    }
}

int check_shape_collision(shape *current_shape, all_shapes *shapes) {
    for (int i = 0; i < shapes->size; i++) {
        if (shapes->shapes[i] == current_shape) {continue;}
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                int piecex = shapes->shapes[i]->offsets[j].x + shapes->shapes[i]->pos.x;
                int piecey = shapes->shapes[i]->offsets[j].y + shapes->shapes[i]->pos.y;
                int shapex = current_shape->offsets[k].x + current_shape->pos.x;
                int shapey = current_shape->offsets[k].y + current_shape->pos.y;
                if ( (piecex == shapex) && (piecey == shapey)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void handle_keydown(SDL_Keycode key, int *to_quit) {
    switch (key) {
        case 'q':
            *to_quit = 1;
            break;
        case 'w':
            is_up_pressed = 1;
            is_down_pressed = 0;
            break;
        case 'a':
            is_left_pressed = 1;
            is_right_pressed = 0;
            break;
        case 's':
            is_down_pressed = 1;
            is_up_pressed = 0;
            break;
        case 'd':
            is_left_pressed = 0;
            is_right_pressed = 1;
            break;
    }
}
void handle_keyup(SDL_Keycode key) {
    switch (key) {
        case 'w':
            is_up_pressed = 0;
            break;
        case 'a':
            is_left_pressed = 0;
            break;
        case 's':
            is_down_pressed = 0;
            break;
        case 'd':
            is_right_pressed = 0;
            break;
    }
}