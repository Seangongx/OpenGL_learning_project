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

//build a fragmentShader Sourcecode
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

//settings
#define DEF_WINDOW_WIDTH 800
#define DEF_WINDOW_HEIGHT 600

//#define ORIGIN

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

	//####Fragment shader object####
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	/*unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);为什么不能这样写？*/
	//attach shader source code to shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//check success
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "EERRO::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//shader program object
	unsigned int shaderProgram = glCreateProgram();
	/*unsigned int shaderProgram;
	shaderProgram = glCreateProgram();为什么不能这样写？*/

	//link the shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "EERRO::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Delete remnent
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = {
		-0.5f, -0.5f, -0.0f,//left
		0.5f, -0.5f, -0.0f,//right 注意这里x坐标是0.5不是-0.5,否则三角形无法显示！
		0.0f, 0.5f, 0.0f//top
	};


	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//注意注释不要嵌套，否则容易出编译问题
	/*这样写就麻烦了
	Vertex Buffer Object
	unsigned int VBO;
	glGenBuffer(1, &VBO);
	Vertex Array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	*/


	//0. 复制顶点数组到缓冲中供OpenGL使用
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	//Bind object to Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to mem
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// 1. 设置顶点属性指针
	//analyze data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的。
	glEnableVertexAttribArray(0);
	
	//源代码为什么在这里解绑和失效？
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/*
	Viewport settings为什么这里不需要设置视口？因为在窗口回调中设置了
	glViewport(0, 0, DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT);*/

	while (!glfwWindowShouldClose(window))//check the exit request
	{

		processInput(window);//react to press keyboard

		// 2. 渲染物体...
#ifndef ORIGIN
		glClearColor(m_x, m_y, m_z, m_h);
#else
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#endif
		glClear(GL_COLOR_BUFFER_BIT);

		// 3. 当我们渲染一个物体时要使用着色器程序
		//activate program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// 4. 绘制物体
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//Swap colour buffer(backgroundcolour)
		glfwPollEvents();//check if trigger anyevents

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}