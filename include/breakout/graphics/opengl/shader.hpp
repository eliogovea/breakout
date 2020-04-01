#pragma once

#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <string>
#include <vector>
#include <map>

namespace breakout::graphics {

class shader {
public:
  static std::string load(const std::string filepath);
  static GLuint compile(GLuint type, const std::string& code);
  static GLuint link(const std::vector<GLuint>& shaders);
  explicit shader(const std::string& vertex_shader, const std::string& fragment_shader);
  ~shader();
  GLuint program_id();
  GLuint locate_uniform(const std::string& name);
  void bind();
private:
  GLuint program_id_;
  std::map<std::string, GLuint> uniforms_;
};


std::string shader::load(const std::string file_path) {
  std::string result;
  std::ifstream input{file_path.data(), std::ios::in};
  if (!input.is_open()) {
    // should throw an exception ???
    std::cout << "could not load the shader " << file_path << std::endl;
  }
  std::string line;
  while (getline(input, line)) {
    result += line + "\n";
  }
  return result;
}

// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
GLuint shader::compile(GLuint type, const std::string& code) {
  GLuint id = glCreateShader(type);
  
  GLchar const* ptr = (const GLchar*)code.c_str();
  glShaderSource(id, 1, &ptr, 0);
  glCompileShader(id);

  GLint result = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    GLint msg_length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &msg_length);
    if (msg_length > 0) {
      std::vector<char> msg(msg_length);
      glGetShaderInfoLog(id, msg_length, nullptr, &msg[0]);
      std::cout << "error compiling shader: " << msg.data() << std::endl;
      glDeleteShader(id);
    }
  }
  return id;
}

GLuint shader::link(const std::vector<GLuint>& shaders) {
  GLuint id = glCreateProgram();
  for (auto x : shaders) {
    glAttachShader(id, x);
  }
  glLinkProgram(id);

  GLint result = 0;

  glGetProgramiv(id, GL_LINK_STATUS, &result);
  
  if (result == GL_FALSE) {
    GLint msg_length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &msg_length);
    if (msg_length > 0) {
      std::vector<char> msg(msg_length + 1);
      glGetProgramInfoLog(id, msg_length, nullptr, &msg[0]);
      std::cout << "error linking: " << msg.data() << std::endl;
    }
    glDeleteProgram(id);
    for (auto x : shaders) {
      glDeleteShader(x);
    }
    return id;
  }

  glValidateProgram(id);

  if (result == GL_FALSE) {
    GLint msg_length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &msg_length);
    if (msg_length > 0) {
      std::vector<char> msg(msg_length + 1);
      glGetProgramInfoLog(id, msg_length, nullptr, &msg[0]);
      std::cout << "error validating: " << msg.data() << std::endl;
    }
    glDeleteProgram(id);
    for (auto x : shaders) {
      glDeleteShader(x);
    }
    return id;
  }

  for (auto x : shaders) {
    glDetachShader(id, x);
  }
  for (auto x : shaders) {
    glDeleteShader(x);
  }
  return id;
}

shader::shader(const std::string& vertex_shader, const std::string& fragment_shader) {
  std::cout << "loading vetex shader: " <<  vertex_shader << " ..." << std::endl;
  auto code_vs = shader::load(vertex_shader);
  // std::cout << "vertex shader code:\n" << code_vs << std::endl;

  std::cout << "loading fragment shader: " << fragment_shader << " ..." << std::endl;
  auto code_fs = shader::load(fragment_shader);
  // std::cout << "fragment shader code:\n" << code_fs << std::endl;

  std::cout << "compiling vertex shader ..." << std::endl;
  auto id_vs = shader::compile(GL_VERTEX_SHADER, code_vs);
  std::cout << "vertex shader id: " << id_vs << std::endl;

  std::cout << "compiling fragment shader ..." << std::endl;
  auto id_fs = shader::compile(GL_FRAGMENT_SHADER, code_fs);
  std::cout << "fragment shader id: " << id_fs << std::endl;

  std::cout << "linking ..." << std::endl;
  program_id_ = shader::link({id_vs, id_fs});
  std::cout << "program id: " << program_id_ << std::endl;
}

shader::~shader() {
  std::cerr << "!!!!! DELETE SHADER, CHECK !!!!!!\n";
  glDeleteProgram(program_id_);
}

GLuint shader::program_id() {
  return program_id_;
}

GLuint shader::locate_uniform(const std::string& name) {
  if (uniforms_.find(name) != uniforms_.end()) {
    return uniforms_[name];
  }
  auto id = glGetUniformLocation(program_id_, name.c_str());
  if (id >= 0) { // good
    uniforms_[name] = id;
  }
  return id;
}

void shader::bind() {
		glUseProgram(program_id_);
}


} // namespace breakout::graphics