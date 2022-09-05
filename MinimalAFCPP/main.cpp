#include "helpers.h"
#include<iostream>

#include "MinimalAF.h"

class App : public af::Window {
public:
    inline App() : af::Window(1280, 720, "hi there bro") {}
    ~App() {
    }

    inline void initialize() override {
        setDrawColor(af::rgb(1, 0, 0));

        setModelMatrix(glm::identity<mat4>());
        setViewMatrix(glm::identity<mat4>());
        setProjectionMatrix(glm::identity<mat4>());
    }

    inline void render() override {
        float z = 0;
        print("about to render da triangle : ...");
        drawTriangle(
            af::Vertex(vec3(-0.5f, -0.5f, z), vec2(0, 0)),
            af::Vertex(vec3(0.5f, -0.5f, z), vec2(1, 0)),
            af::Vertex(vec3(0, 0.5f, z), vec2(0.5f, 1))
            // af::Vertex(vec3(-50.0f, -50.0f, z), vec2(0, 0)),
            // af::Vertex(vec3(50.0f, -50.0f, z), vec2(1, 0)),
            // af::Vertex(vec3(0, 0.5f, z), vec2(0.5f, 1))
        );
    }

    inline void update() override {}
};


int main(void) {
    if (af::init() != 0) {
        std::cin.get();
        return -1;
    }

    App *a = new App();

    a->setTitle("bro, innit");

    print("Started");

    a->run();

    delete a;

    af::uninit();

    return 0;
}