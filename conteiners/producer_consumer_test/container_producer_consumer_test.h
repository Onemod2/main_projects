//
// Created by filip on 28.04.2022.
//
#pragma once

#include <iostream>
#include <thread>
#include <vector>

using std::size_t;

static constexpr size_t PRODUCER_NUM = 1'000u;
static constexpr size_t CONSUMER_NUM = 1'000u;
static constexpr size_t BUNCH_NUM = 100u;

// is_dereferencing traits
namespace traits {
  template <typename ...>
  char deref(...);

  template <typename T>
  auto deref(T) -> decltype(*std::declval<T>(), 4);

  template <typename T>
  constexpr bool is_deref_v = std::is_same_v<decltype(deref<T>(std::declval<T>())), int>;
};

template<template<typename> typename _Container,
         typename _Type, typename _MethodPtr>
void produce_tasks(_Container<_Type> &container, _MethodPtr method_push,
                   size_t task_number) {
  for (size_t i = 0; i < task_number; ++i) {
    (container.*method_push)(static_cast<_Type>(i + 1));
  }
}

template<template<typename> typename _Container,
         typename _Type, typename _MethodPtr>
void consume_tasks(_Container<_Type> &container, _MethodPtr method_pop,
                   size_t task_number) {
  for (size_t i = 0; i < task_number; ++i) {
    if (auto it = (container.*method_pop)(); !it) {
      std::cout << "null" << std::endl;
    } else {
      // type have a dereferencing opportunity
      if constexpr (traits::is_deref_v<decltype(it)>)
      {
        std::cout << *it << std::endl;
      } else {
        std::cout << it << std::endl;
      }
    }
  }
}

template<template<typename> typename _Container,
         typename _Type, typename _MethodPtr>
void producer_creator(_Container<_Type> &container, _MethodPtr method) {
  std::vector<std::thread> producers;
  for (size_t i = 0; i < PRODUCER_NUM; ++i) {
    producers.push_back(std::thread(produce_tasks<_Container, _Type, _MethodPtr>,
                                    std::ref(container), method, BUNCH_NUM));
  }
  for (auto &producer: producers) {
    producer.join();
  }
}

template<template<typename> typename _Container,
         typename _Type, typename _MethodPtr>
void consumer_creator(_Container<_Type> &container, _MethodPtr method) {
  std::vector<std::thread> consumers;
  for (size_t i = 0; i < CONSUMER_NUM; ++i) {
    consumers.push_back(std::thread(consume_tasks<_Container, _Type, _MethodPtr>,
                                    std::ref(container), method, BUNCH_NUM));
  }
  for (auto &consumer: consumers) {
    consumer.join();
  }
}