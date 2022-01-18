#include "vec3.h"

class texture {
public:
  virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture {
public:
  constant_texture() {}
  constant_texture(const vec3& c) : color(c) {}
  virtual vec3 value(float u, float v, const vec3& p) const {
    return color;
  }
private:
  vec3 color;
};

class checker_texture : public texture {
public:
  checker_texture() {}
  checker_texture(texture* t0, texture* t1) : even(t0), odd(t1) {}
  virtual vec3 value(float u, float v, const vec3& p) const {
    float sines = sin(10.0f * p.x()) * sin(10.0f * p.y()) * sin(10.0f * p.z());
    if (sines < 0) {
      return odd->value(u, v, p);
    }
    return even->value(u, v, p);
  }
private:
  texture* even;
  texture* odd;
};
