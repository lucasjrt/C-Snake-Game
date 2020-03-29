#include <stdlib.h>
#include <curses.h>

#include "snake.h"

#define INITIAL_SIZE 5

Snake create_snake(int max_size, int scrhalf_x, int scrhalf_y) {
    int i;
    Snake snake = (Snake)malloc(sizeof(struct snake));
    snake->body = (Coordinate)malloc(max_size * sizeof(struct coordinate));
    snake->size = INITIAL_SIZE;
    for (i = 0; i < INITIAL_SIZE; i++) {
        snake->body[i].x = scrhalf_x - i;
        snake->body[i].y = scrhalf_y;
    }
    snake->head = 0;
    snake->facing = RIGHT;
    return snake;
}

void destroy_snake(Snake snake) {
    if (snake != NULL && snake->body != NULL) {
        free(snake->body);
        snake->body = NULL;
    }
    if (snake != NULL) {
        free(snake);
        snake = NULL;
    }
}

void first_draw(Snake snake, Food food) {
    int i;
    for (i = 0; i < snake->size; i++)
        if (i == snake->head)
            mvaddch(snake->body[i].y, snake->body[i].x, '#');
        else
            mvaddch(snake->body[i].y, snake->body[i].x, 'O');
    mvaddch(food->y, food->x, '*');
}

Food generate_food(Snake snake, Food food) {
    if (snake->size == LINES * COLS) {
        if (food != NULL) {
            free(food);
            food = NULL;
        }
        return food;
    }

    int i;
    if (food == NULL)
        food = (Food)malloc(sizeof(struct coordinate));
    char valid = 1;
    food->x = rand() % COLS;
    food->y = rand() % LINES;
    for (i = 0; i < snake->size; i++) {
        if (food->x == snake->body[i].x && food->y == snake->body[i].y) {
            valid = 0;
            break;
        }
    }
    if (!valid) {
        Coordinate conflict = (Coordinate)malloc(sizeof(struct coordinate));
        if (food->x < COLS - 1)
            conflict->x = food->x++;
        else {
            conflict->x = 0;
            if (food->y < LINES - 1)
                conflict->y = food->y++;
            else
                conflict->y = 0;
        }
        while (1) {
            valid = 1;
            for (i = 0; i < snake->size; i++) {
                if (conflict->x == snake->body[i].x && conflict->y == snake->body[i].y) {
                    valid = 0;
                    break;
                }
            }
            if (!valid) {
                if (conflict->x < COLS - 1)
                    conflict->x++;
                else {
                    conflict->x = 0;
                    if (conflict->y < LINES - 1)
                        conflict->y++;
                    else
                        conflict->y = 0;
                }
            } else {
                food->x = conflict->x;
                food->y = conflict->y;
                free(conflict);
                conflict = NULL;
                break;
            }
        }
    }
    return food;
}

void update_direction(Snake snake, char facing) {
    snake->facing = facing;
}

int move_snake(Snake snake, Coordinate food) {
    int i;
    int next_head = (snake->size + snake->head - 1) % snake->size;
    Coordinate new_head = (Coordinate)malloc(sizeof(struct coordinate));
    new_head->x = snake->body[snake->head].x;
    new_head->y = snake->body[snake->head].y;
    mvaddch(snake->body[snake->head].y, snake->body[snake->head].x, 'O');
    //snake->body[next_head] = snake->body[snake->head];
    switch (snake->facing) {
    case TOP:
        if (snake->body[snake->head].y == 0)
            return 1;
        new_head->y--;
        break;
    case RIGHT:
        if (snake->body[snake->head].x == COLS - 1)
            return 1;
        new_head->x++;
        break;
    case BOTTOM:
        if (snake->body[snake->head].y == LINES - 1)
            return 1;
        new_head->y++;
        break;
    case LEFT:
        if (snake->body[snake->head].x == 0)
            return 1;
        new_head->x--;
        break;
    default:
        return 1;
    }
    if (new_head->x == food->x && new_head->y == food->y) {
        snake->size++;
        for (i = snake->size - 1; i > snake->head; i--)
            snake->body[i] = snake->body[i - 1];
        snake->body[snake->head].x = new_head->x;
        snake->body[snake->head].y = new_head->y;
        food = generate_food(snake, food);
        if (food == NULL)
            return 2;
    } else {
        mvaddch(snake->body[next_head].y, snake->body[next_head].x, ' ');
        snake->head = next_head;
        snake->body[snake->head].x = new_head->x;
    snake->body[snake->head].y = new_head->y;
    }
    for (i = 0; i < snake->size; i++) {
        if (snake->head != i && snake->body[snake->head].x == snake->body[i].x && snake->body[snake->head].y == snake->body[i].y)
            return 1;
    }
    mvaddch(snake->body[snake->head].y, snake->body[snake->head].x, '#');
    mvaddch(food->y, food->x, '*');
    //mvprintw(0, 0, "head: %d", snake->head);
    //mvprintw(2, 0, "tail index: %d", next_head);
    //int j;
    //for (j = 0; j < snake->size; j++) {
    //    mvprintw(3 + j, 0, "%3d: %3d %3d", j, snake->body[j].x, snake->body[j].y);
    //}
    return 0;
}