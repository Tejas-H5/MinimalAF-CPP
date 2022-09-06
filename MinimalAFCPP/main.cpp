#include "helpers.h"
#include<iostream>

#include "MinimalAF.h"

class App : public af::Window {

    af::Texture* tex;
public:
    inline App() : af::Window(1280, 720, "hi there bro") {}

    ~App() {
        delete tex;
    }

    inline void initialize() override {
        tex = new af::Texture("res\\textures\\heavy_metal.jpg", af::TextureImportSettings(
            af::FilteringType::NearestNeighbour,
            af::ClampingType::Repeat
        ));

        setDrawColor(af::rgb(1, 1, 1));

        setModelMatrix(glm::identity<mat4>());
        setViewMatrix(glm::identity<mat4>());
        setProjectionMatrix(glm::identity<mat4>());
    }

    inline void render() override {
        float z = 0;
        setTexture(tex);
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


int main(int argc, char* argv[]) {
    std::cout << "cwd: " << argv[0] << std::endl;

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