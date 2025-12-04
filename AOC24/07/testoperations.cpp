// I solved the problem this morning but wanted to see ChatGPTs recursive solution.
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


// Recursive function to check if target can be achieved with the given operators
bool canFormTarget(const vector<int>& numbers, int index, long long currentValue, long long target) {
    if (index == numbers.size()) {
        return currentValue == target;
    }

    // Try addition
    if (canFormTarget(numbers, index + 1, currentValue + numbers[index], target)) {
        return true;
    }

    // Try multiplication
    if (canFormTarget(numbers, index + 1, currentValue * numbers[index], target)) {
        return true;
    }

    // Try concatenation (||)
    long long concatenatedValue = stoll(to_string(currentValue) + to_string(numbers[index]));
    if (canFormTarget(numbers, index + 1, concatenatedValue, target)) {
        return true;
    }

    return false;
}

// Function to parse input and calculate the total calibration result
long long totalCalibrationResult(const string& filename) {
    long long totalSum = 0;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return -1;
    }

    string line;
    while (getline(inputFile, line)) {
        // Split the equation into target and numbers
        stringstream ss(line);
        string targetStr, numbersStr;
        getline(ss, targetStr, ':');
        getline(ss, numbersStr);

        long long target = stoll(targetStr);
        stringstream numStream(numbersStr);
        vector<int> numbers;
        int num;

        while (numStream >> num) {
            numbers.push_back(num);
        }

        // Check if the equation can be made true
        if (canFormTarget(numbers, 1, numbers[0], target)) {
            totalSum += target;
        }
    }

    inputFile.close();
    return totalSum;
}

int main() {
    string filename = "input.txt";

    long long result = totalCalibrationResult(filename);
    if (result != -1) {
        cout << "Total calibration result: " << result << endl;
    }

    return 0;
}

