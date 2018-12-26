#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "callbacks.h"

int main(int argc, char* argv[]) {
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 640;

	// Initialization of GLFW
	if(!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		return -1;
	}

	// Setting window context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the window and making the GL context current
	GLFWwindow *window;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello world !", NULL, NULL);
	if(!window) {
		std::cerr << "Failed to create the window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialization of GLAD
	if(!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress) ) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Setting the viewport and callbacks
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, processInputs);

	while(!glfwWindowShouldClose(window)) {

		glClearColor(0.3f, 0.f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}