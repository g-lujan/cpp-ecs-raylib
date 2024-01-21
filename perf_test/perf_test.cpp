#include "ecs.hpp"
#include <memory>
#include <random>
#include <chrono>
#include <iostream>

using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

float get_random();

struct Serializable {};

struct C1 : public Component {
  C1() : val{get_random()} {}
  virtual std::string type_name() const { return "C1"; }
  virtual std::unique_ptr<Serializable> serialize() { return std::make_unique<Serializable>(); }

  float val;
};

struct C2 : public Component {
  C2() : val{get_random()} {}
  virtual std::string type_name() const { return "C2"; }
  virtual std::unique_ptr<Serializable> serialize() { return std::make_unique<Serializable>(); }

  float val;
};

struct C3 : public Component {
  C3() : val{get_random()} {}
  virtual std::string type_name() const { return "C3"; }
  virtual std::unique_ptr<Serializable> serialize() { return std::make_unique<Serializable>(); }

  float val;
};

namespace System {
  struct S1 {};
  struct S2 {};
}

template <> void ECS::run_system<System::S1>()
{
  for (auto &id : all_components<C1, C2>()) {
    auto &c1 = component<C1>(id);
    auto &c2 = component<C2>(id);
    float div12 = c1.val / c2.val;
    float div21 = c2.val / c1.val;
  }
}

template <> void ECS::run_system<System::S2>()
{
  for (auto &id : all_components<C1, C2, C3>()) {
    auto &c1 = component<C1>(id);
    auto &c2 = component<C2>(id);
    auto &c3 = component<C3>(id);
    float div12 = c1.val / c2.val;
    float div21 = c2.val / c1.val;
    float div31 = c3.val / c1.val;
    float div32 = c3.val / c2.val;
  }
}

void spawn_entities(ECS &ecs, int n_entities)
{
  for (int i = 0; i < n_entities; ++i) {
    ecs.spawn_entity(C1());
    ecs.spawn_entity(C2());
    ecs.spawn_entity(C3());
  }
}

int main(int argc, const char *argv[])
{
  if (argc != 3) {
    std::cout << "Usage: perf_test <n_entities/3> <n_iterations>";
    return 0;
  }

  ECS ecs;
  auto spawn_being = high_resolution_clock::now();
  spawn_entities(ecs, std::stoi(argv[1]));
  auto time_to_run_spawns = duration_cast<milliseconds>(high_resolution_clock::now() - spawn_being);
  std::cout << std::format("Time to perform {} spawns: {} \n", 3 * std::stoi(argv[1]), time_to_run_spawns);

  auto iterations_being = high_resolution_clock::now();
  for (int iteration = 0; iteration < std::stoi(argv[2]); ++iteration) {
    ecs.run_system<System::S1>();
    ecs.run_system<System::S2>();
  }
  auto time_to_run_iterations = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - iterations_being);
  std::cout << std::format("Time to run {} iterations: {} \n", std::stoi(argv[2]), time_to_run_iterations);
}


float get_random()
{
  static std::default_random_engine e;
  static std::uniform_real_distribution<> dis(10, 1e9);
  return dis(e);
}

