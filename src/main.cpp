#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "callbacks.hpp"
#include "shader_utility.hpp"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 640;

int main(int argc, char* argv[]) {
	// Check if we passed shader to the program
	if (argc == 1) {
		std::cerr << "Usage : " << argv[0] << " SHADER_FILE" << std::endl;
		return -1;
	}
	
	// Initialization of GLFW
	if(!glfwInit()) {
		std::cerr << "Failed to init GLFW" << std::endl;
		return -1;
	}

	// Setting window context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the window and making the GL context current
	GLFWwindow *window;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
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


        // Build and compile the vertex shader program
	ShadersSources shaderSources = parseShader(argv[1]);
	unsigned int shaderProgram = createShader(shaderSources.VertexShader, shaderSources.FragmentShader);

	float vertices[] = {
			-0.5f, -0.5f, 0.f,
			 0.5f, -0.5f, 0.f,
		 	 0.f,   0.5f, 0.f
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glUseProgram(shaderProgram);

	// Main loop
	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
