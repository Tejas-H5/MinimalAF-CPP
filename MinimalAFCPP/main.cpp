#include "helpers.h"
#include<iostream>
#include "MinimalAF.h"
#include <chrono>
#include <thread>

class App : public af::Window {
	af::Texture tex;
	af::Font font;
	af::Font framerateFont;
	// af::Shader* shader;

	af::FpsTimer renderFpsTimer;
	float updateFps;
	af::FpsTimer updateFpsTimer;

	af::Shader shader;

	char framerateStrBuffer[64];
	float a;
public:
	inline App() : af::Window(1280, 720, "hi there bro") {
		a = 0;
		// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		glfwSwapInterval(1);

		tex.load("res/textures/heavy_metal.jpg", af::TextureImportSettings(
			af::FilteringType::NearestNeighbour,
			af::ClampingType::Repeat
		));
		font.load("Source Code Pro", 55);
		framerateFont.load("Source Code Pro", 22);
	}

	inline void render(double dt) {
		clear();
		cartesian2D(1, 1);

		setDrawColor(af::rgb(1, 1, 1));

		float fps = renderFpsTimer.tick(1000);
		int len = sprintf(framerateStrBuffer, "r: %F, u: %F", fps, updateFps);

		setFont(&framerateFont);
		drawText(framerateStrBuffer, len, 0, getHeight(), af::HAlign::Left, af::VAlign::Top);


		float size = 200;

		setModel(translation(glm::vec3(getWidth() / 2, getHeight() / 2, 0)) * rotation(glm::quat(glm::vec3(0, 0, a))));
		a += dt * 2;
		drawRect(-size, -size, +size, +size);


		// setTexture(&tex);
		// drawRect(100, 10, getWidth() - 1, getHeight() - 1);

		swapBuffers();
	}

	inline void update() {
		updateFps = updateFpsTimer.tick(1000);
	}
};

// I couldn't find another way to make this reuseable :(
// Looks a bit hacky but it works. C++ templates are actually based
template <class T> void powerSavingLoop(T& program) {
	print("Started");

	//// trivial loop
	//while (!shouldClose()) {
	//    pollEvents();
	//    update();
	//    render();
	//}

	double renderInterval = 1.0 / 60.0;
	double updateInterval = 1.0 / 120.0;
	double lastRenderTime = glfwGetTime();
	double lastUpdateTime = glfwGetTime();
	bool shouldPowersave = false;
	while (!program.shouldClose()) {
		double t = glfwGetTime();
		double renderDt = t - lastRenderTime;
		double updateDt = t - lastUpdateTime;

		if (updateDt >= updateInterval) {
			lastUpdateTime = t;

			program.pollEvents();
			program.update();
		}

		if (renderDt >= renderInterval) {
			lastRenderTime = t;

			program.render(renderDt);
		}

		if (shouldPowersave) {
			// This bit is an attempt to save power.
			// If the frame rendered really really fast, we just sleep the remainder of the time.
			// Before, this loop would make my PC fan spin like crazy, and use 18% cpu rendering nothing but glClear. 
			// Now, it only takes up 3-4% cpu
			double timeTakenToDoThisCalculationProbably = 0.005;
			double nextUpdateTime = lastUpdateTime + updateInterval;
			double nextRenderTime = lastRenderTime + renderInterval;
			double nextLoopTime = min(nextUpdateTime, nextRenderTime);
			double remainingTime = nextLoopTime - glfwGetTime() - timeTakenToDoThisCalculationProbably;
			if (remainingTime > 0) {
				std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<long long>(remainingTime * 1000000000.0)));
			}
		}
	}

	print("ended");
}

int main(int argc, char* argv[]) {
	if (af::init() != 0) {
		return -1;
	}

	powerSavingLoop(App());

	af::uninit();
	return 0;
}