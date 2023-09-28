#pragma once

#include <exception>
#include <string>

namespace LearnOpenGLUtils {
class OpenGLExcption : public std::exception {
  std::string mMessage;
  std::string mLog;

 public:
  OpenGLExcption(const std::string &message, const std::string &log)
      : mMessage(message), mLog(log) {}
  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
  const char *log() const noexcept { return mLog.c_str(); }
};

class ShaderCompilationException : public OpenGLExcption {
 public:
  ShaderCompilationException(const std::string &message, const std::string &log)
      : OpenGLExcption(message, log) {}
};

class ShaderLinkingException : public OpenGLExcption {
 public:
  ShaderLinkingException(const std::string &message, const std::string &log)
      : OpenGLExcption(message, log) {}
};
}  // namespace LearnOpenGLUtils