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

	T(T& obj) {
		cout << "test copy\n";
	}
	
	template <typename U>
	T(U&& obj) {
		obj.val = 4;
		cout << "move\n";
	}
	int val = 0;
};

int main() {
	Allocator<T> Alloc(100);
	
	using AllocatorTraits = allocator_traits<Allocator<T>>;
	T obj(4);
	T* ptr = AllocatorTraits::allocate(Alloc, 1);
	Alloc.construct(ptr, obj);	


	return 0;
}
