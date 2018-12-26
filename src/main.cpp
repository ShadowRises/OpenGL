#include <GLFW/glfw3.h>
#include <iostream>

void processInputs(GLFWwindow *window, int key, int scancode, int action, int mods);

int main() {
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 640;

	GLFWwindow *window;

	if(!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello world !", NULL, NULL);
	if(!window) {
		std::cerr << "Failed to create the window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, processInputs);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.3f, 0.f, 0.5f, 1.f);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInputs(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}