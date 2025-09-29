export module perlin;
import dotz;
import rng;

export class perlin {
  int p[512];
  dotz::vec2 g[512];

  static inline constexpr float s_curve(float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
  }

public:
  constexpr perlin() {
    for (auto i = 0; i < 256; i++) {
      p[i] = i;
      g[i] = dotz::normalise(dotz::vec2(rng::randf(), rng::randf()) * 2.0f - 1.0f);
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

  constexpr float noise(dotz::vec2 pos) const {
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

    auto u = dotz::mix(
        r0.x * g00.x + r0.y * g00.y,
        r1.x * g10.x + r0.y * g10.y,
        s.x);
    auto v = dotz::mix(
        r0.x * g01.x + r1.y * g01.y,
        r1.x * g11.x + r1.y * g11.y,
        s.x);

    return dotz::mix(u, v, s.y);
  }

  constexpr float operator()(dotz::vec2 pos) const { return noise(pos); }
};
