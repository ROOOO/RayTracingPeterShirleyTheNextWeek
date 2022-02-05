#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class material;

class sphere: public hitable {
public:
  sphere() {}
  sphere(vec3 cen, float r, material* mat_ptr = nullptr)
    : center(cen)
    , radius(r)
    , mat_ptr(mat_ptr) {}
  virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
  virtual bool bounding_box(float t0, float t1, aabb& box) const;

  vec3 center;
  float radius;
  material* mat_ptr;
};

void get_sphere_uv(const vec3& p, float& u, float& v) {
  float phi = atan2(p.z(), p.x());
  float theta = asin(p.y());
  u = 1.0f - (phi + M_PI) / (2.0f * M_PI);
  v = (theta + M_PI * 0.5f) / M_PI;
}
bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(b * b - a * c)) / a;
    if (temp >= tmax || temp <= tmin) {
      temp = (-b + sqrt(b * b - a * c)) / a;
    }
    if (temp < tmax && temp > tmin) {
      rec.t = temp;
      rec.p = r.point_at_parameter(temp);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      get_sphere_uv(rec.normal, rec.u, rec.v);
      return true;
    }
  }
  return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
  box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
  return true;
}

#endif
