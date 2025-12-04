#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <map>
#include <set>

using namespace std;

map<string, tuple<int, string, int, string, int, string, int, string>> decodeInput(const string& filename) {
    map<string, tuple<int, string, int, string, int, string, int, string>> Contains; // map from bag to those it contains

    ifstream file(filename); // open file

    regex patternEmpty(R"((\w+ \w+) bags contain no other bags\.)");
    regex patternFour(R"((\w+ \w+) bags contain (\d+) (\w+ \w+) bags?, (\d+) (\w+ \w+) bags?, (\d+) (\w+ \w+) bags?, (\d+) (\w+ \w+) bags?\.)");
    regex patternThree(R"((\w+ \w+) bags contain (\d+) (\w+ \w+) bags?, (\d+) (\w+ \w+) bags?, (\d+) (\w+ \w+) bags?\.)");
    regex patternTwo(R"((\w+ \w+) bags contain (\d+) (\w+ \w+) bags?, (\d+) (\w+ \w+) bags?\.)");
    regex patternOne(R"((\w+ \w+) bags contain (\d+) (\w+ \w+) bags?\.)");

    smatch match;

    string line;
	
    while (getline(file, line)) {
	if (regex_match(line, match, patternEmpty)) {
	    string initialbag = match[1];
	    Contains[initialbag] = make_tuple(0, "", 0, "", 0, "", 0, "");
	} else if (regex_match(line, match, patternFour)) {
	    string initialbag = match[1];
	    int num1 = stoi(match[2]);
	    string bag1 = match[3]; 
	    int num2 = stoi(match[4]);
	    string bag2 = match[5];
	    int num3 = stoi(match[6]);
	    string bag3 = match[7];
	    int num4 = stoi(match[8]);
	    string bag4 = match[9];
	    Contains[initialbag] = make_tuple(num1, bag1, num2, bag2, num3, bag3, num4, bag4);
	} else if (regex_match(line, match, patternThree)) {
	    string initialbag = match[1];
	    int num1 = stoi(match[2]);
	    string bag1 = match[3]; 
	    int num2 = stoi(match[4]);
	    string bag2 = match[5];
	    int num3 = stoi(match[6]);
	    string bag3 = match[7];
	    Contains[initialbag] = make_tuple(num1, bag1, num2, bag2, num3, bag3, 0, "");
	} else if (regex_match(line, match, patternTwo)) {
	    string initialbag = match[1];
	    int num1 = stoi(match[2]);
	    string bag1 = match[3]; 
	    int num2 = stoi(match[4]);
	    string bag2 = match[5];
	    Contains[initialbag] = make_tuple(num1, bag1, num2, bag2, 0, "", 0, "");
	} else if (regex_match(line, match, patternOne)) {
	    string initialbag = match[1];
	    int num1 = stoi(match[2]);
	    string bag1 = match[3]; 
	    Contains[initialbag] = make_tuple(num1, bag1, 0, "", 0, "", 0, "");
	} else {
	    cout << "Something has gone wrong; no match found." << endl;
	} 
    }

    file.close();

    return Contains;

} // decodeInput

int findParents(const map<string, tuple<int, string, int, string, int, string, int, string>>& Contains) {
    string child = "shiny gold";

    set<string> conts;

    map<string, bool> Checked;
    set<string> tocheck({child});

    string checking;

    while (!tocheck.empty()) {
	checking = *(tocheck.begin());
	for (auto const& element : Contains) {
	    if (!Checked[element.first]) {
		auto [num1, bag1, num2, bag2, num3, bag3, num4, bag4] = element.second;

		if (checking == bag1 || checking == bag2 || checking == bag3 || checking == bag4) {
		    conts.insert(element.first);
		    tocheck.insert(element.first);
		} 
	    }
	}
	tocheck.erase(checking);
	Checked[checking] = true;
    }
    return conts.size();    
} // findParents

int findChildren(string parent, map<string, int>& memomap, const map<string, tuple<int, string, int, string, int, string, int, string>>& Contains) {
    if (parent.empty()) {
       return 0;
    } 

    if (memomap[parent] != 0) {
       return memomap[parent];
    } 

    int num1 = get<0>(Contains.at(parent));
    string bag1 = get<1>(Contains.at(parent));
    int num2 = get<2>(Contains.at(parent));
    string bag2 = get<3>(Contains.at(parent));
    int num3 = get<4>(Contains.at(parent));
    string bag3 = get<5>(Contains.at(parent));
    int num4 = get<6>(Contains.at(parent));
    string bag4 = get<7>(Contains.at(parent));	

    if (num1 == 0) {
        return 1;
    }

    int tempval = 1 + num1 * findChildren(bag1, memomap, Contains) +
		      num2 * findChildren(bag2, memomap, Contains) +
		      num3 * findChildren(bag3, memomap, Contains) +
		      num4 * findChildren(bag4, memomap, Contains);
    memomap[parent] = tempval;
    return memomap[parent];
}

int main() {
    map<string, tuple<int, string, int, string, int, string, int, string>> Contains = decodeInput("input.txt");

    int parents = findParents(Contains);
    map<string, int> memomap;
    int children = findChildren("shiny gold", memomap, Contains);

    cout << "The number of parents is: " << parents << endl;
    cout << "The number of children is: " << children - 1 << endl;

    return 0;
} // main

