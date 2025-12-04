#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void readInput(string filename, vector<string>& towels, vector<string>& patterns) {
    ifstream f(filename);
    string line;

    if (getline(f, line)) {
	stringstream ss(line);
	string subs;
	while (getline(ss, subs, ',')) {
	    if (!subs.empty() && subs.front() == ' ') subs.erase(subs.begin());
	    if (!subs.empty()) towels.push_back(subs);
	}
    }
    while (getline(f, line)) {
	if (!line.empty()) patterns.push_back(line);
    }
} // readInput

bool hasAt(const string& s, int pos, const string& t) {
    return pos + t.size() <= s.size() && s.compare(pos, t.size(), t) == 0;
}

long long countWays(const vector<string>& towels, string pattern) {
    int n = pattern.size();
    vector<long long> ways(n + 1, 0);
    ways[0] = 1;
    for (int i = 0; i < n; i++) {
        for (const auto& t : towels) { // Slower than it needs to be. This loop could be simplified.
            if (hasAt(pattern, i, t)) {ways[i + t.size()] += ways[i];}
        }
    }
    return ways[n];
} // countWays

int main() {
    vector<string> towels;
    vector<string> patterns;
    readInput("input.txt", towels, patterns);

    int reachable = 0;
    long long combinations = 0;
    for (const auto& p : patterns) {
        long long w = countWays(towels, p);
	if (w != 0) reachable++;
	combinations += w;
    }
    cout << "Possible patterns: " << reachable << endl;
    cout << "Total combinations: " << combinations << endl;
    return 0;
} // main
