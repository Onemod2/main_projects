#include <iostream>
#include <vector>
#include <ostream>
#include "conteiner.h"

using namespace std;


struct T {
	T() : val(0) { cout << "create\n"; }
	T(int v) : val(v) { cout << "create from int\n"; }
	T(const T& obj) { val = obj.val; cout << "copy\n"; }
	T(T&& obj) noexcept { val = obj.val; obj.val = 0; cout << "move\n"; }
	
	int val;
};

ostream& operator<<(ostream& os, const T& obj) {
	os << obj.val;
	return os;
}
	

int main() {
	Conteiner<T> cnt;

	for (size_t i = 0; i < 5; ++i) {
		cnt.emplace_back(i);		
	}

	cnt.push_back(T(0));

	for (size_t i = 0; i < cnt.size(); ++i) {
		cout << cnt[i].val << endl; 
	}

	Conteiner<T>::const_iterator it = cnt.cbegin();

	for (; it != cnt.cend(); ++it) {
		cout << *it << ' ';
	}
	cout << endl;
	
	return 0;
}
