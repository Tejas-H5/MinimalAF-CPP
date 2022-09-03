#include "helpers.h"
#include "Window.h"

#include<iostream>

using namespace af;

int main(void) {
    if (af::init() != 0) {
        print("couldn not init af :(");
        return -1;
    }

    af::Window w = af::Window(1280, 720, "bro");
    w.setState(af::WindowMode::MAXIMIZED);
    w.run();

    af::uninit();

    return 0;
}