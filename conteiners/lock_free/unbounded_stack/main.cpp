#include <iostream>
#include <atomic>
#include <memory>

using std::cout;
using std::cin;
using std::endl;
using std::size_t;

template <typename T>
class unbounded_lock_free_stack {
private:
    struct node {
        std::shared_ptr<T> data;
        node *next;

        node() = default;
        template <typename TT>
        node(TT&& obj) : data(std::make_shared(std::forward<TT>(obj))) {}
    };
    std::atomic<node*> head;
public:
    template <typename T>
    void push(T&& obj) {
        node *new_node = new node(std::forward<T>(obj)); // maybe leak
        node *old_head = head;
        while (!head.compare_exchange_weak(old_head, new_node, std::memory_order_seq_cst)) {}
    }

    // false on empty buffer
    bool try_pop(std::shared_ptr<T>& customer) {
        if (!head) {
            return false; // empty
        }
        node* old_head = head;
        while (old_head && head.compare_exchange_weak(old_head, old_head->next,
                                                      std::memory_order_seq_cst)) {}
        if (!old_head) {
            return false;
        }
        customer.swap(old_head->data); // swap old_head to nullptr
        return true;
    }

    // exception on empty buffer
    T pop() {

    }

};

int main() {

    return 0;
}