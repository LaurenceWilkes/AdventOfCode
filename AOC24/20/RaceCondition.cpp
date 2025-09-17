#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

using Point = pair<int, int>;

void readInput(string filename, map<Point, char>& track, Point& start, Point& end) {
    ifstream f(filename);
    string line;
    int j = 0;
    while (getline(f, line)) {
        for (int i = 0; i < line.size(); i++) {
	    Point loc = {i, j};
	    if (line[i] == 'S') start = loc;
	    if (line[i] == 'E') end = loc;
            track[make_pair(i, j)] = line[i];
        }
	j++;
    }
} // readInput

map<Point, int> endDist(const map<Point, char>& track, Point start, Point end) {
    map<Point, int> dist;
    vector<Point> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    Point current = end;
    dist[end] = 0;
    Point prev = {-1, -1};
    while (current != start) {
	Point next = current;
	for (const auto& d : dirs) {
	    next.first = current.first + d.first; 
	    next.second = current.second + d.second;
	    if (track.at(next) != '#' && next != prev) break;
	}
	dist[next] = dist[current] + 1;
	prev = current;
	current = next;
    }
    return dist;
} // endDist

int cheatTracker(const map<Point, char>& track, const map<Point, int>& dist, Point start, Point end, int multiLen) {
    vector<Point> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int honestDist = dist.at(start);
    int currentDist = 0;
    int goodCheats = 0;

    Point current = start;
    Point prev = {-1, -1};
    while (current != end) {
	for (int i = -multiLen; i <= multiLen; i++) {
	    for (int j = -multiLen + abs(i); j <= multiLen - abs(i); j++) {
	        Point jumpPoint = {current.first + i, current.second + j};
		if (track.count(jumpPoint) && track.at(jumpPoint) != '#') {
		    int cheatDist = currentDist + abs(i) + abs(j) + dist.at(jumpPoint);
		    if (honestDist - cheatDist >= 100) {goodCheats++;}
		}
	    }
	}
	Point next;
	for (const auto& d : dirs) {
	    next.first = current.first + d.first; 
	    next.second = current.second + d.second;
	    if (track.at(next) != '#' && next != prev) break;
	}
	currentDist += 1;
	prev = current;
	current = next;
    }
    return goodCheats;
} // cheatTracker

int main() {
    map<Point, char> track;
    Point start;
    Point end;
    readInput("input.txt", track, start, end);
    map<Point, int> dists = endDist(track, start, end);
    int goodCheats = cheatTracker(track, dists, start, end, 2);
    cout << "Number of good 2-cheats is: " << goodCheats << endl;
    goodCheats = cheatTracker(track, dists, start, end, 20);
    cout << "Number of good 20-cheats is: " << goodCheats << endl;
    return 0;
} // main

