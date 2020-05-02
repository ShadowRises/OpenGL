#include <iostream>
#include <sstream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb/stb_image.h"
#include "callbacks.hpp"
#include "shader.hpp"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 640;

void usage(const char *command, bool error = false)
{
	std::stringstream message;
	message << "Usage : " << command << " SHADER_FILE [TEXTURE_FILES]"
		<< "\n";
	if (error)
		std::cerr << message.str();
	else
		std::cout << message.str();
}

int main(int argc, char *argv[])
{
	// Check if we passed shader to the program
	if (argc == 1)
	{
		usage(argv[0], true);
		return 1;
	}
	else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
	{
		usage(argv[0]);
		return 0;
	}

	// Initialization of GLFW
	if (!glfwInit())
	{
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
	if (!window)
	{
		std::cerr << "Failed to create the window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialization of GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Setting the viewport and callbacks
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, processInputs);

	// Get the texture if passed to the program
	int width, height, nb_channels;
	unsigned char *texture_data = nullptr;
	unsigned int textures[2];
	bool use_texture = false;

	if (argc >= 3)
	{
		stbi_set_flip_vertically_on_load(true);

		texture_data = stbi_load(argv[2], &width, &height, &nb_channels, 0);

		if (texture_data)
		{
			glGenTextures(2, textures);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (strstr(argv[2], ".jpg") != nullptr) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(texture_data);
			texture_data = nullptr;
			use_texture = true;
		}
		else
		{
			std::cerr << "Error while loading texture \"" << argv[2] << "\""
				  << "\n";
			return 1;
		}

		if (argc == 4)
		{
			texture_data = stbi_load(argv[3], &width, &height, &nb_channels, 0);

			if (texture_data)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, textures[1]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (strstr(argv[3], ".jpg") != nullptr) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
				glGenerateMipmap(GL_TEXTURE_2D);

				stbi_image_free(texture_data);
				texture_data = nullptr;
			}
			else
			{
				std::cerr << "Error while loading texture \"" << argv[3] << "\""
					  << "\n";
				return 1;
			}
		}
	}

	// Build and compile the vertex shader program
	Shader shaders(argv[1]);

	float vertices[] = {
		/*   x      y     z           color         texture coords  */
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // bottom right
	};

	unsigned int indices[] = {
		0,
		1,
		2,
		0,
		2,
		3,
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
	unsigned int textureAttrib = glGetAttribLocation(shaders.get_program_id(), "texture_coord");

	// Attribution of the position data
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

	// Attribution of the color data
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

	// Attribution of the texture data
	glEnableVertexAttribArray(textureAttrib);
	glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	// glBindTexture(GL_TEXTURE_2D, texture);

	// Bind the EBO to the current Element Buffer Object + fills it
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shaders.use();

	// Setting the textures
	if (use_texture)
	{
		shaders.set_int("texture_data1", 0);
		shaders.set_int("texture_data2", 1);
	}

	bool is_transform = false;
	if (strstr(argv[1], "transform") != nullptr)
		is_transform = true;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (is_transform)
		{
			glm::mat4 transform = glm::mat4(1.f);
			transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.f, 0.f, 1.f));
			transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.f));

			shaders.set_mat4("u_transform", transform);
		}

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, sizeof(vertices) / sizeof(float), GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shaders.get_program_id());

	glfwTerminate();
	return 0;
}
