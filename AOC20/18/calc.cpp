#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>


using namespace std;

void operate(stack<long>& stk, char c, long val) {
    if (c == '*') {
        stk.top() *= val;
    } else {
        stk.top() += val;
    }

} // operate

void reduce(stack<long>& stk, stack<char>& opstk) {
    long topval = stk.top();
    stk.pop();
    opstk.pop();
    operate(stk, opstk.top(), topval);
} // reduce


long eval(string line) {
    stack<long> stk;
    stack<char> opstk;
    stk.push(0);
    opstk.push('+');

    for (long pos = 0; pos < line.length(); pos++) {
	if (isdigit(line[pos])) {
	    operate(stk, opstk.top(), line[pos] - '0');
	} else if (line[pos] == '+') {
	    opstk.top() = '+';
	} else if (line[pos] == '*') {
	    opstk.top() = '*';
	} else if (line[pos] == '(') {;
	    stk.push(0);
	    opstk.push('+');
	} else if (line[pos] == ')') {
	    reduce(stk, opstk);
	}
    }
    return stk.top();
} // eval

int main() {
    ifstream file("input.txt");

    string line;
    long sum = 0;

    while (getline(file, line)) {
	long val = eval(line);
	cout << "Line val is: " << val << endl;
        sum += val;
    }

    cout << endl << "Total file value is: " << sum << endl;

    return 0;
} // main

