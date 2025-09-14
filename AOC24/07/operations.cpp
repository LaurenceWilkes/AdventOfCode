#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

using namespace std;

void readFile(string filename, vector<long>& outputs, vector<vector<long>>& inputs) {
    ifstream file(filename);
    string line;
    regex pattern(R"((\d+): (.*))");
    while (getline(file, line)) {
	smatch match;
	vector<long> currentLongs;
	if (regex_match(line, match, pattern)) {
	    outputs.push_back(stol(match[1]));
	    stringstream ss(match[2]);
	    string current;

	    while (ss >> current) {
	        currentLongs.push_back(stol(current));
	    }
	}
	inputs.push_back(currentLongs);
    }
} // readFile

bool incCounter(vector<int>& counter, int base) {
    int ci = 0;
    if (counter[0] >= base-1) {
	counter[0] = 0;
	while (true) {
	    ci++;
	    if (ci == counter.size()) {
		return false;
	    }
	    if (counter[ci] >= base-1) {
		counter[ci] = 0;
	    } else {
		counter[ci]++;
		ci = 0;
		break;
	    }
	}
    } else {
	counter[0]++;
    }
    return true;
} // incCounter

long testConf(const vector<long>& in, const vector<int>& counter) {
    long testVal = in[0];
    for (int j = 0; j < counter.size(); j++) {
	if (counter[j] == 0) {
	    testVal += in[j+1];
	} else if (counter[j] == 1) {
	    testVal *= in[j+1];
	} else {
	   testVal = stol(to_string(testVal) + to_string(in[j+1]));
	}
    }
    return testVal;
} // testConf

bool isValid(const long& out, const vector<long>& in, int base) {
    vector<int> counter(in.size()-1, 0);

    if (testConf(in, counter) == out) return true;
    while (incCounter(counter, base)) {
	if (testConf(in, counter) == out) return true;
    }
    return false;
} // isValid

long sumValid(const vector<long>& outputs, const vector<vector<long>>& inputs, int base) {
    long sumVal = 0;
    for (int i = 0; i < outputs.size(); i++) {
        if (isValid(outputs[i], inputs[i], base)) {
            sumVal += outputs[i];
        }
    }
    return sumVal;
} // sumValid

int main() {
    vector<long> outputs;
    vector<vector<long>> inputs;
    readFile("input.txt", outputs, inputs);

    cout << "The sum of valid values for part 1 is: " << sumValid(outputs, inputs, 2) << endl;
    cout << "The sum of valid values for part 2 is: " << sumValid(outputs, inputs, 3) << endl;

    return 0;
} // main

//bool incCounter(vector<bool>& counter) {
//    int ci = 0;
//    if (counter[0]) {
//	counter[0] = false;
//	while (true) {
//	    ci++;
//	    if (ci == counter.size()) {
//		return false;
//	    }
//	    if (counter[ci]) {
//		counter[ci] = false;
//	    } else {
//		counter[ci] = true;
//		ci = 0;
//		break;
//	    }
//	}
//    } else {
//	counter[0] = true;
//    }
//    return true;
//} // incCounter

//long testConf(const vector<long>& in, const vector<bool>& counter) {
//    long testVal = in[0];
//    for (int j = 0; j < counter.size(); j++) {
//	if (counter[j]) {
//	    testVal += in[j+1];
//	} else {
//	    testVal *= in[j+1];
//	}
//    }
//    return testVal;
//} // testConf
