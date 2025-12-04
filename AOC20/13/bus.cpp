#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>


using namespace std;

pair<int, vector<int>> readFile(string filename) {
    ifstream file(filename);

    string line;

    getline(file, line); // first getline

    int arrtime = stoi(line);

    getline(file, line); // second getline

    vector<int> numbers;
    stringstream ss(line);
    string token;
    
    while (getline(ss, token, ',')) {
	if (token != "X") {
	    try {
		numbers.push_back(stoi(token));  // Convert to integer and add to vector
	    } catch (const invalid_argument& e) {
		cerr << "Invalid integer: " << token << endl;
	    } catch (const out_of_range& e) {
		cerr << "Integer out of range: " << token << endl;
	    }
	}
    } 

    return make_pair(arrtime, numbers);
} // readFile


int earliestBus(const pair<int, vector<int>>& inp) {
    int arrtime = inp.first;
    vector<int> busses = inp.second;

    int stime = arrtime; // works with the input...
    int sel = 0;
   
    for (int el : busses) {
        int nb = el - arrtime % el;
	if (nb < stime) {
	    sel = el;
	    stime = nb;
	} 
    }

    return sel * stime;
} // earliestBus

int main() {
    auto inp = readFile("input.txt");

    int eb = earliestBus(inp);

    cout << "The earliest bus sig is: " << eb << endl;
    return 0;
} // main





