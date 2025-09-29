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

static inline constexpr auto s_curve(float t) {
  return t * t * (3.0f - 2.0f * t);
}

static inline float noise(dotz::vec2 pos) {
  dotz::vec2 t = pos + 0x1000;
  dotz::ivec2 ti { static_cast<int>(t.x), static_cast<int>(t.y) };
  dotz::ivec2 b0 { ti.x & 0xFF, ti.y & 0xFF };
  dotz::ivec2 b1 { (ti.x + 1) & 0xFF, (ti.y + 1) & 0xFF };
  auto r0 = t - ti;
  auto r1 = r0 - 1;
  
  auto i = p[b0.x];
  auto j = p[b1.x];

  auto g00 = g[p[i + b0.y]];
  auto g10 = g[p[j + b0.y]];
  auto g01 = g[p[i + b1.y]];
  auto g11 = g[p[j + b1.y]];

  dotz::vec2 s { s_curve(r0.x), s_curve(r0.y) };

  auto u = dotz::mix(s.x,
      r0.x * g00.x + r0.y * g00.y,
      r1.x * g10.x + r0.y * g10.y);
  auto v = dotz::mix(s.x,
      r0.x * g01.x + r1.y * g01.y,
      r1.x * g11.x + r1.y * g11.y);

  return dotz::mix(s.y, u, v);
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
