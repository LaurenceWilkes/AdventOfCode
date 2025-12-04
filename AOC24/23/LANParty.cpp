#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <tuple>
#include <algorithm>

using namespace std;

void readInput(const string& filename, vector<string>& vertices, set<pair<string, string>>& edges) {
    ifstream f(filename);

    set<string> vertex_set;  
    string line;
    while (getline(f, line)) {
        auto dash = line.find('-');
        string u = line.substr(0, dash);
        string v = line.substr(dash + 1);

        if (u > v) swap(u, v);
        edges.insert({u, v});

        vertex_set.insert(u);
        vertex_set.insert(v);
    }

    vertices.assign(vertex_set.begin(), vertex_set.end());
}

vector<tuple<string, string, string>> getTriples(const vector<string>& vertices, const set<pair<string, string>>& edges) {
    vector<tuple<string, string, string>> triples;
    for (auto it = edges.begin(); it != edges.end(); it++) {
	auto [e1, e2] = *it;
        for (auto v : vertices) {
	    if (v >= e1) continue;
            pair<string, string> a = {v, e1};
            pair<string, string> b = {v, e2};
	    tuple<string, string, string> triple = {v, e1, e2};
	    if (edges.count(a) && edges.count(b)) triples.push_back(triple);
        }
    }
    return triples;
} // getTriples


int noTs(vector<tuple<string, string, string>> triples) {
    int count = 0;
    for (auto [a, b, c] : triples) {
//	cout << a << " " << b << " " << c;
	if (a[0] == 't' || b[0] == 't' || c[0] == 't') {
	    count++;
//	    cout << " -- contains t";
	}
//	cout << endl;
    }
    return count;
}


vector<string> neighbours(const vector<string>& vertices, const set<pair<string, string>>& edges, string v) {
    vector<string> Nv;  
    for (auto vo : vertices) {
       pair<string, string> ed = v < vo ? make_pair(v, vo) : make_pair(vo, v);
       if (edges.count(ed)) {Nv.push_back(vo);}
    } 
    sort(Nv.begin(), Nv.end());
    return Nv;
} // neighbours

bool formsClique(const set<pair<string, string>>& edges, const vector<string>& c, string v) {
    for (auto cv : c) {
	pair<string, string> ed = v < cv ? make_pair(v, cv) : make_pair(cv, v);
	if (!edges.count(ed)) {return false;}
    }
    return true;
}

vector<vector<string>> listMaximalCliques(const vector<string>& vertices, const set<pair<string, string>>& edges) {
    vector<vector<string>> prevCliques = {{vertices[0]}};
    vector<vector<string>> cliques;
    for (int i = 1; i < vertices.size(); i++) {
	cliques.clear();
	string v = vertices[i];
	auto Nv = neighbours(vertices, edges, v);
	for (auto c : prevCliques) {
	    vector<string> k = c;
	    if (formsClique(edges, c, v)) {
		k.push_back(v);
		sort(k.begin(), k.end());
		cliques.push_back(k);
	    } else {
	        cliques.push_back(k);
		int i = 0, j = 0;
		vector<string> kv;
		while (i < k.size() && j < Nv.size()) {
		    if (k[i] == Nv[j]) {
			kv.push_back(k[i]);
			i++;
			j++;
		    } else if (k[i] < Nv[j]) {
		        i++;
		    } else {
		        j++;
		    }
		}
		kv.push_back(v);
		sort(kv.begin(), kv.end());
	        if (find(cliques.begin(), cliques.end(), kv) == cliques.end()) cliques.push_back(kv);
	    }
	}
	prevCliques = cliques;
    }
    return cliques;
} // listMaximalCliques

string createString(vector<string>& mc) {
    string outString;
    for (auto v : mc) {
	outString += v;
	outString += ",";
    }
    outString.pop_back();
    return outString;
} // createString


int main() {
    // part 1
    vector<string> vertices;
    set<pair<string, string>> edges;
    readInput("input.txt", vertices, edges);
    vector<tuple<string, string, string>> triples = getTriples(vertices, edges);
    int outTriples = noTs(triples);
    cout << outTriples << endl;

    // part 2 -- there are probably much more careful ways of doing this...
    vector<vector<string>> maxCliques = listMaximalCliques(vertices, edges);
    int currentMaxSize = 0;
    vector<string> maximumClique;
    for (const auto& mc : maxCliques) {
	if (mc.size() > currentMaxSize) {
	    maximumClique = mc;
	    currentMaxSize = mc.size();
	} 
    }

    string outString = createString(maximumClique);

    cout << "Password: " << outString << endl;

    return 0;
} // main
