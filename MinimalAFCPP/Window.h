#pragma once

#include "Datatypes.h"
#include "helpers.h"

#include"GL/glew.h"
#include "GLFW/glfw3.h"

#include<string>

namespace af {
	int init();
	void uninit();

	enum class WindowMode {
		WINDOWED,
		FULLSCREEN,
		MAXIMIZED,
		MINIMIZED
	};

	class Window {
	private:
		GLFWwindow* window;

	public:
		virtual void initialize() = 0;
		virtual void update() = 0;
		virtual void render() = 0;

		Window(int w, int h, const std::string& title);
		virtual ~Window();

		void run();
		void setState(WindowMode mode);
		void setSize(int w, int h);
		void setTitle(const std::string& title);

		SizeI getSize();
	};
}

