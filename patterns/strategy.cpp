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

struct FlyBehaviour {
    virtual void fly() = 0;
};

struct SimpleFly : FlyBehaviour {
    void fly() override {
        cout << "Im flying\n";
    }
};

struct NoFly : FlyBehaviour {
    void fly() override {
        cout << "I can't flying\n";
    }
};

struct SoundBehaviour {
    virtual void sound() = 0;
};

struct Quack : SoundBehaviour {
    void sound() override {
        cout << "quack\n";
    }
};

struct MutableQuack : SoundBehaviour {
    void sound() override {
        cout << "I'm mute\n";
    }
};

struct Creature {
    virtual void sound() = 0;
    virtual void fly() = 0;

    Creature(FlyBehaviour *fb, SoundBehaviour *sb)
            : fly_behaviour(fb), sound_behaviour(sb) {}

    unique_ptr<FlyBehaviour> fly_behaviour;
    unique_ptr<SoundBehaviour> sound_behaviour;
};

struct Duck : Creature {
    Duck() : Creature(new SimpleFly(), new Quack()) {}

    void sound() override {
        sound_behaviour->sound();
    }
    void fly() override {
        fly_behaviour->fly();
    }
};

struct RubberDuck : Creature {
    RubberDuck() : Creature(new NoFly(), new MutableQuack()) {}

    void sound() override {
        sound_behaviour->sound();
    }
    void fly() override {
        fly_behaviour->fly();
    }
};


int main() {
    unique_ptr<Creature> creature(new RubberDuck());

    creature->sound();
    creature->fly();

    return 0;
}
