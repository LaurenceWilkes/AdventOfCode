#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;

vector<long> readfile(string filename) {
    ifstream f(filename);
    string line; 
    getline(f, line);
    stringstream ss(line);
    vector<long> out;
    while (ss.good()) {
        string substr;	
	getline(ss, substr, ',');
	out.push_back(stoi(substr));
    }
    return out;
}

long sim(long maxlength, const vector<long>& input) {
    map<long, long> mem;

    for (long i = 0; i < input.size() - 1; i++) {
        mem[input[i]] = i;
    }

    long curr = input.back();
    long length = input.size() - 1;

    while (length < maxlength - 1) {
        if (mem.count(curr) == 0) {
            mem[curr] = length;
            curr = 0;
        } else {
            long dist = length - mem[curr];
            mem[curr] = length;
            curr = dist;
        }
        length++;
    }
    return curr;
} // sim

int main() {
    vector<long> input = readfile("input.txt");

    // Start timing
    auto startTime = chrono::high_resolution_clock::now();

    long out = sim(30000000, input);

    // End timing
    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = endTime - startTime;

    cout << "After 30000000 rounds, sim gives: " << out << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    return 0;
} // main

