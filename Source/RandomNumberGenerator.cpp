//
// Created by stefa on 5/22/2025.
//

#include "RandomNumberGenerator.hpp"

RandomIntGenerator::RandomIntGenerator(int min, int max):
        engine(std::chrono::steady_clock::now().time_since_epoch().count()),
        dist(min, max) { }

int RandomIntGenerator::generate() {
  return dist(engine);
}

void RandomIntGenerator::setRange(int min, int max) {
  dist = std::uniform_int_distribution<>(min, max);
}
