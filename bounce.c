
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define BALL_SYMBOL "O"
#define BORDER "#"
#define PADDLE "||"

#define SLEEP_TIME 20000

struct Ball {
    int px, py, dx, dy;
};

struct Paddle {
    int px, py, length;
};

int main(void) {
    // initialization
    int hits = 0, misses = 0, best = 0, streak = 0, quit = 0, frame = 1, max_y,
        max_x;
    struct Ball ball;
    struct Paddle paddle;
    system("clear");
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    nodelay(stdscr, 1);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    getmaxyx(stdscr, max_y, max_x);
    ball.py = max_y / 2;
    ball.px = max_x / 2;
    ball.dy = rand() & 1 ? 1 : -1;
    ball.dx = rand() & 1 ? 1 : -1;
    paddle.length = 6;
    paddle.py = (max_y / 2) - (paddle.length / 2);
    paddle.px = max_x - 3;

    // game logic
    while (!quit) {
        // drawing
        erase();
        getmaxyx(stdscr, max_y, max_x);
        mvprintw(
            0, 0,
            "Hits: %d     Misses: %d     Streak: %d     Best: %d     Time: %d",
            hits, misses, streak, best, frame / 50);  // draw stats
        attron(COLOR_PAIR(3));
        for (int i = 0; i < max_x; i++) {  // draw top borders
            mvprintw(1, i, BORDER);
            mvprintw(max_y - 1, i, BORDER);
        }
        for (int i = 1; i < max_y; i++) {  // draw side border
            mvprintw(i, 0, BORDER);
        }
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(1));
        paddle.px = max_x - 3;  // in case window was resized update paddle x-pos
        for (int i = 0; i < paddle.length; i++) {
            mvprintw(paddle.py + i, paddle.px, PADDLE);  // draw paddle
        }
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(2));
        mvprintw(ball.py, ball.px, BALL_SYMBOL);  // draw ball
        attroff(COLOR_PAIR(2));

        // collisions
        if ((ball.py + ball.dy) < 2 ||
            (ball.py + ball.dy) > max_y - 2) {  // ball-top collision
            ball.dy *= -1;
        }
        if ((ball.px + ball.dx) < 1) {  // ball-side collision
            ball.dx *= -1;
        }
        if ((ball.px + ball.dx == paddle.px) &&
            (ball.py + ball.dy >= paddle.py) &&
            (ball.py + ball.dy <=
             (paddle.py + paddle.length))) {  // ball-paddle collision
            hits++;
            streak++;
            if (streak > best) best = streak;
            ball.dx *= -1;
        }
        if (ball.px > max_x) {  // ball has passed the paddle
            misses++;
            streak = 0;
            ball.py = max_y / 2;
            ball.px = max_x / 2;
            ball.dy = rand() & 1 ? 1 : -1;
            ball.dx = rand() & 1 ? 1 : -1;
        }
        if (frame % 4 == 0) {
            ball.py += ball.dy;
            ball.px += ball.dx;
        }

        // input
        switch (getch()) {
            case 'j':
                if (paddle.py - 1 > 1) paddle.py -= 1;
                break;

            case 'k':
                if ((paddle.py + paddle.length) + 1 < max_y) paddle.py += 1;
                break;

            case 'Q':
                quit = 1;
                break;
        }
        frame++;
        usleep(SLEEP_TIME);
    }
    // exit
    endwin();
    system("clear");
    return 0;
}