#include"Window.h"

using namespace af;

Window::Window(int w, int h, const std::string& title) {
    window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
    if (!window) {
        print("window wasn't created");
        return;
    }
}

Window::~Window() {
    glfwDestroyWindow(window);
    window = nullptr;
}

void af::Window::run() {
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        print("glewInit() != GLEW_OK");
        return;
    }

    initialize();

    // this is an update/render loop.
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: make render and update happen at different intervals
        // TODO: also give them the deltatime somehow. static class or pass it in as an arg here
        update();
        render();

        glfwSwapBuffers(window);
    }
}

void af::Window::setState(WindowMode mode) {
    if (mode == WindowMode::MAXIMIZED) {
        glfwMaximizeWindow(window);
    }
    else if (mode == WindowMode::FULLSCREEN) {
        print("TODO: implement window fullscreening");
    }
    else if (mode == WindowMode::MINIMIZED) {
        glfwIconifyWindow(window);
    }
    else if (mode == WindowMode::WINDOWED) {
        glfwRestoreWindow(window);
    }
}

void af::Window::setSize(int w, int h) {
    glfwSetWindowSize(window, w, h);
}

void af::Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(window, title.c_str());
}

SizeI af::Window::getSize() {
    SizeI windowSize;
    glfwGetWindowSize(window, &windowSize.width, &windowSize.height);

    return windowSize;
}

int af::init() {
    if (!glfwInit()) {
        print("couldn't init glfw");
        return -1;
    }


    return 0;
}

void af::uninit() {
    print("glfwTerminate();");
    glfwTerminate();
}
