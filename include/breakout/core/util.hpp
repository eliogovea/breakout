#pragma once

#include <glm/glm.hpp>

namespace breakout::core::util {

struct circle {
  glm::vec2 center;
  float radius;
};

struct rectangle {
  glm::vec2 bottom_left;
  float width;
  float height;
};

inline bool inside(const circle& c, const glm::vec2 p) {
  float dx = p.x - c.center.x;
  float dy = p.y - c.center.y;
  return (dx * dx + dy * dy <= c.radius * c.radius);
}

inline bool collides(const circle& c, const rectangle& r) {
  if (c.center.x + c.radius < r.bottom_left.x) {
    return false;
  }
  if (r.bottom_left.x + r.width < c.center.x - c.radius) {
    return false;
  }
  if (c.center.y + c.radius < r.bottom_left.y) {
    return false;
  }
  if (r.bottom_left.y + r.height < c.center.y - c.radius) {
    return false;
  }
  // TODO properly handle collision !!!
  return r.bottom_left.x <= c.center.x && c.center.x <= r.bottom_left.x + r.width &&
          r.bottom_left.y <= c.center.y && c.center.y <= r.bottom_left.y + r.height; // quick test only
}

} // namespace breakout::core::util