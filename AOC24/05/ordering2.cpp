#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

void readFile(string filename, set<pair<int, int>>& order, vector<vector<int>>& pageSets) {
    ifstream file(filename);

    string line;

    while (getline(file, line)) {
	if (!line.empty() && line[2] == '|') {
            int firstNo = (line[0] - '0') * 10 + (line[1] - '0');
	    int secondNo = (line[3] - '0') * 10 + (line[4] - '0');
	    order.insert(make_pair(firstNo, secondNo));
        } else if (!line.empty()) {
	    stringstream ss(line);
	    string substring;

	    vector<int> lineVec;

	    while (getline(ss, substring, ',')) {
		lineVec.push_back(stoi(substring));
	    }
	    pageSets.push_back(lineVec);
        }
    }

} // readFile

bool isValid(const vector<int>& pages, set<pair<int, int>>& order) {
    for (int i = 0; i < pages.size() - 1; i++) {
	for (int j = i+1; j < pages.size(); j++) {
	    if (order.count(make_pair(pages[j], pages[i])) > 0) {
		return false;
	    }
	}
    }
    return true;
} // isValid

int main() {
    set<pair<int, int>> order;
    vector<vector<int>> pageSets;

    readFile("input.txt", order, pageSets);

    int sumMiddles = 0;
    for (vector<int>& pages : pageSets) {
        if (isValid(pages, order)) {
            sumMiddles += pages[pages.size() / 2];
        }
    }

    cout << "The middle page sum of valid pages is: " << sumMiddles << endl;

    return 0;
} // main
