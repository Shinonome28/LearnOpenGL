#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "Exceptions.h"

namespace LearnOpenGLUtils {
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

class Shader {
  // 程序ID
  GLuint ID;

 public:
  // 构造器读取并构建着色器
  Shader(const char *vertexPath, const char *fragmentPath,
         const char *prefix = nullptr);
  ~Shader();
  // 使用/激活程序
  void use() const { glUseProgram(ID); }
  // uniform工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, const glm::mat4 &mat);
  void setVec3(const std::string &name, float v1, float v2, float v3);

  GLuint getID() const { return ID; }
};

class ShaderGuard {
  Shader mShader;
  GLuint mOldShaderID;

 public:
  ShaderGuard(Shader shader) : mShader(shader) {
    glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint *>(&mOldShaderID));
    mShader.use();
  }

  ~ShaderGuard() { glUseProgram(mOldShaderID); }
};

Shader::Shader(const char *vertexShaderSourcePath_,
               const char *fragmentShaderSourcePath_, const char *prefix) {
  std::filesystem::path vertexShaderSourcePath(vertexShaderSourcePath_);
  std::filesystem::path fragmentShaderSourcePath(fragmentShaderSourcePath_);
  if (prefix != nullptr) {
    vertexShaderSourcePath =
        std::filesystem::path(prefix).append(vertexShaderSourcePath_);
    fragmentShaderSourcePath =
        std::filesystem::path(prefix).append(fragmentShaderSourcePath_);
  }

  std::string vertexShaderCode;
  std::string fragmentShaderCode;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;
  // 保证ifstream对象可以抛出异常：
  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  vertexShaderFile.open(vertexShaderSourcePath);
  fragmentShaderFile.open(fragmentShaderSourcePath);
  std::stringstream vertexShaderCodeStream, fragmentShaderCodeStream;
  vertexShaderCodeStream << vertexShaderFile.rdbuf();
  fragmentShaderCodeStream << fragmentShaderFile.rdbuf();
  vertexShaderFile.close();
  fragmentShaderFile.close();
  vertexShaderCode = vertexShaderCodeStream.str();
  fragmentShaderCode = fragmentShaderCodeStream.str();

  // compile and linkage
  GLuint vertexShader, fragmentShader;
  const char *vertexShaderSource = vertexShaderCode.c_str();
  const char *fragmentShaderSource = fragmentShaderCode.c_str();
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  dealWithShaderCompilationError(vertexShader);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  dealWithShaderCompilationError(fragmentShader);

  GLuint program;
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  dealWithShaderLinkingError(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  ID = program;
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
  int modelLoc = glGetUniformLocation(getID(), name.c_str());
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mat));
}

inline void Shader::setVec3(const std::string &name, float v1, float v2,
                            float v3) {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

}  // namespace LearnOpenGLUtils