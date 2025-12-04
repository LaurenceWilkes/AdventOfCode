#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <tuple>
#include <climits>

using namespace std;
using Point = pair<int, int>;
using CharPair = pair<char, char>;

const map<CharPair, vector<string>> keyMapFrom = {
    {{'^','^'}, {"A"}},
    {{'^','A'}, {">A"}},
    {{'^','<'}, {"v<A"}},
    {{'^','v'}, {"vA"}},
    {{'^','>'}, {"v>A", ">vA"}},

    {{'A','A'}, {"A"}},
    {{'A','^'}, {"<A"}},
    {{'A','<'}, {"v<<A"}},
    {{'A','v'}, {"<vA", "v<A"}},
    {{'A','>'}, {"vA"}},

    {{'<','<'}, {"A"}},
    {{'<','^'}, {">^A"}},
    {{'<','A'}, {">>^A"}},
    {{'<','v'}, {">A"}},
    {{'<','>'}, {">>A"}},

    {{'v','v'}, {"A"}},
    {{'v','^'}, {"^A"}},
    {{'v','A'}, {"^>A", ">vA"}},
    {{'v','<'}, {"<A"}},
    {{'v','>'}, {">A"}},

    {{'>','>'}, {"A"}},
    {{'>','^'}, {"<^A", "^<A"}},
    {{'>','A'}, {"^A"}},
    {{'>','<'}, {"<<A"}},
    {{'>','v'}, {"<A"}}
};

Point numCoords(char c) {
    switch(c) {
	case 'A' : return {0, 0};
	case '0' : return {-1, 0};
	case '1' : return {-2, 1};
	case '2' : return {-1, 1};
	case '3' : return {0, 1};
	case '4' : return {-2, 2};
	case '5' : return {-1, 2};
	case '6' : return {0, 2};
	case '7' : return {-2, 3};
	case '8' : return {-1, 3};
	case '9' : return {0, 3};
    }
    return {0, 0};
}

void readInput(string filename, vector<string>& inputs) {
    ifstream f(filename);
    string line;
    while (getline(f, line)) {
	inputs.push_back(line);        
    }
} // readInput

long long keySmallestLen(char from, char to, int level, map<tuple<char, char, int>, long long>& memo) {
    if (level == 0) return 1;

    tuple<char, char, int> key = make_tuple(from, to, level);
    if (memo.count(key)) return memo[key];

    long long best = LLONG_MAX;
    for (string seq : keyMapFrom.at({from, to})) {
	char prev = 'A';
	long long len = 0;
	for (char c : seq) {
	    len += keySmallestLen(prev, c, level - 1, memo);
	    prev = c;
	}
	if (len < best) best = len;
    }

    memo[key] = best;
    return best;
} // keySmallestLen

string numPath(char a, char b, bool horFirst) {
    Point aPt = numCoords(a);
    Point bPt = numCoords(b);

    int hor = bPt.first  - aPt.first;
    int ver = bPt.second - aPt.second;

    string path = "";
    auto addPath = [&](char ch, int n) {
	for (int i = 0; i < abs(n); i++) path += ch;
    };

    if (horFirst) {
	if (hor > 0) addPath('>', hor);
	else if (hor < 0) addPath('<', hor);
	if (ver > 0) addPath('^', ver);
	else if (ver < 0) addPath('v', ver);
    } else {
	if (ver > 0) addPath('^', ver);
	else if (ver < 0) addPath('v', ver);
	if (hor > 0) addPath('>', hor);
	else if (hor < 0) addPath('<', hor);
    }
    path += 'A';
    return path;
} // numPath

long long bestNumLen(char a, char b, int level, map<tuple<char, char, int>, long long>& memo) {
    Point aPt = numCoords(a);
    Point bPt = numCoords(b);

    long long best = LLONG_MAX;
    if (aPt.second != 0 || bPt.first != -2) {
	string np = numPath(a, b, true);
	char prev = 'A';
	long long len = 0;
	for (char c : np) {
	    len += keySmallestLen(prev, c, level, memo);
	    prev = c;
	}
	if (len < best) best = len;
    }
    if (aPt.first != -2 || bPt.second != 0) {
	string np = numPath(a, b, false);
	char prev = 'A';
	long long len = 0;
	for (char c : np) {
	    len += keySmallestLen(prev, c, level, memo);
	    prev = c;
	}
	if (len < best) best = len;
    }
    return best;
} // bestNumLen

long long countBestLen(string str, int level, map<tuple<char, char, int>, long long>& memo) {
    long long out = 0;
    char prev = 'A';
    for (char c : str) {
	out += bestNumLen(prev, c, level, memo);
	prev = c;
    }
    return out;
} // countBestLen

int findNo(string str) {
    str.pop_back();
    return stoi(str);
}

int main() {
    vector<string> inputs;
    readInput("input.txt", inputs);
    long long outSum1 = 0;
    int level1 = 2; 
    map<tuple<char, char, int>, long long> memo;
    for (string str : inputs) {
	cout << "str: " << str << endl;
	long long presses = countBestLen(str, level1, memo);
	int number = findNo(str);
	outSum1 += presses * number;
	cout << "presses: " << presses << " product: " << presses * number << endl;
    }
    cout << endl << "Part 1 total is: " << outSum1 << endl << endl;

    long long outSum2 = 0;
    int level2 = 25;
    for (string str : inputs) {
	cout << "str: " << str << endl;
	long long presses = countBestLen(str, level2, memo);
	int number = findNo(str);
	outSum2 += presses * number;
	cout << "presses: " << presses << " product: " << presses * number << endl;
    }
    cout << endl << "Part 2 total is: " << outSum2 << endl;
    return 0;
} // main
