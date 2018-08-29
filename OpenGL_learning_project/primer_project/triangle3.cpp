#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>

//build a VertexShader SourceCode 
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//build a fragmentShader Sourcecode1
const char *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

//build a fragmentShader Sourcecode2
const char *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

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


	//####Vertex shader object####
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	/*unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);为什么不能这样写？*/

	//attach shader source code to shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//check success
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "EERRO::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//####Fragment shader object####11111111111111111111
	unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	/*unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);为什么不能这样写？*/
	//attach shader source code to shader object
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader1);
	//check success
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
		std::cout << "EERRO::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//####Fragment shader object####22222222222222222222
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	/*unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);为什么不能这样写？*/
	//attach shader source code to shader object
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);
	//check success
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "EERRO::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//shader program object
	unsigned int shaderProgram1 = glCreateProgram();
	unsigned int shaderProgram2 = glCreateProgram();
	/*unsigned int shaderProgram;
	shaderProgram = glCreateProgram();为什么不能这样写？*/

	//link the shaders1
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);

	//link the shaders2
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	//check1
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "EERRO::PROGRAM1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//check2
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "EERRO::PROGRAM2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Delete remnent
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);


	float vertices1[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float vertices2[] = {
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	unsigned int VBO1, VAO1, VBO2, VAO2;//其实可以用数组！
	//OpenGL的核心模式要求我们使用VAO，所以它知道该如何处理我们的顶点输入。如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);


	//first triangle
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//second triangle
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的。
	glEnableVertexAttribArray(0);


	//源代码为什么这里解绑和失效？
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	/*
	Viewport settings为什么这里不需要设置视口？因为在窗口回调中设置了
	glViewport(0, 0, DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT);*/

	while (!glfwWindowShouldClose(window))//check the exit request
	{

		processInput(window);//react to press keyboard

#ifndef ORIGIN
		glClearColor(m_x, m_y, m_z, m_h);
#else
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#endif
		glClear(GL_COLOR_BUFFER_BIT);

		//activate program
		glUseProgram(shaderProgram1);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);//如果是6怎么办？


		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);//Swap colour buffer(backgroundcolour)
		glfwPollEvents();//check if trigger anyevents

	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO1);
	glfwTerminate();

	return 0;
}