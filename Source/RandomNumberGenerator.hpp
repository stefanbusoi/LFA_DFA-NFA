//
// Created by stefa on 5/22/2025.
//
#pragma once
#include <chrono>
#include <random>


class RandomIntGenerator {
private:
  std::mt19937 engine;
  std::uniform_int_distribution<> dist;

public:
  // Constructor with range
  RandomIntGenerator(int min, int max);

  // Generate a random integer
  int generate();

  // Optionally, allow changing the range later
  void setRange(int min, int max);
};


