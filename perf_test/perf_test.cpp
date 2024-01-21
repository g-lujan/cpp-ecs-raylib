#include "ecs.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

double get_random();

struct Serializable {};

struct C1 : public Component {
  virtual std::string type_name() const { return "C1"; }
  virtual std::unique_ptr<Serializable> serialize() { return std::make_unique<Serializable>(); }
};

namespace System {
  struct S1 {};
} // namespace System

template <> void ECS::run_system<System::S1>()
{
  for (auto &id : all_components<C1>()) {
    // work for every component
    double a = get_random(), b = get_random(), c = get_random();
    double product = a * b;
    double div = product / c;
  }
}

void spawn_entities(ECS &ecs, int n_entities)
{
  for (int i = 0; i < n_entities; ++i) {
    ecs.spawn_entity(C1());
  }
}

/* Test time to run multiples systems in a single frame */
int main(int argc, const char *argv[])
{
  if (argc != 3) {
    std::cout << "Usage: perf_test <n_entities/3> <n_systems>";
    return 0;
  }

  ECS ecs;
  auto spawn_being = high_resolution_clock::now();
  spawn_entities(ecs, std::stoi(argv[1]));
  auto time_to_run_spawns = duration_cast<milliseconds>(high_resolution_clock::now() - spawn_being);
  std::cout << std::format("Time to perform {} spawns: {} \n", 3 * std::stoi(argv[1]), time_to_run_spawns);

  // emulate multiple systems runs
  auto systems_being = high_resolution_clock::now();
  for (int i = 0; i < std::stoi(argv[2]); ++i) {
    ecs.run_system<System::S1>();
  }
  auto time_to_run_systems = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - systems_being);
  std::cout << std::format("Time to run {} systems with {} entities: {} \n", std::stoi(argv[2]), std::stoi(argv[1]), time_to_run_systems);
}

double get_random()
{
  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(10, 1e9);
  return dis(e);
}
