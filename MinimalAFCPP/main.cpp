#include "helpers.h"
#include "Window.h"

int main(void) {
    if (!glfwInit()) {
        print("couldn't init glfw");
        return -1;
    }

    Window w = Window();

    std::cin.get();

    return 0;
}