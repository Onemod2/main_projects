#include "container_producer_consumer_test.h"
#include "unbounded_stack.h"
#include <exception>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::size_t;

// some template aliases and load_test function
// this part of code belong for specific container
//
// if u want to use consumer (producer) test
// create load_function specify your own container
//
// TODO: may-be create a template part of code
// TODO: (container api should be common)

template<template<typename> class _Container, class _Type>
using method_pop_t = std::shared_ptr<_Type> (_Container<_Type>::*)();

template<template<typename> class _Container, class _Type>
using method_push_t = void (_Container<_Type>::*)(_Type &&);

template<typename _Type>
void unbounded_stack_load_test() {
  // Pointers to method
  using PopType = method_pop_t<unbounded_lock_free_stack, _Type>;
  using PushType = method_push_t<unbounded_lock_free_stack, _Type>;
  PopType pop_method = &unbounded_lock_free_stack<_Type>::pop;
  PushType push_method = &unbounded_lock_free_stack<_Type>::push;

  unbounded_lock_free_stack<_Type> stack;
  std::thread t_producer(producer_creator<unbounded_lock_free_stack, _Type, PushType>, std::ref(stack), push_method);
  std::thread t_consumer(consumer_creator<unbounded_lock_free_stack, _Type, PopType>, std::ref(stack), pop_method);

  t_producer.join();
  t_consumer.join();
}

int main() {
  try {
    unbounded_stack_load_test<int>();
  } catch (const std::exception &e) {
    std::cout << "Exc" << e.what() << std::endl;
  } catch (...) {
    std::cout << "Another" << std::endl;
  }

  return 0;
}