//* SETUP
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 20
#define DELAY 100000
#define WALL '#'
#define FOOD '@'
#define SNAKE 'D'
#define SNAKE_BODY '='
#undef FOOD
#define FOOD '*'

char direction[100] = "RIGHT";
int snake_x, snake_y, food_x, food_y, score;
int tail_X[100], tail_Y[100]; // snake tail's storage
int nTail;

//* * * * * * * * * * * * * * * *

void init_game()
{
    initscr(); //? START THE NCURSE
    clear();
    noecho();
    cbreak();    //? DISABLE LINE BUFFERING
    curs_set(0); //? HIDE THE CURSOR
    timeout(0);  //? non-blocking input

    snake_x = WIDTH / 2;
    snake_y = HEIGHT / 2;
    srand(time(NULL));
    food_x = rand() % WIDTH - 1;
    food_y = rand() % HEIGHT - 1;
    score = 0;
    nTail = 0;
}

void draw_board()
{
    //? clear the console
    clear();

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {

            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                //? DRAW THE WALL
                mvprintw(i, j, "%c", WALL);
            }
            else if (i == snake_y && j == snake_x)
            {
                //? DRAW THE SNAKE
                mvprintw(i, j, "%c", SNAKE);
            }
            else if (i == food_y && j == food_x)
            {
                //? DRAW THE FOOD
                mvprintw(i, j, "%c", FOOD);
            }
            else
            {
                int isTail = 0;
                for (int k = 0; k < nTail; ++k)
                {
                    if (tail_X[k] == j && tail_Y[k] == i)
                    {
                        mvprintw(i, j, "%c", SNAKE_BODY);
                        isTail = 1;
                    }
                }

                if (!isTail)
                    mvprintw(i, j, " ");
            }
        }
    }

    mvprintw(HEIGHT + 1, 0, "Score: %d", score);
    refresh();
}

//* THIS PART MAKE BY CHATGPT cause i don't know how do to it XD
void move_snake(int dx, int dy)
{
    int prev_x = tail_X[0];
    int prev_y = tail_Y[0];
    int prev2_x, prev2_y;
    tail_X[0] = snake_x;
    tail_Y[0] = snake_y;

    for (int i = 1; i < nTail; ++i)
    {
        prev2_x = tail_X[i];
        prev2_y = tail_Y[i];
        tail_X[i] = prev_x;
        tail_Y[i] = prev_y;
        prev_x = prev2_x;
        prev_y = prev2_y;
    }

    snake_x += dx;
    snake_y += dy;

    if (snake_x == food_x && snake_y == food_y)
    {
        ++nTail;
        ++score;
        food_x = rand() % WIDTH;
        food_y = rand() % HEIGHT;
    }

    // Collision with walls
    if (snake_x <= 0 || snake_x >= WIDTH - 1 || snake_y <= 0 || snake_y >= HEIGHT - 1)
    {
        endwin();
        printf("Game Over! Your score: %d\n", score);
        exit(0);
    }

    // Collision with self
    for (int i = 0; i < nTail; ++i)
    {
        if (tail_X[i] == snake_x && tail_Y[i] == snake_y)
        {
            endwin();
            printf("Game Over! Your score: %d\n", score);
            exit(0);
        }
    }
}

//* * * * * * * * * * * * * * * * * * * * * * * * * * *

int main()
{
    int dx = 1, dy = 0;
    printf("Run: snake()");
    init_game();

    while (1)
    {
        usleep(DELAY);
        int ch = getch();

        switch (ch)
        {
        case 'w':
            if (strcmp(direction, "left") != 0 || strcmp(direction, "right") != 0)
            {
                dx = 0;
                dy = -1;
                strcpy(direction, "up");
            }
            break;
        case 's':
            if (strcmp(direction, "left") != 0 || strcmp(direction, "right") != 0)
            {
                dx = 0;
                dy = 1;
            }
            break;
        case 'a':
            if (strcmp(direction, "up") != 0 || strcmp(direction, "down") != 0)
            {
            dx = -1;
            dy = 0;
            }
            break;
        case 'd':
            if (strcmp(direction, "up") != 0 || strcmp(direction, "down") != 0)
            {
            dx = 1;
            dy = 0;
            }
            break;
        }

        move_snake(dx, dy);
        draw_board();
    }

    endwin();
    return 0;
}