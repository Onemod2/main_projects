#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <cstdlib>

void add_poly(std::map<std::size_t, int>& poly, std::size_t pow, int k) {
    auto it = poly.find(pow);
    if (it != poly.end()) {
        it->second += k;
    }
    else {
        poly[pow] = k;
    }
}

std::string derivative(std::string polynomial) {
    std::map<std::size_t, int> pows;
    std::size_t i = 0;
    std::size_t n = polynomial.size();

    while (i < n) {
        bool is_neg = false;
        std::string pow = "", k = "";

        if (polynomial[i] == '-' || polynomial[i] == '+') {
            if (polynomial[i] == '-') is_neg = true;
            i++;
        }

        while (int(polynomial[i]) >= 48 && int(polynomial[i]) <= 57) {
            k += polynomial[i];
            i++;
        }
        if (k == "") {
            k = "1";
            i += 1; // passing (x)
        }
        else {
            if (polynomial[i] == '*') {
                i += 2; // passing multiplication symbol and (x)
            }
            else {
                pow = "0";
            }
        }
        if (i < n && polynomial[i] == '^') {
            i++;
            while (i < n && int(polynomial[i]) >= 48 && int(polynomial[i]) <= 57 ) {
                pow += polynomial[i];
                i++;
            }
        }
        if (pow == "") {
            pow = "1";
        }
        add_poly(pows, std::stoul(pow), std::pow(-1, int(is_neg)) * std::stoi(k));
    }

    std::string answer = "";
    for (auto it = pows.rbegin(); it != pows.rend(); ++it) {
        std::size_t new_pow = it->first;
        int new_k = it->second * int(it->first);
        // derivation of const is 0 (then next part of poly), else new_pow -= 1u (facility derivation)
        if (new_pow == 0) {
            continue;
        }
        else {
            new_pow -= 1u;
        }
        // print + symbol, exclusion first part of poly
        if (answer != "" && new_k > 0) {
            answer += "+";
        }
        // print new_k, exclusion new_k == 1 and new_pow = 0 (new_k = 0)
        if (std::abs(new_k) != 1) {
            answer += std::to_string(new_k);
            if (new_pow != 0) {
                answer += "*";
            }
        }
        if (new_pow == 0) {
            if (std::abs(new_k) == 1) {
                answer += std::to_string(new_k);
            }
            continue;
        }
        answer += "x";
        // print new_pow, exclusion is new_pow == 1
        if (new_pow != 1) {
            answer += "^" + std::to_string(new_pow);
        }
    }
    return answer;
}

int main() {

    std::string s; std::cin >> s;
    std::cout << derivative(s) << std::endl;

    return 0;
}
