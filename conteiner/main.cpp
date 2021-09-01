#include <iostream>
#include "allocator.h"
#include <type_traits>

using namespace std;

struct T {
	T() = delete;
	T(int v) { cout << "construct\n"; }
	~T() { cout << "destroy\n"; }

	T(const T& obj)  {
		cout << "copy\n";
	}

	T(T&& obj) {
		val = std::move(obj.val);	
		cout << "move\n";
	}
	int val = 0;
};

int main() {
	Allocator<T> Alloc(100);
	
	using AllocatorTraits = allocator_traits<Allocator<T>>;
	T obj(4);
	T* ptr = AllocatorTraits::allocate(Alloc, 1);
	AllocatorTraits::construct(Alloc, ptr, T(4));	
	AllocatorTraits::destroy(Alloc, ptr);
	return 0;
}
