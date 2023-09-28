#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <string>

#include "Exceptions.h"

// 原文并没有使用OpenGL类型，此处使用了OpenGL类型

// 该回调函数用于在窗口大小变化时调整视口大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// 通过GLFW的机制处理输入函数
void processInput(GLFWwindow *window);

void dealWithShaderCompilationError(GLuint shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << infoLog << std::endl;
    throw LearnOpenGLUtils::ShaderCompilationException(
        "ERROR::SHADER::VERTEX::COMPILATION_FAILED", infoLog);
  }
}

void dealWithShaderLinkingError(GLuint shaderProgram) {
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << infoLog << std::endl;
    throw LearnOpenGLUtils::ShaderLinkingException(
        "ERROR::SHADER::PROGRAM::LINKING_FAILED", infoLog);
  }
}

// 01-04
int SimpleTriangle() {
  glfwInit();
  // 设定GLFW参数，包括OpenGL版本，使用的是3.3版本的核心模式
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // 以下部分为创建窗口。这部分与操作系统调用或窗口管理器有关，与OpenGL无关
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // 将窗口的上下文设置为当前的上下文
  // 往后所有的OpenGL调用都是针对当前的上下文的
  // 也就是渲染到GLFW窗口上
  glfwMakeContextCurrent(window);
  // 查找OpenGL函数地址，为调用OpenGL函数做准备
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 现在开始可以执行和OpenGL相关的操作了

  // 顶点数组对象
  GLfloat vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};

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
  // 设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  // 启用顶点属性（参数为绑定为位置值）
  glEnableVertexAttribArray(0);
  // 解绑VAO，防止更改
  glBindVertexArray(0);

  // 顶点着色器源码
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
  // 现在我们要编译这个着色器源码
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // 指定shader源代码
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  dealWithShaderCompilationError(vertexShader);

  // 编译顶点着色器
  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  dealWithShaderCompilationError(fragmentShader);

  // 把两个着色器链接到一个着色器程序中（用以定义一个完整的图形管线）
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  // 加载编译好的着色器
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  dealWithShaderLinkingError(shaderProgram);
  // 删除着色器对象
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // 设置视口。这部分与视口变换有关。
  glViewport(0, 0, 800, 600);
  // 设置窗口大小变化时的回调函数，这个回调函数用于通知OpenGL调整视口大小。
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // 交换缓冲区和消息循环
  // GLFW已经帮我们设置了双缓冲区
  while (!glfwWindowShouldClose(window)) {
    // 简单处理输入
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 渲染指令
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // 释放所有资源
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() { return SimpleTriangle(); }