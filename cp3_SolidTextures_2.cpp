#include <iostream>
#include <cfloat>
#include "sphere.h"
#include "moving_sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "bvh_node.h"

vec3 color(const ray& r, hitable* world, int depth = 0) {
  hit_record rec;
  if (world->hit(r, 0.0001, FLT_MAX, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth + 1);
    }
    return vec3(0, 0, 0);
  } 
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

hitable* two_spheres() {
  int n = 2;
  hitable** list = new hitable*[n];
  texture* checker = new checker_texture(new constant_texture(vec3(0.2f, 0.3f, 0.1f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
  list[0] = new sphere(vec3(0.0f, -10.0f, 0.0f), 10.0f, new lambertion(checker));
  list[1] = new sphere(vec3(0.0f, 10.0f, 0.0f), 10.0f, new lambertion(checker));
  return new bvh_node(list, 2, 0, 1);
}

int main() {
  int nx = 180;
  int ny = 180;
  int ns = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";
  hitable* world = two_spheres();
  vec3 lookfrom(13.0f, 2.0f, 3.0f);
  vec3 lookat(0.0f, 0.0f, 0.0f);
  float dist_to_focus = 10.0f;
  float aperture = 0.0f;
  camera cam(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), 20.0f, float(nx) / float(ny), aperture, dist_to_focus);
  for (int j = ny - 1; j >= 0; --j) {
    for (int i = 0; i < nx; ++i) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; ++s) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}
