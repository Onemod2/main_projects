#include <iostream>
#include <vector>
#include <ostream>
#include "conteiner.h"

using std::cout;
using std::endl;
using std::ostream;

struct T {
	T() : value(0) { cout << "create\n"; }
	T(int v) : value(v) { cout << "create from int\n"; }
	T(const T& obj) { value = obj.value; cout << "copy\n"; }
	T(T&& obj) noexcept { value = obj.value; obj.value = 0; cout << "move\n"; }

	int get_value() const noexcept { return value; }

private:	
	int value;

	friend ostream& operator<<(ostream&, const T&);
};

ostream& operator<<(ostream& os, const T& obj) {
	os << obj.value;
	return os;
}
	

int main() {
	Conteiner<T> cnt;

	for (size_t i = 0; i < 5; ++i) {
		cnt.emplace_back(i);		
	}

	cnt.push_back(T(0));

	for (auto&& element : cnt) {
		cout << element.get_value() << ' '; 
	}
	cout << endl;

	Conteiner<T>::const_iterator it = cnt.cbegin();

	for (; it != cnt.cend(); ++it) {
		cout << *it << ' ';
	}
	cout << endl;
	
	return 0;
}
