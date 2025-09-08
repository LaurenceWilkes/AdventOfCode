#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

int groupProcess1(vector<string> group) {
    set<char> questionset;
    for (string st : group) {
        for (char c : st) {
            questionset.insert(c);
        }
    }
    return questionset.size();
} // groupProcess

int groupProcess(vector<string> group) {
    set<char> questionset(group[0].begin(), group[0].end());
    int len = group.size();
    set<char> tempset;

    for (int i = 1; i < len; i++) {
	for (char c : questionset) {
	    for (char d : group[i]) {
	        if (d == c) {
	            tempset.insert(c);
	        } 
	    }
	}
	questionset = tempset;
	tempset.clear();
    }
    return questionset.size();
} // groupProcess

int main() {
    ifstream file("input.txt");

    if (!file.is_open()) {
	cerr << "Error: Could not open the file!" << endl;
	return 1;
    }

    string line;
    vector<string> currentGroup;
    int total = 0;

    while (getline(file, line)) {
	if (line.empty()) {
	    total += groupProcess(currentGroup);
	    currentGroup.clear();
	} else {
	    currentGroup.push_back(line);
	}
    }
    total += groupProcess(currentGroup);
    
    file.close();
   
    cout << "The sum of the counts is: " << total << endl;

    return 0;
} // main

