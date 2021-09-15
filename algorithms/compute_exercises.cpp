#include <iostream>
#include <stack>
#include <map>

using namespace std;

template <typename T>
T compute(T f, T s, char op) {
    T result;
    switch(op) {
        case '+': result = f + s; break;
        case '-': result = f - s; break;
        case '*': result = f * s; break;
        case '/': result = f / s; break;
    };
    return result;
}

int main() {
    string ex; cin >> ex;
    ex = ex + ")";

    stack<int> nums_stack;
    stack<char> op_stack({'('});

    map<char, int> p;

    p['+'] = 2;
    p['-'] = 2;
    p['*'] = 1;
    p['/'] = 1;
    p['('] = -1;
    p[')'] = 3;

    size_t i = 0;
    size_t n = ex.size();
    while (i < n) {
        string new_number = "";

        while (int(ex[i]) >= 48 && int(ex[i]) <= 57) {
            new_number += ex[i];
            i++;
        }
        if (new_number.size() != 0u) {
            nums_stack.push( stoi(new_number) );
        }

        char op = ex[i];
        while (!op_stack.empty() &&
            op_stack.top() != '(' && p[op] >= p[ op_stack.top() ])
        {
            char prev_op = op_stack.top(); op_stack.pop();
            int b = nums_stack.top(); nums_stack.pop();
            int a = nums_stack.top(); nums_stack.pop();

            nums_stack.push( compute(a, b, prev_op) );
        }

        if (op == ')') {
            op_stack.pop(); // pop '('
        }
        else {
            op_stack.push(op);
        }
        i++;
    }

    if (!op_stack.empty() || nums_stack.size() != 1u) {
        cout << "ERROR\n";
        return 1;
    }
    cout << '(' << ex << " = " << nums_stack.top() << endl;

    return 0;
}