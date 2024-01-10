#include <ncurses.h>
#include <chrono>

#include <iostream>
#include <chrono>

int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    // nodelay(stdscr, TRUE);
    timeout(100);

    for (int i = 0;; i += 1)
    {
        auto start = std::chrono::high_resolution_clock::now();
        mvprintw(1, 0, "Hello World !!!");
        if(i % 2 == 0)
            mvprintw(2, 0, "float: %07.2f", (float)(i * 10.f));
        else
            mvprintw(2, 0, "float: %07.2f", 0.04f);
        refresh();
        auto ch = getch();
        if (ch == 'q')
            break;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        float frameRate = 1000.0f / static_cast<float>(duration);
        mvprintw(3, 0, "FPS: %7.6f", frameRate);

    }


    endwin();
    return 0;
}
