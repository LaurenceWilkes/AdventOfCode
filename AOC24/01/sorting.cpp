#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>

using namespace std;

pair<vector<int>, vector<int>> readData(string filename) {
    ifstream file(filename);
    vector<int> firstVector, secondVector;

    string line;

    while (getline(file, line)) {
	istringstream iss(line);
	string firstNumber, secondNumber;
	iss >> firstNumber >> secondNumber;
	firstVector.push_back(stoi(firstNumber));
	secondVector.push_back(stoi(secondNumber));
    }

    return make_pair(firstVector, secondVector);
} // readData

vector<int> assessData(pair<vector<int>, vector<int>>& data) {
    auto [firstVector, secondVector] = data;

    sort(firstVector.begin(), firstVector.end());
    sort(secondVector.begin(), secondVector.end());

    vector<int> outVector;

    for (int i = 0; i < firstVector.size(); i++) {
	outVector.push_back(abs(firstVector[i] - secondVector[i]));
    }

    return outVector;
} // assessData

int similarityScore(const pair<vector<int>, vector<int>>& data) {
    auto [firstVector, secondVector] = data;

    map<int, int> appears;

    for (auto& i : secondVector) {
        appears[i]++;
    }

    int sum = 0;
    for (auto& i : firstVector) {
        sum += i * appears[i];
    }

    return sum;
} // similarityScore

int main() {
    pair<vector<int>, vector<int>> data = readData("input.txt");
    vector<int> outVector = assessData(data);

    int sum = reduce(outVector.begin(), outVector.end());

    cout << "The total distance is: " << sum << endl;

    int score = similarityScore(data);

    cout << "The score is: " << score << endl;
    return 0;
} // main
