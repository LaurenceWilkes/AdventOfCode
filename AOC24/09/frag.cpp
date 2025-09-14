#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


void printData(const vector<int>& data) {
    for (int i : data) {
	if (i == -1) {
	    cout << '.' << " ";
	} else {
	    cout << i << " ";
	}
    }
    cout << endl;
} // printData

void readFile(string filename, string& compressed) {
    ifstream file(filename);
    getline(file, compressed);
} // readFile

void expand(const string& compressed, vector<int>& data) {
    bool fileFlag = true;
    int IDCount = 0;
    for (char c : compressed) {
	int len = c - '0';
	if (fileFlag) {
	    for (int i = 0; i < len; i++) {
	        data.push_back(IDCount);
	    }
	    IDCount++;
	} else {
	    for (int i = 0; i < len; i++) {
		data.push_back(-1);
	    }
	}
	fileFlag = !fileFlag;
    }
} // expand

void sortData(vector<int>& data) {
    int endPoint = data.size() - 1;
    while (data[endPoint] < 0) {
	endPoint--;
    }
    int startPoint = 0;
    while (data[startPoint] >= 0) {
	startPoint++;
    }
    while (startPoint < endPoint) {
	swap(data[startPoint], data[endPoint]);
	while (data[startPoint] >= 0) {
	    startPoint++;
	}
	while (data[endPoint] < 0) {
	    endPoint--;
	}
    }
} // sortData

void sortDataBlocks(vector<int>& data) {
    int endPoint = data.size() - 1;
    int startPoint = 0;
    while (true) {
//	printData(data);
	bool failFlag = false;

	// Find new endpoint
	while (data[endPoint] < 0) {
	    endPoint--;
	}
	int tempEnd = endPoint;
	while (data[endPoint] == data[tempEnd] && endPoint > 0) {
	    endPoint--;
	}
	if (endPoint <= 0) {
	    break;
	}

	// Find start point
	startPoint = 0;
	int tempStart = 0;
	while (startPoint - tempStart < tempEnd - endPoint) {
	    while (data[startPoint] >= 0) {
		startPoint++;
	    }
	    if (startPoint >= tempEnd) {
		failFlag = true;
		break;
	    }
	    tempStart = startPoint;
	    while (data[startPoint] < 0) {
		startPoint++;
	    }
	}

	// Swap over
	if (!failFlag) {
	    for (int i = 0; i < tempEnd - endPoint; i++) {
		swap(data[tempStart + i], data[endPoint + 1 + i]);
	    }
	}

    }
} // sortData

long checksum(const vector<int>& data) {
    long out = 0;
    for (int i = 0; i < data.size(); i++) {
	if (data[i] >= 0) {
	    out += ((long) data[i]) * i;
	}
    }
    return out;
} // checksum

int main() {
    string compressed;
    readFile("input.txt", compressed);

    vector<int> data;
    expand(compressed, data);

    // part 1
    vector<int> data1 = data;
    sortData(data1);
    long out1 = checksum(data1);
    cout << "checksum for one is: " << out1 << endl;

    // part 2
    vector<int> data2 = data;
    sortDataBlocks(data2);
    long out2 = checksum(data2);
    cout << "checksum for two is: " << out2 << endl;

    return 0;
} // main
