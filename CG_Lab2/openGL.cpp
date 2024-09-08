#include <iostream>

#include <GL/glew.h>
#include <gl/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "camera.h"
#include <iostream>
#include <soil.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define M_PI 3.1415

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 1.4f;
float lastY = SCR_HEIGHT / 1.4f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;


glm::vec3 lightPos(2.2f, 2.0f, 0.4f);
int main()
{
	
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// glfw: создание окна
	GLFWwindow* window = glfwCreateWindow(1100, 800, "Window", nullptr, 0);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("basic_lighting.vs", "basic_lighting.fs");
	Shader lampShader("lamp.vs", "lamp.fs");


	float secondObjectVertices[] = {
		
		-1.4f, -1.25f, -1.4f,   1.0f, 1.0f,  0.0f,
		 1.4f, -1.25f, -1.4f,   0.0f, 1.0f,  0.0f,
		-1.4f, -1.25f,  1.4f,   1.0f, 0.0f,  0.0f,
		-1.4f, -1.25f,  1.4f,   1.0f, 1.0f,  0.0f,
		 1.4f, -1.25f,  1.4f,   1.0f, 0.0f,  0.0f,
		 1.4f, -1.25f, -1.4f,   0.0f, 0.0f,  0.0f,
		 
		 -1.4f, -1.75f, -1.4f,  1.0f, 1.0f,  0.0f,
		  1.4f, -1.75f, -1.4f,  0.0f, 1.0f,  0.0f,
		 -1.4f, -1.75f,  1.4f,  1.0f, 0.0f,  0.0f,
		 -1.4f, -1.75f,  1.4f,  1.0f, 1.0f,  0.0f,
		  1.4f, -1.75f,  1.4f,  1.0f, 0.0f,  0.0f,
		  1.4f, -1.75f, -1.4f,  0.0f, 0.0f,  0.0f,

		 
		-1.4f, -1.25f,  1.4f,  1.0f, 1.0f,  0.0f,
		 1.4f, -1.25f,  1.4f,  0.0f, 1.0f,  0.0f,
		-1.4f, -1.75f,  1.4f,  1.0f, 0.0f,  0.0f,
		-1.4f, -1.75f,  1.4f,  1.0f, 1.0f,  0.0f,
		 1.4f, -1.75f,  1.4f,  1.0f, 0.0f,  0.0f,
		 1.4f, -1.25f,  1.4f,  0.0f, 0.0f,  0.0f,		 
		 
		-1.4f, -1.25f,  -1.4f, 1.0f, 1.0f,  0.0f,
		 1.4f, -1.25f,  -1.4f, 0.0f, 1.0f,  0.0f,
		-1.4f, -1.75f,  -1.4f, 1.0f, 0.0f,  0.0f,
		-1.4f, -1.75f,  -1.4f, 1.0f, 1.0f,  0.0f,
		 1.4f, -1.75f,  -1.4f, 1.0f, 0.0f,  0.0f,
		 1.4f, -1.25f,  -1.4f, 0.0f, 0.0f,  0.0f,
		 
		 1.4f, -1.25f,  1.4f,   1.0f, 1.0f,  0.0f,
		 1.4f, -1.25f, -1.4f,   0.0f, 1.0f,  0.0f,
		 1.4f, -1.75f,  1.4f,   1.0f, 0.0f,  0.0f,
		 1.4f, -1.75f,  1.4f,   1.0f, 1.0f,  0.0f,
		 1.4f, -1.75f, -1.4f,   1.0f, 0.0f,  0.0f,
		 1.4f, -1.25f, -1.4f,   0.0f, 0.0f,  0.0f,
		 
		 -1.4f, -1.25f,  1.4f,  1.0f, 1.0f,  0.0f,
		 -1.4f, -1.25f, -1.4f,  0.0f, 1.0f,  0.0f,
		 -1.4f, -1.75f,  1.4f,  1.0f, 0.0f,  0.0f,
		 -1.4f, -1.75f,  1.4f,  1.0f, 1.0f,  0.0f,
		 -1.4f, -1.75f, -1.4f,  1.0f, 0.0f,  0.0f,
		 -1.4f, -1.25f, -1.4f,  0.0f, 0.0f,  0.0f,
	};
	float vertices[] = {
		
		-0.5f, 0.5f, -0.5f,   0.5f, 1.5f,  0.5f,
		 0.5f, 0.5f, -0.5f,   0.5f, 1.5f,  0.5f,
		 0.5f, 0.5f,  0.5f,   0.5f, 1.5f,  0.5f,
		-0.5f, 0.5f, -0.5f,   0.5f, 1.5f,  0.5f,
		 0.5f, 0.5f,  0.5f,   0.5f, 1.5f,  0.5f,
		-0.5f, 0.5f,  0.5f,   0.5f, 1.5f,  0.5f,
		
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  -1.7071f,
		 0.5f, 0.5f, -0.5f,  0.0f, 1.0f,  -1.7071f,
		 0.0f, 1.5f,  0.0f,  1.0f, 0.0f,  -1.7071f,
		-0.5f, 0.5f,  0.5f,  1.0f, 1.0f,   1.7071f,
		 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,   1.7071f,
		 0.0f, 1.5f,  0.0f,  0.0f, 0.0f,   1.7071f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  -1.7071f,
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f,   1.7071f,
		 0.0f, 1.5f,  0.0f,  1.0f, 0.0f,  -1.7071f,
		 0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  -1.7071f,
		 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,   1.7071f,
		 0.0f, 1.5f,  0.0f,  0.0f, 0.0f,   1.7071f,
		 
		-0.5f, 0.5f, -0.5f,   0.5f, -1.5f,  0.5f,
		 0.5f, 0.5f, -0.5f,   0.5f, -1.5f,  0.5f,
		 0.5f, 0.5f,  0.5f,   0.5f, -1.5f,  0.5f,
		-0.5f, 0.5f, -0.5f,   0.5f, -1.5f,  0.5f,
		 0.5f, 0.5f,  0.5f,   0.5f, -1.5f,  0.5f,
		-0.5f, 0.5f,  0.5f,   0.5f, -1.5f,  0.5f,
		
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  -1.7071f,
		 0.5f, 0.5f, -0.5f,  0.0f, 1.0f,  -1.7071f,
		 0.0f,-0.5f,  0.0f,  1.0f, 0.0f,  -1.7071f,
		-0.5f, 0.5f,  0.5f,  1.0f, 1.0f,   1.7071f,
		 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,   1.7071f,
		 0.0f,-0.5f,  0.0f,  0.0f, 0.0f,   1.7071f,
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  -1.7071f,
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f,   1.7071f,
		 0.0f,-0.5f,  0.0f,  1.0f, 0.0f,   1.7071f,
		 0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  -1.7071f,
		 0.5f, 0.5f,  0.5f,  1.0f, 0.0f,   1.7071f,
		 0.0f, -0.5f,  0.0f, 0.0f, 0.0f,   1.7071f,
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	unsigned char* data = SOIL_load_image("Redtexture1.png", &width, &height, &nChannels, SOIL_LOAD_RGB);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "texture error\n";
	}
	SOIL_free_image_data(data);


	unsigned int secondObjectVBO, secondObjectVAO;
	glGenVertexArrays(1, &secondObjectVAO);
	glGenBuffers(1, &secondObjectVBO);

	glBindBuffer(GL_ARRAY_BUFFER, secondObjectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondObjectVertices), secondObjectVertices, GL_STATIC_DRAW);

	glBindVertexArray(secondObjectVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int secondObjectTexture;
	glGenTextures(1, &secondObjectTexture);
	glBindTexture(GL_TEXTURE_2D, secondObjectTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* secondObjectData = SOIL_load_image("rainbow_texture.png", &width, &height, &nChannels, SOIL_LOAD_RGB);
	if (secondObjectData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, secondObjectData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Second object texture error\n";
	}
	SOIL_free_image_data(secondObjectData);


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//-------------------------------------------------------------------------------
		//--------------------------------------------1----------------------------------
		glBindTexture(GL_TEXTURE_2D, texture);

		lightingShader.use();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		lightingShader.setVec3("lightColor", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setInt("texture_diffuse1", 0);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom+50), 0.4f / 0.3f, 1.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(70.0f), glm::vec3(0.1f, -0.8f, -0.1f));
		lightingShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// lamp
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); 
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//-------------------------------------------------------------------------------
		//--------------------------------------------2----------------------------------
		lightingShader.use();
		lightingShader.setVec3("lightColor", 0.3f, 0.1f, 0.3f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setInt("texture_diffuse1", 0);

		glBindTexture(GL_TEXTURE_2D, secondObjectTexture);
		lightingShader.use();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		glm::mat4 secondObjectModel = glm::mat4(1.0f);
		secondObjectModel = glm::rotate(secondObjectModel, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		lightingShader.setMat4("model", secondObjectModel);

		glBindVertexArray(secondObjectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 58);

		// lamp
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		secondObjectModel = glm::mat4(1.0f);
		secondObjectModel = glm::translate(secondObjectModel, lightPos);
		secondObjectModel = glm::scale(secondObjectModel, glm::vec3(0.2f));
		lampShader.setMat4("model", secondObjectModel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 58);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
