#include <GLFW/glfw3.h>
#include <unistd.h>
#include <iostream>
#include <cmath>

constexpr float SCREEN_X = 480.0f; // 縦
constexpr float SCREEN_Y = 480.0f; // 横

struct Paint
{
    static bool initialized;
    GLFWwindow *window;
    Paint()
    {
        if (initialized)
        {
            throw std::runtime_error("Already initialized");
        }
        initialized = true;
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        window = glfwCreateWindow(SCREEN_Y, SCREEN_X, "GLFW Point", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create window");
        }
        glfwMakeContextCurrent(window);
        std::cout << "Initialized" << std::endl;
    }

    ~Paint()
    {
        if (initialized)
        {
            glfwTerminate();
            std::cout << "Terminated glfw." << std::endl;
        }
    }

    void drawRectangle(const float x, const float y)
    {
        const float rect_size = 0.05; // 1辺の大きさ
        const float float_x = x / SCREEN_X;
        const float float_y = y / SCREEN_Y;
        glBegin(GL_QUADS);
        glVertex2f(float_x - rect_size, float_y - rect_size); // Bottom left corner
        glVertex2f(float_x - rect_size, float_y + rect_size); // Bottom right corner
        glVertex2f(float_x + rect_size, float_y + rect_size); // Top right corner
        glVertex2f(float_x + rect_size, float_y - rect_size); // Top left corner
        glEnd();
    }

    bool drawDisplay()
    {
        static float count = 0.0f;
        if (!glfwWindowShouldClose(window))
        {
            // 終了しなくてはいけない状況ならfalseを返す
            std::cout << "Finish drawing" << std::endl;
            return false;
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
            count += 0.01f;
            float off_x = std::sin(count) * 60;
            float off_y = std::cos(count) * 60;
            drawRectangle(50 + off_x, 50 + off_y);
            glfwSwapBuffers(window);
            glfwPollEvents();
            return true;
        }
    }
};

bool Paint::initialized = false;

int main()
{

    Paint paint;
    try
    {
        while (true)
        {
            if(!paint.drawDisplay()){
                std::cout << "break" << std::endl;
                break;
            }
            std::cout << "a" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}