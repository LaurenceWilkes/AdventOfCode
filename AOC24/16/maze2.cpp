#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <fstream>
#include <queue>

using namespace std;

using Point = pair<int, int>;
using Vertex = pair<Point, Point>;  // (position, direction)
using Maze = vector<vector<char>>;

const vector<Point> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void readFile(string filename, Maze &maze, Point &startPoint, Point &endPoint) {
    ifstream file(filename);
    string line;
    int y = 0;
    while (getline(file, line)) {
        vector<char> row;
        for (int x = 0; x < (int) line.size(); x++) {
            row.push_back(line[x]);
            if (line[x] == 'S') startPoint = {x, y};
            if (line[x] == 'E') endPoint = {x, y};
        }
        maze.push_back(row);
        y++;
    }
}

bool isWalkable(Point p, Maze &maze) {
    int rows = maze.size(), cols = maze[0].size();
    if (0 <= p.first && p.first < cols && 0 <= p.second && p.second < rows) {
        char c = maze[p.second][p.first];
        return c != '#';
    }
    return false;
}

pair<int, int> dijkstra(Point start, Point end, Maze &maze) {
    map<Vertex, int> dist;
    map<Vertex, vector<Vertex>> prev;
    set<Vertex> visited;

    using State = pair<int, Vertex>;
    priority_queue<State, vector<State>, greater<State>> pq;

    Vertex startV = {start, {1, 0}};
    dist[startV] = 0;
    pq.push({0, startV});

    int bestDist;
    vector<Vertex> ends;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (visited.count(u)) continue;
        visited.insert(u);

        if (maze[u.first.second][u.first.first] == 'E') {
	    if (ends.size() == 0) { // There can be at most two end points for my input
		bestDist = d; 
		ends.push_back(u);
	    } else {
		if (d == bestDist) {ends.push_back(u);}
		break;
	    }
        }

        Point pos = u.first;
        Point dir = u.second;

        // forward
        Point np = {pos.first + dir.first, pos.second + dir.second};
        if (isWalkable(np, maze)) {
            Vertex v = {np, dir};
            int nd = d + 1;
            if (!dist.count(v) || nd < dist[v]) {
                dist[v] = nd;
		prev[v].clear();
		prev[v].push_back(u);
                pq.push({nd, v});
            } else if (nd == dist[v]) {
		prev[v].push_back(u);
            }
        }

        // left turn
        Point left = {-dir.second, dir.first};
        Vertex lv = {pos, left};
        int nd = d + 1000;
        if (!dist.count(lv) || nd < dist[lv]) {
            dist[lv] = nd;
	    prev[lv].clear();
	    prev[lv].push_back(u);
            pq.push({nd, lv});
        } else if (nd == dist[lv]) {
            prev[lv].push_back(u);
        }

        // right turn
        Point right = {dir.second, -dir.first};
        Vertex rv = {pos, right};
        nd = d + 1000;
        if (!dist.count(rv) || nd < dist[rv]) {
            dist[rv] = nd;
	    prev[rv].clear();
	    prev[rv].push_back(u);
            pq.push({nd, rv});
        } else if (nd == dist[rv]) {
            prev[rv].push_back(u);
        }
    }

    // Part 2: Count points in optimal paths
   
    set<Point> tiles;
    queue<Vertex> check;

    for (const auto& v : ends) {
        check.push(v);
    }
    while (!check.empty()) {
	Vertex current = check.front();
        tiles.insert(current.first);
	for (const auto& vl : prev[current]) {
	    check.push(vl);
	}
	check.pop();
    }
    return make_pair(bestDist, tiles.size());
}

int main() {
    Maze maze;
    Point S, E;
    readFile("input.txt", maze, S, E);
    auto [bestDist, noTiles] = dijkstra(S, E, maze);
    cout << "Best distance is: " << bestDist << endl;
    cout << "Number of tiles on best paths: " << noTiles << endl;
}
