#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>


using namespace std;

long ctol(char c) {
    return c - '0';
}

int findPair(const string& line, int st) {
    int pos = st;
    int count = 1;
    while (count != 0) {
	pos++;
        if (line[pos] == '(') {
            count += 1;
        } else if (line[pos] == ')') {
            count -= 1;
        }
    }
    return pos;
}

long eval(const string& line, int st, int end) {
    long backval = 1;
    long curval = 0;
    int pos = st;
    while (pos < end) {
	if (isdigit(line[pos])) {
	    curval += ctol(line[pos]);
	} else if (line[pos] == '*') {
	    backval *= curval;
	    curval = 0;
	} else if (line[pos] == '(') {;
	    int bracketEnd = findPair(line, pos);
	    curval += eval(line, pos+1, bracketEnd);
	    pos = bracketEnd;
	}
	pos++;
    }
    return backval * curval;
} // eval


int main() {
    ifstream file("input.txt");

    string line;
    long sum = 0;

    while (getline(file, line)) {
	for (int i = 0; i < line.length(); i++) {
	    cout << line[i];
	}
	cout << endl;

	long val = eval(line, 0, line.length());
	cout << "Line val is: " << val << endl;
        sum += val;
    }

    cout << endl << "Total file value is: " << sum << endl;

    return 0;
} // main

