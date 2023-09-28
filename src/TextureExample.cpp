#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

#include "Shader.h"
#include "config.h"

// 该回调函数用于在窗口大小变化时调整视口大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// 通过GLFW的机制处理输入函数
void processInput(GLFWwindow *window);

static float mixValue = 0.0f;

int TextureExample() {
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

  // 加载并生成纹理
  stbi_set_flip_vertically_on_load(true);

  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width1, height1, nrChannels1;
  unsigned char *data1 = stbi_load((imagePath + "\\container.jpg").c_str(),
                                   &width1, &height1, &nrChannels1, 0);
  if (data1) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture1 " << imagePath + "\\container.png"
              << std::endl;
  }
  stbi_image_free(data1);

  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width2, height2, nrChannels2;
  unsigned char *data2 = stbi_load((imagePath + "\\awesomeface.png").c_str(),
                                   &width2, &height2, &nrChannels2, 0);
  if (data2) {
    // 注意这里的源文件格式是GL_RGBA，但是我们存储为GL_RGB，即丢弃一个通道
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture2 " << imagePath + "\\awesomeface.png"
              << std::endl;
  }
  stbi_image_free(data2);

  // 顶点数组对象
  GLfloat vertices[] = {
      //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
  };

  // 元素索引
  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // 颜色属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 纹理坐标属性
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // 解绑VAO，防止更改
  glBindVertexArray(0);

  LearnOpenGLUtils::Shader shader(
      "textureExample1.vs.glsl", "textureExample1.fs.glsl", shaderPath.c_str());
  // 设置纹理采样器uniform对应的纹理单元
  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

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

    shader.use();
    shader.setFloat("mixValue", mixValue);

    // 设置纹理单元对应的纹理对象
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // 释放所有资源
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    mixValue -= 0.001f;
    if (mixValue < 0.0f) mixValue = 0.0f;
  } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    mixValue += 0.001f;
    if (mixValue > 1.0f) mixValue = 1.0f;
  }
}

int main() { return TextureExample(); }