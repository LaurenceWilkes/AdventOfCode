#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <unordered_map>
#include <queue>
#include <set>

using namespace std;

void readInput(string filename, unordered_map<string, bool>& vals, unordered_map<string, tuple<string, char, string>>& gates, vector<string>& outStrings) {
    ifstream f(filename);
    string line;
    bool firstPart = true;
    while (getline(f, line)) {
	if (line.empty()) {
	    firstPart = false;
	    continue;
	}
        if (firstPart) {
	    string key = line.substr(0, 3);
	    bool val = stoi(line.substr(5));
	    vals[key] = val;
	} else {
	    string lhs, op, rhs, arrow, out;
	    stringstream ss(line);
	    ss >> lhs >> op >> rhs >> arrow >> out; 
	    if (out[0] == 'z') {outStrings.push_back(out);}
	    char opChar = op[0]; 
	    gates[out] = make_tuple(lhs, opChar, rhs);
	}
    }
    sort(outStrings.begin(), outStrings.end(), greater<string>()); // outStrings sorted from largest to smallest
} // readInput

bool getVal(string input, unordered_map<string, bool>& vals, const unordered_map<string, tuple<string, char, string>>& gates) {
   if (vals.count(input)) {return vals.at(input);}
   auto [str1, g, str2] = gates.at(input);
   if (g == 'O') {
       return getVal(str1, vals, gates) | getVal(str2, vals, gates);
   } else if (g == 'A') {
       return getVal(str1, vals, gates) & getVal(str2, vals, gates);
   } else {
       return getVal(str1, vals, gates) ^ getVal(str2, vals, gates);
   }
} // getVal

int main() {
    unordered_map<string, bool> vals;
    unordered_map<string, tuple<string, char, string>> gates;
    vector<string> outStrings;
    readInput("input.txt", vals, gates, outStrings);
  
    // part 1: 
//    long long result = 0;
//    cout << "Base 2 result: ";
//    for (auto str : outStrings) {
//	result <<= 1;
//	bool val = getVal(str, vals, gates);
//	cout << val;
//	if (val) result++;
//    }
//    cout << endl;
//    cout << "Base 10 result: " << result << endl;
//    return 0;

    // part 2: used the following to deduce the answer and did swap(gates["aaa"], gates["bbb"]) to test.
    for (int i = 0; i < 10; i++) {
	cout << "level " << i << ": ";
        string inp = "z0" + to_string(i);
	auto [a, op, b] = gates[inp];
	cout << a << " " << op << " " << b << " -> " << inp << ", " << endl;
//	cout << op << endl;
    }
    for (int i = 10; i < 46; i++) {
	cout << "level " << i << ": ";
        string inp = "z" + to_string(i);
	auto [a, op, b] = gates[inp];
	cout << a << " " << op << " " << b << " -> " << inp << ", " << endl;
//	cout << op << endl;
    }
    cout << endl;
    cout << endl;

    queue<string> inputs;
    inputs.push("z45");
    queue<string> next;

    int level = 0;
    while (!inputs.empty()) {
	cout << "level " << level << ": ";
	while (!inputs.empty()) {
	    auto inp = inputs.front();
	    inputs.pop();

	    if (!gates.count(inp)) {
//		cout << "end - " << inp << ", ";
		continue;
	    }

	    auto [a, op, b] = gates[inp];
	    cout << a << " " << op << " " << b << " -> " << inp << ", ";
//	    cout << op << ", ";
	    next.push(a);
	    next.push(b);
	}
	cout << endl;
	swap(inputs, next);
	level++;
    }

} // main
