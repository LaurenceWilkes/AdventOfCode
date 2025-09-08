#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;


pair<int, int> stringDecode(string seatno) {
    int column = 0; 
    int row = 0;
    for (int i = 0; i < 7; i++) {
	if (seatno[i] == 'B') {
	    row += 1 << (6-i);
	} 
    }
    for (int i = 7; i < 10; i++) {
	if (seatno[i] == 'R') {
	    column += 1 << (9-i);
	}  
    }
    return {row, column}; 
} // stringDecode

int main() {
    
    ifstream file("input.txt");

    if (!file.is_open()) {
	cerr << "Error opening file!" << endl;
	return 1;
    }

    string line;

    int HighestID = 0;
    int LowestID = 1000;
    int ID;

    vector<int> IDs;

    while (getline(file, line)) {
	auto [row, column] = stringDecode(line);
	ID = 8 * row + column;
	if (ID > HighestID) {
	    HighestID = ID;
	} 
	if (ID < LowestID) {
	    LowestID = ID;
	} 
	IDs.push_back(ID);
    }

    file.close();

    cout << "Highest ID is: " << HighestID << endl;

    sort(IDs.begin(), IDs.end());

    for (int i = LowestID; i < HighestID+1; i++) {
	if (IDs[i-LowestID] != i) {
	    cout << "bad id: " << i << endl;
	    break;
	} 
    }

    return 0;
} // main

