#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "snake.h"

#define CENTER_H (((LINES - 1) >> 1) - 1)
#define CENTER_V (((COLS - 1) >> 1) - 1)

// initscr();
// cbreak();
// noecho();
// clear();
// available functions
// addch(char)
// addstr(str)
// mvaddch(y, x, char)
// mvaddstr(y, x, str)
// refresh()
// LINES
// COLS

int msleep(long tms) {
    struct timespec ts;
    int ret;
    if (tms < 0)
        return 1;
    ts.tv_sec = tms / 1000;
    ts.tv_nsec = (tms % 1000) * 1000000;
    ret = nanosleep(&ts, &ts);
    return ret;
}

int main(void) {
    int i;
    char running = 1;
    int action;
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();

    if (LINES < 5 || COLS < 10) {
        mvaddstr(0, 0, "The windows is too small...");
        mvaddstr(1, 0, "Please increase the size");
        mvaddstr(2, 0, "and start again.");
        getch();
        endwin();
        exit(1);
    }

    Snake snake = create_snake(LINES * COLS, CENTER_V, CENTER_H);
    Food food = generate_food(snake, NULL);

    keypad(stdscr, TRUE);
    //nodelay(stdscr, TRUE);
    first_draw(snake, food);
    while (running) {
        switch (getch()) {
        case KEY_UP:
            if (snake->facing != BOTTOM)
                update_direction(snake, TOP);
            break;
        case KEY_RIGHT:
            if (snake->facing != LEFT)
                update_direction(snake, RIGHT);
            break;
        case KEY_DOWN:
            if (snake->facing != TOP) 
                update_direction(snake, BOTTOM);
            break;
        case KEY_LEFT:
            if (snake->facing != RIGHT)
                update_direction(snake, LEFT);
            break;
        case 81:
        case 113:
            endwin();
            exit(0);
        default:
            break;
        }
//        if (snake->facing == TOP || snake->facing == BOTTOM)
            //msleep(100);
            //msleep(500);
        //else
//            //msleep(60);
        action = move_snake(snake, food);
        //msleep(300);
        switch (action)
        {
        case 1:
            running = 0;
            mvprintw(LINES - 1, 0, "Game Over. Your score: %d", snake->size);
            nodelay(stdscr, FALSE);
            getch();
            break;
        case 2:
            running = 0;
            mvaddstr(LINES - 1, 0, "You win!!");
            nodelay(stdscr, FALSE);
            getch();
            break;
        default:
            break;
        }
    }
    if (food != NULL)
        free(food);
    destroy_snake(snake);
    endwin();
    exit(0);
}