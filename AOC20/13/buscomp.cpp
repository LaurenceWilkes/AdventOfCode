#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <array>



using namespace std;

pair<vector<int>, vector<long long>> readFile(string filename) {
    ifstream file(filename);

    string line;

    getline(file, line); // first getline

    int arrtime = stoi(line);

    getline(file, line); // second getline

    vector<long long> numbers;
    vector<int> disps;
    stringstream ss(line);
    string token;

    int disp = 0;

    while (getline(ss, token, ',')) {
	if (token != "x") {
	    try {
		numbers.push_back(stoll(token));  // Convert to integer and add to vector
		disps.push_back(disp);
	    } catch (const invalid_argument& e) {
		cerr << "Invalid integer: " << token << endl;
	    } catch (const out_of_range& e) {
		cerr << "Integer out of range: " << token << endl;
	    }
	}
	disp++;
    }

    return make_pair(disps, numbers);
} // readFile



int main() {
    auto [disps, numbers]  = readFile("input.txt");

    long long prod = 1;

    for (auto el : numbers) {
        prod *= el;
    }

    cout << "Next 0 is: " << prod << endl;

    long long sum = 0;

    for (int i = 0; i < disps.size(); i++) {
	if (disps[i] != 0) {
	    long long pprod = (prod / numbers[i]) % numbers[i];
	    long long inv;
	    for (int j = 1; j <= numbers[i]; j++) {
		if ((pprod * j) % numbers[i] == 1) {
		    inv = j;
		    break;
		}
	    }
	    sum -= (prod / numbers[i]) * (disps[i]) * inv;
	}
    }

    while (sum < 0) {
	sum += prod;
    }

    for (auto i : numbers) {
        cout << sum % i << " ";
    }
    cout << endl;


    cout << "Estimated solution is: " << sum << endl;

    return 0;
} // main





