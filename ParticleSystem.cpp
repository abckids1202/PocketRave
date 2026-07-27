#include "ParticleSystem.h"

#include "Config.h"

namespace {

struct Particle {
  int16_t x8;
  int16_t y8;
  int8_t velocityX8;
  int8_t velocityY8;
  uint8_t lifetime;
};

Particle particles[Config::PARTICLE_COUNT];

void respawn(Particle& particle, bool initial) {
  particle.x8 = static_cast<int16_t>(Config::SCREEN_WIDTH / 2) * 8;
  particle.y8 = static_cast<int16_t>(Config::SCREEN_HEIGHT / 2) * 8;
  particle.velocityX8 = static_cast<int8_t>(random(-14, 15));
  particle.velocityY8 = static_cast<int8_t>(random(-12, 13));

  if (particle.velocityX8 > -3 && particle.velocityX8 < 3) {
    particle.velocityX8 = particle.velocityX8 < 0 ? -5 : 5;
  }
  if (particle.velocityY8 > -3 && particle.velocityY8 < 3) {
    particle.velocityY8 = particle.velocityY8 < 0 ? -4 : 4;
  }

  particle.lifetime = initial ? static_cast<uint8_t>(random(1, 50))
                              : static_cast<uint8_t>(random(35, 80));
}

}  // namespace

namespace ParticleSystem {

void initialize() {
  for (uint8_t index = 0; index < Config::PARTICLE_COUNT; ++index) {
    respawn(particles[index], true);
  }
}

void update() {
  for (uint8_t index = 0; index < Config::PARTICLE_COUNT; ++index) {
    Particle& particle = particles[index];
    particle.x8 += particle.velocityX8;
    particle.y8 += particle.velocityY8;
    if (particle.lifetime > 0) {
      --particle.lifetime;
    }

    const int16_t x = particle.x8 / 8;
    const int16_t y = particle.y8 / 8;
    if (particle.lifetime == 0 || x < 0 || x >= Config::SCREEN_WIDTH ||
        y < 0 || y >= Config::SCREEN_HEIGHT) {
      respawn(particle, false);
    }
  }
}

void render(Adafruit_SSD1306& display) {
  for (uint8_t index = 0; index < Config::PARTICLE_COUNT; ++index) {
    const int16_t x = particles[index].x8 / 8;
    const int16_t y = particles[index].y8 / 8;
    if (x >= 0 && x < Config::SCREEN_WIDTH &&
        y >= 0 && y < Config::SCREEN_HEIGHT) {
      display.drawPixel(x, y, SSD1306_WHITE);
      if ((index % 4) == 0 && x + 1 < Config::SCREEN_WIDTH) {
        display.drawPixel(x + 1, y, SSD1306_WHITE);
      }
    }
  }
}

}  // namespace ParticleSystem
