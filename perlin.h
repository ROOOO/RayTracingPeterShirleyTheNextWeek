#pragma once

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
  float accum = 0.0f;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        accum += (i * u + (1 - i) * (1 - u)) *
                 (j * v + (1 - j) * (1 - v)) *
                 (k * w + (1 - k) * (1 - w)) * c[i][j][k];
      }
    }
  }
  return accum;
}

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
  float uu = u * u * (3 - 2 * u);
  float vv = v * v * (3 - 2 * v);
  float ww = w * w * (3 - 2 * w);
  float accum = 0.0f;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        vec3 weight_v(u - i, v - j, w - k);
        accum += (i * uu + (1 - i) * (1 - uu)) *
                 (j * vv + (1 - j) * (1 - vv)) *
                 (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
      }
    }
  }
  return accum;
}

class perlin {
public:
  float noise(const vec3& p) const {
    float x = p.x();
    float y = p.y();
    float z = p.z();
    float u = x - floor(x);
    float v = y - floor(y);
    float w = z - floor(z);
    int i = int(floor(x));
    int j = int(floor(y));
    int k = int(floor(z));
    vec3 c[2][2][2];
    for (int di = 0; di < 2; ++di) {
      for (int dj = 0; dj < 2; ++dj) {
        for (int dk = 0; dk < 2; ++dk) {
          c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
        }
      }
    }
    return perlin_interp(c, u, v, w);
  }
  float turb(const vec3& p, int depth=7) const {
    float accum = 0.0f;
    vec3 temp_p = p;
    float weight = 1.0f;
    for (int i = 0; i < depth; ++i) {
      accum += weight * noise(temp_p);
      weight *= 0.5f;
      temp_p *= 2;
    }
    return fabs(accum);
  }
  static vec3* ranvec;
  static int* perm_x;
  static int* perm_y;
  static int* perm_z;
};

#ifdef _WIN32
  #ifndef srand48
    #define srand48(x) srand((int)(x))
  #endif
  #ifndef drand48
    #define drand48() ((double)rand()/(RAND_MAX + 1.0))
  #endif
#endif
static vec3* perlin_generate() {
  unsigned n = 256;
  vec3* p = new vec3[n];
  for (int i = 0; i < n; ++i) {
    p[i] = unit_vector(vec3(-1 + 2 * drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));
  }
  return p;
}

void permute(int* p, int n) {
  for (int i = n - 1; i > 0; --i) {
    int target = int(drand48() * (i + 1));
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}

static int* perlin_generate_perm() {
  unsigned n = 256;
  int* p = new int[n];
  for (int i = 0; i < n; ++i) {
    p[i] = i;
  }
  permute(p, n);
  return p;
}

vec3* perlin::ranvec = perlin_generate();
int* perlin::perm_x = perlin_generate_perm();
int* perlin::perm_y = perlin_generate_perm();
int* perlin::perm_z = perlin_generate_perm();
