#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

pair<int, int> readArray(string filename, vector<char>& outVec) {
    ifstream file(filename);

    string line;

    int rowLen = 0;
    int colLen = 1;

    getline(file, line);
    for (char c : line) {
        outVec.push_back(c);
	rowLen++;
    }

    while (getline(file, line)) {
        for (char c : line) {
            outVec.push_back(c);
        }
	colLen++;
    }
    return make_pair(rowLen, colLen);
} // readArray

bool isXMAS(int xp, int yp, char dir, vector<char>& dataVec, int rowLen) {
    bool Xflag = true;
    bool Sflag = true;
    if (dir == 'h') {
	if (dataVec[xp + rowLen * yp] != 'X') Xflag = false;
	if (dataVec[xp + rowLen * yp] != 'S') Sflag = false;
	if (dataVec[xp + 1 + rowLen * yp] != 'M') Xflag = false;
	if (dataVec[xp + 1 + rowLen * yp] != 'A') Sflag = false;
	if (dataVec[xp + 2 + rowLen * yp] != 'A') Xflag = false;
	if (dataVec[xp + 2 + rowLen * yp] != 'M') Sflag = false;
	if (dataVec[xp + 3 + rowLen * yp] != 'S') Xflag = false;
	if (dataVec[xp + 3 + rowLen * yp] != 'X') Sflag = false;
    } else if (dir == 'b') {
	if (dataVec[xp + rowLen * yp] != 'X') Xflag = false;
	if (dataVec[xp + rowLen * yp] != 'S') Sflag = false;
	if (dataVec[xp + 1 + rowLen * (yp + 1)] != 'M') Xflag = false;
	if (dataVec[xp + 1 + rowLen * (yp + 1)] != 'A') Sflag = false;
	if (dataVec[xp + 2 + rowLen * (yp + 2)] != 'A') Xflag = false;
	if (dataVec[xp + 2 + rowLen * (yp + 2)] != 'M') Sflag = false;
	if (dataVec[xp + 3 + rowLen * (yp + 3)] != 'S') Xflag = false;
	if (dataVec[xp + 3 + rowLen * (yp + 3)] != 'X') Sflag = false;
    } else if (dir == 'f') {
	if (dataVec[xp + 3 + rowLen * yp] != 'X') Xflag = false;
	if (dataVec[xp + 3 + rowLen * yp] != 'S') Sflag = false;
	if (dataVec[xp + 2 + rowLen * (yp + 1)] != 'M') Xflag = false;
	if (dataVec[xp + 2 + rowLen * (yp + 1)] != 'A') Sflag = false;
	if (dataVec[xp + 1 + rowLen * (yp + 2)] != 'A') Xflag = false;
	if (dataVec[xp + 1 + rowLen * (yp + 2)] != 'M') Sflag = false;
	if (dataVec[xp + rowLen * (yp + 3)] != 'S') Xflag = false;
	if (dataVec[xp + rowLen * (yp + 3)] != 'X') Sflag = false;
    } else if (dir == 'v') {
	if (dataVec[xp + rowLen * yp] != 'X') Xflag = false;
	if (dataVec[xp + rowLen * yp] != 'S') Sflag = false;
	if (dataVec[xp + rowLen * (yp + 1)] != 'M') Xflag = false;
	if (dataVec[xp + rowLen * (yp + 1)] != 'A') Sflag = false;
	if (dataVec[xp + rowLen * (yp + 2)] != 'A') Xflag = false;
	if (dataVec[xp + rowLen * (yp + 2)] != 'M') Sflag = false;
	if (dataVec[xp + rowLen * (yp + 3)] != 'S') Xflag = false;
	if (dataVec[xp + rowLen * (yp + 3)] != 'X') Sflag = false;
    }
    return Xflag || Sflag;
} // isXMAS


int findXMAS(vector<char>& dataVec, int rowLen, int colLen) {
    int currentSum = 0;
    for (int i = 0; i < rowLen-3; i++) { // horizontal
	for (int j = 0; j < colLen; j++) {
	    if (isXMAS(i, j, 'h', dataVec, rowLen)) currentSum++;
	}
    }
    for (int i = 0; i < rowLen; i++) { // vertical
	for (int j = 0; j < colLen-3; j++) {
	    if (isXMAS(i, j, 'v', dataVec, rowLen)) currentSum++;
	}
    }
    for (int i = 0; i < rowLen-3; i++) { // forward slash
	for (int j = 0; j < colLen-3; j++) {
	    if (isXMAS(i, j, 'f', dataVec, rowLen)) currentSum++;
	}
    }
    for (int i = 0; i < rowLen-3; i++) { // backward slash
	for (int j = 0; j < colLen-3; j++) {
	    if (isXMAS(i, j, 'b', dataVec, rowLen)) currentSum++;
	}
    }
    return currentSum;
} // findXMAS

bool isX_MAS(int xp, int yp, vector<char>& dataVec, int rowLen) {
    bool uFlag = true;
    bool rFlag = true;
    bool dFlag = true;
    bool lFlag = true;
    if (dataVec[xp + 1 + rowLen * (yp + 1)] != 'A') return false;
    if (dataVec[xp + rowLen * yp] != 'M') {uFlag = false; lFlag = false;}
    if (dataVec[xp + rowLen * yp] != 'S') {dFlag = false; rFlag = false;}
    if (dataVec[xp + 2 + rowLen * yp] != 'M') {uFlag = false; rFlag = false;}
    if (dataVec[xp + 2 + rowLen * yp] != 'S') {dFlag = false; lFlag = false;}
    if (dataVec[xp + rowLen * (yp + 2)] != 'M') {dFlag = false; lFlag = false;}
    if (dataVec[xp + rowLen * (yp + 2)] != 'S') {uFlag = false; rFlag = false;}
    if (dataVec[xp + 2 + rowLen * (yp + 2)] != 'M') {dFlag = false; rFlag = false;}
    if (dataVec[xp + 2 + rowLen * (yp + 2)] != 'S') {uFlag = false; lFlag = false;}
    return uFlag || rFlag || dFlag || lFlag;
} // isX_MAS

int findX_MAS(vector<char>& dataVec, int rowLen, int colLen) {
    int currentSum = 0;
    for (int i = 0; i < rowLen - 2; i++) { // horizontal
	for (int j = 0; j < colLen - 2; j++) {
	    if (isX_MAS(i, j, dataVec, rowLen)) currentSum++;
	}
    }
    return currentSum;
} // findX_MAS

int main() {
    vector<char> dataVec;
    auto [rowLen, colLen] = readArray("input.txt", dataVec);
    int totalSum = findX_MAS(dataVec, rowLen, colLen);

    cout << "The number of X-MASes is: " << totalSum << endl;
    return 0;
} // main
