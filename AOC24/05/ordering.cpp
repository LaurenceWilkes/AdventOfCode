#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <sstream>

using namespace std;

void readFile(string filename, map<int, set<int>>& lessThan, map<int, set<int>>& greaterThan, vector<vector<int>>& pageSets) {
    ifstream file(filename);

    string line;

    while (getline(file, line)) {
	if (!line.empty() && line[2] == '|') {
            int firstNo = (line[0] - '0') * 10 + (line[1] - '0');
	    int secondNo = (line[3] - '0') * 10 + (line[4] - '0');
	    lessThan[secondNo].insert(firstNo);
	    greaterThan[firstNo].insert(secondNo);
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

bool isValid(const vector<int>& pages, const map<int, set<int>>& lessThan, const map<int, set<int>>& greaterThan) {
    for (int i = 0; i < pages.size() - 1; i++) {
	for (int j = i+1; j < pages.size(); j++) {
	    if (lessThan.count(pages[i]) > 0 && (lessThan.at(pages[i])).count(pages[j]) > 0) {
		return false;
	    } else if (greaterThan.count(pages[j]) > 0 && (greaterThan.at(pages[j])).count(pages[i]) > 0) {
	        return false;
	    }
	}
    }
    return true;
} // isValid

void reorder(vector<int>& pages, const map<int, set<int>>& lessThan, const map<int, set<int>>& greaterThan) {
    vector<int> toDo = pages;
    vector<int> seen;
    while (toDo.size() > 0) {
	int remInt;
	for (int i = 0; i < toDo.size(); i++) {
	    bool badFlag = false;
	    for (int j = 0; j < toDo.size(); j++) {
		if (j != i) {
		    if (lessThan.count(toDo[i]) > 0 && (lessThan.at(toDo[i])).count(toDo[j]) > 0) {
			badFlag = true;
			break;
		    }
		}
	    }
	    if (!badFlag) {
	        remInt = i;
		break;
	    }
	}
	seen.push_back(toDo[remInt]);
	toDo.erase(toDo.begin()+remInt);
    }
    pages = seen;
} // reorder

int main() {
    map<int, set<int>> lessThan;
    map<int, set<int>> greaterThan;
    vector<vector<int>> pageSets;

    readFile("input.txt", lessThan, greaterThan, pageSets);

    int sumMiddles = 0;
    for (vector<int>& pages : pageSets) {
        if (!isValid(pages, lessThan, greaterThan)) {
	    reorder(pages, lessThan, greaterThan);
            sumMiddles += pages[pages.size() / 2];
        }
    }

    cout << "The middle page sum of valid pages is: " << sumMiddles << endl;

    return 0;
} // main
