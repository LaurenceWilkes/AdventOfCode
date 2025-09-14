#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <cmath>
#include <set>

using namespace std;

// Vertex type
using Point = pair<int, int>;
using Vertex = pair<Point, Point>;
using Maze = vector<vector<char>>;

const vector<Point> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

void printVert(Vertex vert) {
    cout << vert.first.first << " " << vert.first.second << " - " << vert.second.first << " " << vert.second.second << endl;
} // printVert

void printPoint(Point pt) {
    cout << pt.first << " " << pt.second << endl;
} // printPoint

void readFile(string filename, Maze& maze, Point& startPoint, Point& endPoint) {
    ifstream file(filename);
    string line;
    int i = 0, j = 0;
    while (getline(file, line)) {
	vector<char> current;
	current.clear();
	for (char c : line) {
	    current.push_back(c);
	    if (c == 'S') {
	        startPoint = make_pair(i, j);
	    } else if (c == 'E') {
		endPoint = make_pair(i, j);
	    }
	    i++;
	}
	maze.push_back(current);
	i = 0;
	j++;
    }
} // readFile

char charOf(Point& pt, Maze& maze) {
    return maze[pt.second][pt.first];
} // charOf

bool isWalkable(Point& pt, Maze& maze) {
    int rows = maze.size(), cols = maze[0].size();
    if (0 <= pt.first && pt.first < cols && 0 <= pt.second && pt.second < rows) {
	char cell = maze[pt.second][pt.first];
	return cell == '.' || cell == 'S' || cell == 'E';
    }
    return false;
}

void updateDistances(Vertex& vert, map<Vertex, int>& distance, set<Vertex>& visited, set<Vertex>& unvisited, Maze& maze) {
    Point current = vert.first;

    Point dir = vert.second;
    Point leftDir = {dir.second, -dir.first};
    Point rightDir = {-dir.second, dir.first};

    vector<pair<Vertex, int>> out;
    int trundle = 1;
    bool breakFlag = false;
    while (!breakFlag) {
	Point fp = {current.first + dir.first, current.second + dir.second};
	Point lp = {current.first + leftDir.first, current.second + leftDir.second};
	Point rp = {current.first + rightDir.first, current.second + rightDir.second};
	if (isWalkable(lp, maze)) {
	    breakFlag = true;
	    Vertex nextVert = make_pair(current, leftDir);
	    if (visited.count(nextVert) == 0) unvisited.insert(nextVert);
	    if (distance.count(nextVert) == 0 || distance[vert] + 999 + trundle < distance[nextVert]) {
		Vertex adjVert = make_pair(current, leftDir);
		distance[adjVert] = distance[vert] + 999 + trundle;
	    }
	}
	if (isWalkable(rp, maze)) {
	    breakFlag = true;
	    Vertex nextVert = make_pair(current, rightDir);
	    if (visited.count(nextVert) == 0) unvisited.insert(nextVert);
	    if (distance.count(nextVert) == 0 || distance[vert] + 999 + trundle < distance[nextVert]) {
		Vertex adjVert = make_pair(current, rightDir);
		distance[adjVert] = distance[vert] + 999 + trundle;
	    }
	}
	if (breakFlag) {
	    if (isWalkable(fp, maze)) {
		Vertex nextVert = make_pair(fp, dir);
		if (visited.count(nextVert) == 0) unvisited.insert(nextVert);
		if (distance.count(nextVert) == 0 || distance[vert] + trundle < distance[nextVert]) {
		    distance[nextVert] = distance[vert] + trundle;
		}
	    }
	    break;
	} else {
	    if (isWalkable(fp, maze)) {
		trundle++;
		current = fp;
	    } else {
	        break;
	    }
	}
    }
} // nextNodes

Vertex findNext(map<Vertex, int>& distance, set<Vertex>& unvisited) {
    bool firstFlag = true;
    int md = 0;
    Vertex nextVert;
    for (const auto& vert : unvisited) {
	if (firstFlag) {
	    md = distance[vert];
	    nextVert = vert;
	    firstFlag = false;
	} else if (distance[vert] < md) {
	    md = distance[vert];
	    nextVert = vert;
	}
    }
    return nextVert;
} // findNext

int dijkstra(Point& startPoint, Maze& maze) {
    map<Vertex, int> distance;
    set<Vertex> visited;
    set<Vertex> unvisited;
    Vertex currentVert = make_pair(startPoint, make_pair(1, 0));
    distance[currentVert] = 0;
    long loops = 0;
    while (charOf(currentVert.first, maze) != 'E') {
	loops++;
	updateDistances(currentVert, distance, visited, unvisited, maze);
	visited.insert(currentVert);
	unvisited.erase(currentVert);
	currentVert = findNext(distance, visited);
    }
    cout << "distance: " << distance.size() << endl;
    cout << "loops: " << loops << endl;
    return distance[currentVert];
} // dijkstra

int main() {
    vector<vector<char>> maze;
    Point startPoint, endPoint;
    readFile("testinput.txt", maze, startPoint, endPoint);

    int dist = dijkstra(startPoint, maze);

    cout << "The total distance is: " << dist << endl;
    return 0;
} // main
