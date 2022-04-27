#include "unbounded_stack.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::size_t;

static constexpr size_t PRODUCER_NUM = 1'000u;
static constexpr size_t CONSUMER_NUM = 1'000u;
static constexpr size_t BUNCH_NUM = 100u;

void produce_tasks(unbounded_lock_free_stack<int> &stack, size_t task_number) {
  for (size_t i = 0; i < task_number; ++i) {
    stack.push(static_cast<int>(i));
  }
}

void consume_tasks(unbounded_lock_free_stack<int> &stack, size_t task_number) {
  for (size_t i = 0; i < task_number; ++i) {
    if (auto it = stack.pop(); !it) {
      std::cout << "null" << std::endl;
    } else {
      std::cout << *it << std::endl;
    }
  }
}

void producer_creator(unbounded_lock_free_stack<int> &stack) {
  std::vector<std::thread> producers;
  for (size_t i = 0; i < PRODUCER_NUM; ++i) {
    producers.push_back(std::thread(produce_tasks, std::ref(stack), BUNCH_NUM));
  }
  for (auto &producer: producers) {
    producer.join();
  }
}

void consumer_creator(unbounded_lock_free_stack<int> &stack) {
  std::vector<std::thread> consumers;
  for (size_t i = 0; i < CONSUMER_NUM; ++i) {
    consumers.push_back(std::thread(consume_tasks, std::ref(stack), BUNCH_NUM));
  }
  for (auto &consumer: consumers) {
    consumer.join();
  }
}

void load_test() {
  unbounded_lock_free_stack<int> stack;
  std::thread t_producer(producer_creator, std::ref(stack));
  std::thread t_consumer(consumer_creator, std::ref(stack));

  t_producer.join();
  t_consumer.join();
}

int main() {
  load_test();

  return 0;
}