#include <iostream>
#include <string>

using namespace std;

int main() {
	string s, search; cin >> s >> search;
	int p[search.size() + 1];
	p[0] = -1;
	
	for (size_t i = 1; i < search.size() + 1; ++i) {
		int k = p[i-1];		
		while (k >= 0 && search[i-1] != search[k]) {
			k = p[k];	
		}
		p[i] = k + 1;
	}

	int k = 0;
	for (size_t i = 0; i < s.size(); ++i) {
		while (k >= 0 && s[i] != search[k]) {
			k = p[k];
		}

		k++;	

		if (k == search.size()) {
			cout << i - k + 1 << endl;
		}
	}

	return 0;
}
