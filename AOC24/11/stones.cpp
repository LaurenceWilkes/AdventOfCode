#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>


using namespace std;

void updateVec(vector<long>& input) {
    vector<long> posMem;
    vector<long> insVal;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == 0) {
	    input[i] = 1;
        } else if ((int(log10(input[i])) + 1) % 2 == 0) {
	    string si = to_string(input[i]);
	    input[i] = stol(si.substr(0, si.length() / 2));
	    posMem.push_back(i);
	    insVal.push_back(stol(si.substr(si.length() / 2)));
        } else {
            input[i] *= 2024;
        }
    }
    for (int i = posMem.size() - 1; i >= 0; i--) {
        input.insert(input.begin() + posMem[i] + 1, insVal[i]);
    }
} // updateVec

long simFor(map<pair<long, int>, long>& mem, long i, int n) {
    if (n == 0) {
        return 1;
    }
    pair<long, int> mp = {i, n};
    if (mem.count(mp) > 0) {
	return mem.at(mp);
    }
    if (i == 0) {
	mem[mp] = simFor(mem, 1, n-1);
	return mem[mp];
    } else if ((int(log10(i)) + 1) % 2 == 0) {
	string si = to_string(i);
	long firstHalf = stol(si.substr(0, si.length() / 2));
	long secondHalf = stol(si.substr(si.length() / 2));
	mem[mp] = simFor(mem, firstHalf, n-1) + simFor(mem, secondHalf, n-1);
	return mem[mp];
    } else {
	mem[mp] = simFor(mem, 2024 * i, n-1);
	return mem[mp];
    }
} // simFor


void displayVec(vector<long>& input) {
    for (int j = 0; j < input.size(); j++) {
	cout << input[j] << " ";
    }
    cout << endl;
} // displayVec

int main() {
    vector<long> test = {0};
    vector<long> myinput = {6563348, 67, 395, 0, 6, 4425, 89567, 739318};
    vector<long> testinput = {0, 1, 10, 99, 999};
    vector<long> testinput2 = {125, 17};

    vector<long> testing = myinput;
    int n = 75;

//    for (int i = 0; i < n; i++) {
//        updateVec(testing);
//    }
//    cout << "length after " << n << " tries is: " << testing.size() << endl;

    map<pair<long, int>, long> mem;
    long curSum = 0;
    for (int it : testing) {
        curSum += simFor(mem, it, n);
    }
    cout << "Alternatively, length after " << n << " tries is: " << curSum << endl;

    return 0;
} // main
