#pragma leco test
import perlin;

import dotz;
import rng;
import stubby;

int main() {
  rng::seed();
  perlin p {};

  stbi::pixel img[1024 * 1024] {};

  for (auto y = 0; y < 1024; y++) {
    for (auto x = 0; x < 1024; x++) {
      int c = p(dotz::vec2 { x, y } / 32.0) * 128 + 127;
      auto cc = static_cast<unsigned char>(c & 0xFF);
      img[y * 1024 + x] = { cc, cc, 255, 255 };
    }
  }

  stbi::write_rgba_unsafe("out/test.png", 1024, 1024, img);
}
