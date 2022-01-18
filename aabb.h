#include "vec3.h"
#include "ray.h"

class aabb {
public:
  aabb() {}
  aabb(const vec3& a, const vec3& b) {
    _min = a;
    _max = b;
  }

  inline vec3 min() const { return _min; }
  inline vec3 max() const { return _max; }
  inline bool hit(const ray& r, float tmin, float tmax) const;

private:
  vec3 _min;
  vec3 _max;
};

inline bool aabb::hit(const ray& r, float tmin, float tmax) const {
  for (int i = 0; i < 3; ++i) {
    float invD = 1.0f / r.direction()[i];
    float t0 = (min()[i] - r.origin()[i]) * invD;
    float t1 = (max()[i] - r.origin()[i]) * invD;
    if (invD < 0.0f) {
      std::swap(t0, t1);
    }
    tmin = t0 > tmin ? t0 : tmin;
    tmax = t1 < tmax ? t1 : tmax;
    if (tmin >= tmax) {
      return false;
    }
  }
  return true;
}

aabb surrounding_box(const aabb& box0, const aabb& box1) {
  vec3 small(fminf(box0.min().x(), box1.min().x()),
             fminf(box0.min().y(), box1.min().y()),
             fminf(box0.min().z(), box1.min().z()));
  vec3 big(fmaxf(box0.max().x(), box1.max().x()),
           fmaxf(box0.max().y(), box1.max().y()),
           fmaxf(box0.max().z(), box1.max().z()));
  return aabb(small, big);
}
