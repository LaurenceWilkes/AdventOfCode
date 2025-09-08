#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

const double PI = 3.141592653589793;

vector<pair<char, int>> readFile(string filename) {
    ifstream file(filename);

    string line;

    vector<pair<char, int>> inst;

    while (getline(file, line)) {
        inst.emplace_back(line[0], stoi(line.substr(1)));
    }
    return inst;
} // readFile

double simShip(vector<pair<char, int>> inst) {
    pair<double, double> pos = {0., 0.};
    double dir = 0.;
    for (auto p : inst) {
	if (p.first == 'N') {
	    pos.second += p.second; 
	} else if (p.first == 'S') {
	    pos.second -= p.second;
	} else if (p.first == 'E') {
	    pos.first += p.second;
	} else if (p.first == 'W') {
	    pos.first -= p.second;
	} else if (p.first == 'L') {
	    dir -= p.second;
	} else if (p.first == 'R') {
	    dir += p.second;
	} else {
	    pos.first += p.second * cos(PI * dir / 180.);
	    pos.second += - p.second * sin(PI * dir / 180.);
	}
    }
    return abs(pos.first) + abs(pos.second);
}

int main() {
    auto inst = readFile("input.txt");

    cout << "Manhattan dist: " << simShip(inst) << endl;

    return 0;
} // main

    

