#pragma once

#include <glm/glm.hpp>

namespace breakout::core {


namespace util {

bool inside_circle(const glm::vec2 P, const glm::vec2 C, const glm::float32 r) {
  return glm::distance(P, C) <= r;
}

bool inside_rectangle(const glm::vec2 P, const glm::vec2 C, const glm::float32 w, const glm::float32 h) {
  return C.x - 0.5 * w <= P.x && P.x <= C.x + 0.5 * w && C.y - 0.5 * h <= P.y && P.y <= C.y + 0.5 * h;
}

struct circle {
  glm::vec2 center;
  glm::float32 radius;
};

struct rectangle {
  glm::vec2 center;
  glm::float32 width;
  glm::float32 height;
};

inline static bool collide(const circle& ball_, const rectangle& brick_) {
  if (ball_.center.x + ball_.radius < brick_.center.x - 0.5 * brick_.width) {
    return false;
  }
  if (ball_.center.x - ball_.radius > brick_.center.x + 0.5 * brick_.width) {
    return false;
  }
  if (ball_.center.y + ball_.radius < brick_.center.y - 0.5 * brick_.height) {
    return false;
  }
  if (ball_.center.y - ball_.radius > brick_.center.y + 0.5 * brick_.height) {
    return false;
  }

  if (inside_circle(brick_.center + glm::vec2{0.5 * brick_.width, 0.5 * brick_.height}, ball_.center, ball_.radius)) {
    return true;
  }
  if (inside_circle(brick_.center + glm::vec2{-0.5 * brick_.width, -0.5 * brick_.height}, ball_.center, ball_.radius)) {
    return true;
  }
  if (inside_circle(brick_.center + glm::vec2{-0.5 * brick_.width, +0.5 * brick_.height}, ball_.center, ball_.radius)) {
    return true;
  }
  if (inside_circle(brick_.center + glm::vec2{0.5 * brick_.width, -0.5 * brick_.height}, ball_.center, ball_.radius)) {
    return true;
  }
  if (inside_rectangle(ball_.center + glm::vec2{0.0, ball_.radius}, brick_.center, brick_.width, brick_.height)) {
    return true;
  }
  if (inside_rectangle(ball_.center + glm::vec2{ball_.radius, 0.0}, brick_.center, brick_.width, brick_.height)) {
    return true;
  }
  if (inside_rectangle(ball_.center + glm::vec2{0.0, -ball_.radius}, brick_.center, brick_.width, brick_.height)) {
    return true;
  }
  if (inside_rectangle(ball_.center + glm::vec2{-ball_.radius, 0.0}, brick_.center, brick_.width, brick_.height)) {
    return true;
  }
  return false;
}

} // namespace util

} // namespace breakout::core::util