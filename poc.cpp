#pragma leco test

import dotz;
import rng;
import stubby;

import print;

static inline int p[512];
static inline dotz::vec2 g[512];

static inline void init() {
  for (auto i = 0; i < 256; i++) {
    p[i] = i;
    g[i] = dotz::normalise(dotz::vec2(rng::randf(), rng::randf()));
  }
  for (auto i = 0; i < 256; i++) {
    auto j = rng::rand(256);
    auto k = p[i];
    p[i] = p[j];
    p[j] = k;
  }
  for (auto i = 0; i < 256; i++) {
    p[i + 256] = p[i];
    g[i + 256] = g[i];
  }
}

static inline float noise(dotz::vec2 p) {
  return 1.0;
}

int main() {
  rng::seed();
  init();

  stbi::pixel img[1024 * 1024] {};

  for (auto y = 0; y < 1024; y++) {
    for (auto x = 0; x < 1024; x++) {
      int c = noise({ x, y }) * 128 + 127;
      auto cc = static_cast<unsigned char>(c & 0xFF);
      img[y * 1024 + x] = { cc, cc, 255, 255 };
    }
  }

  stbi::write_rgba_unsafe("out/test.png", 1024, 1024, img);
}
