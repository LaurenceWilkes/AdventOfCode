#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <set>


using namespace std;

pair<int, int> readGrid(string filename, map<char, vector<pair<int, int>>>& charLocs) {
    ifstream file(filename);

    string line;

    int i = 0;
    int j = 0;
    while (getline(file, line)) {
	j = 0;
        for (char c : line) {
	    if (c != '.') {
	        charLocs[c].push_back(make_pair(i, j));
	    }
	    j++;
        }
	i++;
    }
    return make_pair(i, j);
} // readGrid

void extend(set<pair<int, int>>& antinodes, const pair<int, int>& va, const pair<int, int>& vb, int x, int y) {
    int g1 = vb.first - va.first;
    int g2 = vb.second - va.second;

    int m1 = va.first;
    int m2 = va.second;
    while (0 <= m1 && m1 < x && 0 <= m2 && m2 < y) {
	antinodes.insert(make_pair(m1, m2));
	m1 -= g1;
	m2 -= g2;
    }

    int p1 = vb.first;
    int p2 = vb.second;
    while (0 <= p1 && p1 < x && 0 <= p2 && p2 < y) {
	antinodes.insert(make_pair(p1, p2));
	p1 += g1;
	p2 += g2;
    }
} // extend

//void extend(set<pair<int, int>>& antinodes, const pair<int, int>& va, const pair<int, int>& vb, int x, int y) {
//    int g1 = vb.first - va.first;
//    int g2 = vb.second - va.second;
//
//    int m1 = va.first - g1;
//    int m2 = va.second - g2;
//    if (0 <= m1 && m1 < x && 0 <= m2 && m2 < y) {
//	antinodes.insert(make_pair(m1, m2));
//    }
//
//    int p1 = vb.first + g1;
//    int p2 = vb.second + g2;
//    if (0 <= p1 && p1 < x && 0 <= p2 && p2 < y) {
//	antinodes.insert(make_pair(p1, p2));
//    }
//} // extend

void findAntinodes(set<pair<int, int>>& antinodes, const vector<pair<int, int>>& positions, int x, int y) {
    for (int a = 0; a < positions.size() - 1; a++) {
        for (int b = a + 1; b < positions.size(); b++) {
	    extend(antinodes, positions.at(a), positions.at(b), x, y);
        }
    }
} // findAntinodes

int countAllAntinodes(set<pair<int, int>>& antinodes, const map<char, vector<pair<int, int>>>& charLocs, int x, int y) {
    for (auto const& cl : charLocs) {
	findAntinodes(antinodes, cl.second, x, y);
    }
    return antinodes.size();
} // countAllAntinodes

int main() {
    map<char, vector<pair<int, int>>> charLocs;
    set<pair<int, int>> antinodes;
    auto [x, y] = readGrid("input.txt", charLocs);

    int nodes = countAllAntinodes(antinodes, charLocs, x, y);

    cout << "The number of antinodes is: " << nodes << endl;
    return 0;
} // main
