#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace breakout::graphics {

class window {
  public:
    window(int width, int height, const std::string& title);
    virtual ~window();
    void clear();
    bool is_closed() const noexcept;
    void update();
    GLFWwindow* get_window();
  private:
    GLFWwindow* window_;
    bool is_closed_{false};
};

} // namespace breakout::graphics