#include <ncurses.h>
#include <cmath>
#include <chrono>
#include <utility>
#include <iostream>
#include <chrono>
#include <string>

void drawSideLine(const uint32_t &x, const uint32_t &y, const uint32_t &width)
{
    std::string str(width + 2, ' ');
    str.front() = '|';
    str.back() = '|';
    mvprintw(y, x, str.c_str());
}

void drawTopLine(const uint32_t &x, const uint32_t &y, const uint32_t &width)
{
    std::string str(width + 2, '-');
    str.front() = '|';
    str.back() = '|';
    mvprintw(y, x, str.c_str());
}

void drawBottomLine(const uint32_t &x, const uint32_t &y, const uint32_t &width)
{
    std::string str(width + 2, '-');
    str.front() = '|';
    str.back() = '|';
    mvprintw(y, x, str.c_str());
}

struct CanvasSize
{
    uint32_t height = 0;
    uint32_t width = 0;
};

CanvasSize lenderCanvas(const uint32_t &x, const uint32_t &y)
{
    constexpr uint32_t width = 60;
    constexpr uint32_t height = 30;
    drawTopLine(x, y, width);
    for (int i = 0; i < height; i += 1)
    {
        drawSideLine(x, y + i + 1, width);
    }
    drawBottomLine(x, y + height, width);
    return {height, width};
}

void drawPoint(const uint32_t &x, const uint32_t &y)
{
    mvprintw(y, x, "@");
}

int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    // nodelay(stdscr, TRUE);
    timeout(100);
    uint32_t point_x = 0;
    uint32_t point_y = 0;
    for (int i = 0;; i += 1)
    {
        auto start = std::chrono::high_resolution_clock::now();
        mvprintw(1, 0, "Hello World !!!");
        if (i % 2 == 0)
            mvprintw(2, 0, "float: %07.2f", (float)(i * 10.f));
        else
            mvprintw(2, 0, "float: %07.2f", 0.04f);
        auto size = lenderCanvas(0, 5);
        point_x++;
        point_y = std::sin(point_x * (360.f / size.width) / 360.f * M_PI) * static_cast<float>(size.height) / 2.0f + static_cast<float>(size.height) / 2.0f;
        // point_x = std::clamp(point_x, 0u, size.width);
        point_y = std::clamp(point_y, 0u, size.height);
        drawPoint(point_x % size.width + 0, point_y + 5);
        refresh();
        auto ch = getch();
        if (ch == 'w')
            point_y -= 1;
        else if (ch == 's')
            point_y += 1;
        else if (ch == 'a')
            point_x -= 1;
        else if (ch == 'd')
            point_x += 1;
        else if (ch == KEY_LEFT)
            point_x -= 1;
        else if (ch == KEY_RIGHT)
            point_x += 1;
        else if (ch == KEY_UP)
            point_y -= 1;
        else if (ch == KEY_DOWN)
            point_y += 1;
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
