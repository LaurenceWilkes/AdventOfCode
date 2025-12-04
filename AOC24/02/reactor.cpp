#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

bool isSafe(const vector<int>& data) {
    int prev = data[0];
    bool dir = data[1] - data[0] > 0;
    for (int i = 1; i < data.size(); i++) {
	int diff = data[i] - prev;
        if (abs(diff) > 3 || abs(diff) < 1) {
	    // cout << "False for diff between elements" << endl;
            return false;
        } else if ((diff > 0) != dir) {
	    // cout << "False for change in direction" << endl;
	    return false;
        }
	prev = data[i];
    }
    return true;
} // isSafe

bool isSafeDamp(const vector<int>& data) {
    if (isSafe(data)) return true;
    vector<int> temp;
    for (int i = 0; i < data.size(); i++) {
	for (int j = 0; j < data.size(); j++) {
	    if (j != i) temp.push_back(data[j]);
	}
	if (isSafe(temp)) return true;
	temp.clear();
    }
    return false;
} // isSafe

int readCount(string filename) {
    ifstream file(filename);

    string line;

    int count = 0;

    while (getline(file, line)) {
        stringstream ss(line);

	string num;

	vector<int> dataLine;

	while (ss >> num) {
	    dataLine.push_back(stoi(num));
	    // cout << stoi(num) << " ";
	}
	// cout << endl;

	if (isSafeDamp(dataLine)) {
	    count++;
	}
    }
    return count;
} // readCount

int main() {
    int out = readCount("input.txt");
    cout << "The count of good strings is " << out << endl;
    return 0;
} // main







