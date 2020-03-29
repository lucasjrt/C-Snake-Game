#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

typedef struct snake *Snake;
typedef struct coordinate *Coordinate;
typedef Coordinate Food;

struct coordinate {
    int x;
    int y;
};

struct snake
{
    int size;
    Coordinate body;
    char facing;
    char prev_facing;
    int head;
};

Snake create_snake(int max_size, int scrhalf_x, int scrhalf_y);
void destroy_snake(Snake snake);
void first_draw(Snake snake, Food food);
Food generate_food(Snake snake, Food food);
void update_direction(Snake snake, char direction);
int move_snake(Snake snake, Food food_position);

#endif