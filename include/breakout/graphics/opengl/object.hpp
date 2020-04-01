#pragma once

#include <iostream>

#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <vendor/tiny_obj_loader.h>

namespace breakout::graphics {

struct vertex {
  glm::vec3 position;
  glm::vec3 color{};
  glm::vec2 texture_uv{};

  vertex(glm::vec3 position, glm::vec3 color, glm::vec2 texture_uv)
    : position(position), color(color), texture_uv(texture_uv) {}
};

class object {
public:
  explicit object(const std::vector<vertex>& vertices);
  ~object();
  void render();
private:
  std::vector<vertex> vertices_;
  GLuint va_, vb_;
};

object::object(const std::vector<vertex>& vertices) : vertices_(vertices) {
  glGenVertexArrays(1, &va_);
  glGenBuffers(1, &vb_);

  glBindVertexArray(va_);
    glBindBuffer(GL_ARRAY_BUFFER, vb_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertex), &vertices_[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0); // position
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (vertex), (GLvoid*)(3 * sizeof (GLfloat)));
    glEnableVertexAttribArray(1); // color
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (vertex), (GLvoid*)(6 * sizeof (GLfloat)));
    glEnableVertexAttribArray(2); // texture
  glBindVertexArray(0);
}

object::~object() {
  glDeleteBuffers(1, &vb_);
  glDeleteVertexArrays(1, &va_);
}

void object::render() {
  glBindVertexArray(va_);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
  glBindVertexArray(0);
}

object create_object_from_file(const std::string path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::vector<vertex> vertices;

  std::string warn;
  std::string err;

  bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
  if (result == false) {
    // blablabla log
  }
/*
  if (!warn.empty()) {
    std::cout << warn << std::endl;
  }

  if (!err.empty()) {
    std::cerr << err << std::endl;
  }
*/

  for (auto& shape: shapes) {
    size_t offset = 0;
    for (auto& face_size: shape.mesh.num_face_vertices) {
      for (size_t vertex_id = 0; vertex_id < face_size; vertex_id++) {
        auto idx = shape.mesh.indices[offset + vertex_id];

        auto vx = attrib.vertices[3 * idx.vertex_index + 0];
        auto vy = attrib.vertices[3 * idx.vertex_index + 1];
        auto vz = attrib.vertices[3 * idx.vertex_index + 2];

/*
        GLfloat nx = 0.0f;
        GLfloat ny = 0.0f;
        GLfloat nz = 0.0f;
        if (3 * idx.normal_index + 2 < static_cast<int>(attrib.normals.size())) {
          nx = attrib.normals[3 * idx.normal_index + 0];
          ny = attrib.normals[3 * idx.normal_index + 1];
          nz = attrib.normals[3 * idx.normal_index + 2];
        }
*/
        GLfloat tx = 0.0f;
        GLfloat ty = 0.0f;
        if (2 * idx.texcoord_index + 1 < static_cast<int>(attrib.texcoords.size())) {
          tx = attrib.texcoords[2 * idx.texcoord_index + 0];
          ty = attrib.texcoords[2 * idx.texcoord_index + 1];
        }

        GLfloat r = 0.0f;
        GLfloat g = 0.0f;
        GLfloat b = 0.0f;
        if (3 * idx.vertex_index + 2 < static_cast<int>(attrib.colors.size())) {
          r = attrib.colors[2 * idx.vertex_index + 0];
          g = attrib.colors[2 * idx.vertex_index + 1];
          b = attrib.colors[2 * idx.vertex_index + 2];
        }

        vertices.emplace_back(glm::vec3{vx, vy, vz}, glm::vec3{r, g, b}, glm::vec2{tx, ty});
      }
      offset += face_size;
    }
  }

  return object{vertices};
}

} // namespace breakout::graphics