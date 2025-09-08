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

double simShip(const vector<pair<char, int>>& inst) {
    pair<double, double> pos = {0., 0.};
    pair<double, double> wpos = {10., 1.};
    for (auto p : inst) {
	if (p.first == 'N') {
	    wpos.second += p.second; 
	} else if (p.first == 'S') {
	    wpos.second -= p.second;
	} else if (p.first == 'E') {
	    wpos.first += p.second;
	} else if (p.first == 'W') {
	    wpos.first -= p.second;
	} else if (p.first == 'L') {
	    double x = wpos.first - pos.first;
	    double y = wpos.second - pos.second;
	    double c = cos(PI * p.second / 180.);
	    double s = sin(PI * p.second / 180.);
	    wpos.first = pos.first + c * x - s * y;
	    wpos.second = pos.second + s * x + c * y;
	} else if (p.first == 'R') {
	    double x = wpos.first - pos.first;
	    double y = wpos.second - pos.second;
	    double c = cos(PI * p.second / 180.);
	    double s = sin(PI * p.second / 180.);
	    wpos.first = pos.first + c * x + s * y;
	    wpos.second = pos.second - s * x + c * y;
	} else {
	    double x = wpos.first - pos.first;
	    double y = wpos.second - pos.second;
	    pos.first += p.second * x;
	    pos.second += p.second * y;
	    wpos.first += p.second * x;
	    wpos.second += p.second * y;
	}
    }
    return abs(pos.first) + abs(pos.second);
}

int main() {
    auto inst = readFile("input.txt");

    cout << "Manhattan dist: " << simShip(inst) << endl;

    return 0;
} // main

    

