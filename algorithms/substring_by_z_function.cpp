#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {

	string s, search; cin >> s >> search;

	string text = search + "#" + s;
	cout << text << endl;	
	int z[text.size()];
	int l = 0;

	z[l] = 0;

	for (int i = 1; i < int(text.size()) - int(search.size()); ++i) {
		z[i] = min( z[l] + l - i, z[i - l]);
		z[i] = max( z[i], 0 );

		while (i + z[i] != text.size() && text[i + z[i]] == text[z[i]]) {
			z[i]++;
		}
		
		if (i + z[i] > l + z[l]) {
			l = i;
		}
		cout << z[i] << ' ';
		if (search.size() == z[i]) {
			cout << '\n' << i - 1 - int(search.size()) << endl; 
			break;
		}
	}

	return 0;
}
