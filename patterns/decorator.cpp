#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>
#include <unordered_set>
#include <iterator>
#include <stack>
#include <cassert>
#include <typeinfo>
#include <memory>

using namespace std;

struct Order {
    [[nodiscard]] virtual size_t cost() const = 0;

    virtual string get_description() const {
        return description;
    }

protected:
    string description;
};

struct Basket : Order {
    Basket() {
        description = "Basket";
    }

    size_t cost() const override {
        return 10u;
    }
};

struct Cart : Order {
    Cart() {
        description = "Cart";
    }

    size_t cost() const override {
        return 0u;
    }
};

struct OrderDecorator : Order {
    explicit OrderDecorator(const Order* prev) : prev(prev) {}
    string get_description() const override = 0;

protected:
    const Order* prev;
};

struct Bananas : OrderDecorator {
    explicit Bananas(const Order* prev) : OrderDecorator(prev) {}
    string get_description() const override {
        return prev->get_description() + ", banana";
    }

    [[nodiscard]] size_t cost() const override {
        return prev->cost() + 5u;
    }
};

struct Apple : OrderDecorator {
    explicit Apple(const Order* prev) : OrderDecorator(prev) {}
    string get_description() const override {
        return prev->get_description() + ", apple";
    }

    size_t cost() const override {
        return prev->cost() + 15u;
    }
};

int main() {
    Order *order = new Bananas(new Basket());

    cout << order->get_description() << endl;
    cout << order->cost() << endl;

    return 0;
}
