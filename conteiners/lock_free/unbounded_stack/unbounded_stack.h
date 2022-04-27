//
// Created by filip on 27.04.2022.
//
#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

template<typename T>
class unbounded_lock_free_stack {
private:
  struct node {
    std::shared_ptr<T> data;
    node *next;

    node() : next(nullptr) {}
    template<typename TT>
    explicit node(TT &&obj)
        : data(std::make_shared<TT>(std::forward<TT>(obj))), next(nullptr) {}
  };
  std::atomic<node *> head = nullptr;

public:
  template<typename TT>
  void push(TT &&obj) {
    node *new_node = new node(std::forward<TT>(obj));
    new_node->next = head;
    while (!head.compare_exchange_weak(new_node->next, new_node, std::memory_order_seq_cst)) {}
  }

  // false on empty buffer
  bool try_pop(std::shared_ptr<T> &customer) {
    if (!head) {
      return false;// empty
    }
    node *old_head = head;
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next,
                                                   std::memory_order_seq_cst)) {}
    if (!old_head) {
      return false;
    }
    customer.swap(old_head->data);// swap old_head to nullptr
    return true;
  }

  // exception on empty buffer
  std::shared_ptr<T> pop() {
    if (!head) {
      return nullptr;
    }
    node *old_head = head;
    while (old_head && !head.compare_exchange_weak(old_head, old_head->next,
                                                   std::memory_order_seq_cst)) {}
    std::unique_ptr<node> deleter(old_head);
    if (!old_head) {
      return nullptr;
    }
    return old_head->data;
  }
};
