#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "callbacks.hpp"
#include "shader.hpp"

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
	Shader shaders(argv[1]);

	// float vertices[] = {
	//       /*  x      y     z   color  */
	// 	-0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f,
	// 	 0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 
	// 	 0.f,   0.5f, 0.f, 0.f, 0.f, 1.f,

	// 	 1.f,  0.5f, 0.f, 1.f, 1.f, 1.f,
	// };

	float vertices[] = {
		-0.25f,  0.25f, 0.f, 1.f, 1.f, 1.f,
		 0.25f,  0.25f, 0.f, 1.f, 1.f, 1.f,
		 0.25f, -0.25f, 0.f, 1.f, 1.f, 1.f,
		-0.25f, -0.25f, 0.f, 1.f, 1.f, 1.f,

		-0.25f,  0.25f, 0.5f, 1.f, 1.f, 1.f,
		 0.25f,  0.25f, 0.5f, 1.f, 1.f, 1.f,
		 0.25f, -0.25f, 0.5f, 1.f, 1.f, 1.f,
		-0.25f, -0.25f, 0.5f, 1.f, 1.f, 1.f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		0, 4, 7,
		0, 7, 3,

		1, 5, 6,
		1, 6, 2,
	};

	
	unsigned int VBO, VAO, EBO; // VBO = Vertex Buffer Object
	                            // VAO = Vertex Array Object
	                            // EBO = Element Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the VAO to the current Vertex Array Object
	glBindVertexArray(VAO);

	// Bind the VBO to the current Vertex Buffer Object + fills it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Attribution of an index for each Vertex Shader inputs
	unsigned int posAttrib = glGetAttribLocation(shaders.get_program_id(), "position");
	unsigned int colorAttrib = glGetAttribLocation(shaders.get_program_id(), "color");

	// Attribution of the position data
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);

	// Attribution of the color data
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

	// Bind the EBO to the current Element Buffer Object + fills it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shaders.use();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Main loop
	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaders.get_program_id());

	glfwTerminate();
	return 0;
}
