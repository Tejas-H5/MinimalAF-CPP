#pragma once

#include "Datatypes.h"
#include "GLFW/glfw3.h"
#include "helpers.h"

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
		Window(int w, int h, const std::string& title);
		~Window();

		void run();
		void setState(WindowMode mode);
		void setSize(int w, int h);
		void setTitle(const std::string& title);

		SizeI getSize();
	};
}

