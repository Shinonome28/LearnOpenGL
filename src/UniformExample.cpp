

#include "config.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Shader.h"
#include "Exceptions.h"

// 原文并没有使用OpenGL类型，此处使用了OpenGL类型

// 该回调函数用于在窗口大小变化时调整视口大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// 通过GLFW的机制处理输入函数
void processInput(GLFWwindow *window);

// 01-04
int UniformExample()
{
	glfwInit();
	// 设定GLFW参数，包括OpenGL版本，使用的是3.3版本的核心模式
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 以下部分为创建窗口。这部分与操作系统调用或窗口管理器有关，与OpenGL无关
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 将窗口的上下文设置为当前的上下文
	// 往后所有的OpenGL调用都是针对当前的上下文的
	// 也就是渲染到GLFW窗口上
	glfwMakeContextCurrent(window);
	// 查找OpenGL函数地址，为调用OpenGL函数做准备
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 现在开始可以执行和OpenGL相关的操作了

// 修改顶点着色器让三角形上下颠倒
#ifdef _UPSIDE_DOWN
	// 顶点数组对象
	GLfloat vertices[] = {
		// 位置              // 颜色
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	 // 右下
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
		0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f	 // 顶部
	};
#else
	// 顶点数组对象
	GLfloat vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f	  // 顶部
	};
#endif
	// 创建并绑定顶点数组对象
	// 顶点数组对象用于管理顶点缓存对象
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// 注意一定要绑定VAO
	glBindVertexArray(VAO);

	// Vertex Buffer Object的句柄
	GLuint VBO;
	// 生成一个Buffer
	glGenBuffers(1, &VBO);
	// 注册这个Buffer到当前上下文
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 将数据传入Buffer。这将会把数据从内存复制到显存
	// 第四个参数可选值：
	// GL_STATIC_DRAW ：数据不会或几乎不会改变。
	// GL_DYNAMIC_DRAW：数据会被改变很多。
	// GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 解绑VAO，防止更改
	glBindVertexArray(0);

	LearnOpenGLUtils::Shader shader("example1.vs.glsl", "example1.fs.glsl", shaderPath.c_str());
	shader.use();
	// 使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧
	// shader.setFloat("offset", 0.2f);

	// 设置视口。这部分与视口变换有关。
	glViewport(0, 0, 800, 600);
	// 设置窗口大小变化时的回调函数，这个回调函数用于通知OpenGL调整视口大小。
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 交换缓冲区和消息循环
	// GLFW已经帮我们设置了双缓冲区
	while (!glfwWindowShouldClose(window))
	{
		// 简单处理输入
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 渲染指令
		shader.use();
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f;
		float redValue = cos(timeValue) / 2.0f;
		float blueValue = -sin(timeValue) / 2.0f;
		int vertexColorLocation = glGetUniformLocation(shader.getID(), "ourColor");
		glUniform4f(vertexColorLocation, redValue, greenValue, redValue, 1.0f);
		// glUniform4f(vertexColorLocation, 0, 0, 0, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// 释放所有资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	return UniformExample();
}