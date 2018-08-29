#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//settings
#define DEF_WINDOW_WIDTH 800
#define DEF_WINDOW_HEIGHT 600

#define ORIGIN

#ifndef ORIGIN

float stride = 0.001;

float m_x = 0.5;
float m_y = 0.5;
float m_z = 0.5;
float m_h = 0.5;

#endif//not define origin


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
#ifndef ORIGIN
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_x += stride;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_x -= stride;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_y += stride;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_y -= stride;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		m_z += stride;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		m_z -= stride;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		m_h += stride;
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		m_h -= stride;
#endif // not define origin

}



int main()
{
	//Initial glfw window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creat a window object
	GLFWwindow* window = glfwCreateWindow(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//将我们窗口的上下文设置为当前线程的主上下文了。


	//register a callback function to manipulate window's size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//Initial GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//-------------------------------------------
	//start setting shader and execute


	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");//使用着色器类简化

	float vertices[] = {
		//---- 位置 ----   ---- 颜色 ----   - 纹理坐标 -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	//OpenGL的核心模式要求我们使用VAO，所以它知道该如何处理我们的顶点输入。如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的。
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//源代码为什么这里解绑和失效？
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);




	unsigned int texture1, texture2;

	//第一张图
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//为当前绑定的纹理对象设置环绕过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
	int width, height, nrChannels;
	//使图片反转，加载图片数据之前使用！
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//第二张图
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//为当前绑定的纹理对象设置环绕过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//注意这里GL_RGBA和加载第一张图的区别，可能是因为png与jpg的区别
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	ourShader.use();
	// 别忘记在激活着色器前先设置uniform！
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
	ourShader.setInt("texture2", 1); // 或者使用着色器类设置


	while (!glfwWindowShouldClose(window))//check the exit request
	{

		processInput(window);//react to press keyboard

#ifndef ORIGIN
		glClearColor(m_x, m_y, m_z, m_h);
#else
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#endif
		glClear(GL_COLOR_BUFFER_BIT);


		// 在绑定纹理之前先激活纹理单元
		glActiveTexture(GL_TEXTURE0);
		// 绑定纹理
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// 渲染
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//Swap colour buffer(backgroundcolour)
		glfwPollEvents();//check if trigger anyevents

	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}