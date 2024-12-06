#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

// 연산자의 우선순위를 반환하는 함수
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 두 피연산자와 연산자를 받아 계산하는 함수
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b != 0) return a / b;
            cerr << "Error: Division by zero" << endl;
            return 0;
        default:
            cerr << "Error: Invalid operator" << endl;
            return 0;
    }
}

// 주어진 수식을 평가하는 함수
double evaluate(const string& expression) {
    stack<double> values;
    stack<char> ops;
    istringstream iss(expression);
    char token;

    while (iss >> token) {
        if (isspace(token)) {
            continue;
        } else if (isdigit(token) || token == '.') {
            iss.putback(token);
            double value;
            if (!(iss >> value)) {
                cerr << "Error: Invalid number" << endl;
                return 0;
            }
            values.push(value);
        } else if (token == '(') {
            ops.push(token);
        } else if (token == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            if (ops.empty()) {
                cerr << "Error: Mismatched parentheses" << endl;
                return 0;
            }
            ops.pop();
        } else if (token == '+' || token == '-' || token == '*' || token == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(token);
        } else {
            cerr << "Error: Invalid character" << endl;
            return 0;
        }
    }

    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    if (values.size() != 1) {
        cerr << "Error: Invalid expression" << endl;
        return 0;
    }

    return values.top();
}

int main() {
    string input;

    cout << "Enter expression (e.g., 1.5 + 2.3 * (3.1 - 4.2) / 5.0): ";
    getline(cin, input);

    double result = evaluate(input);
    cout << "Result: " << result << endl;

    return 0;
}