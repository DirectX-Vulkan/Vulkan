#pragma once

#include "Resources.h"
#include "Graphics.h"

class Window {
	public:
		Window(int width, int height, string name);
		void CleanUp();

		GLFWwindow* GetWindow();

	private:
		GLFWwindow* m_window;
		static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
};