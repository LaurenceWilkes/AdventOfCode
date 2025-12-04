#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

pair<int, vector<string>> commandType(const string& command) {
    int type = 0;
    vector<string> vectorCommand;
    istringstream ss(command);
    string word;
    while (ss >> word) {
        if (isdigit(word[0])) {
            vectorCommand.push_back(word);
        } else if (word[0] == '|') {
            vectorCommand.push_back(word);
	    type = 1;
        } else {
            vectorCommand.push_back(word.substr(1,1));
	    type = 2;
        }
    }
    return make_pair(type, vectorCommand);
} // commandType

pair<map<string, pair<int, vector<string>>>, vector<string>> readFile(string filename) {
    ifstream file(filename);

    string line;
    vector<string> messages;
    map<string, pair<int, vector<string>>> mem;
    regex pattern(R"((\d+):\s(.*))");
    smatch match;
    bool readingMessages = false;

    while (getline(file, line)) {
        if (readingMessages) {
	    messages.push_back(line);
        } else if (line.empty()) {
            readingMessages = true;
        } else {
	    if (regex_match(line, match, pattern)) {
		auto ctout = commandType(string(match[2]));
	        mem[string(match[1])] = ctout;
	    } else {
	        cout << "issue with matching" << endl;
	    }
        }
    }
    return make_pair(mem, messages);
} //readFile

vector<string> insertAt(int i, const vector<string>& insertVector, const vector<string>& current) {
    vector<string> newVector;
    int j = 0;
    for (auto e : current) {
	if (j == i) {
	    for (auto ie : insertVector) {
		newVector.push_back(ie);
	    }
	} else {
	    newVector.push_back(e);
	}
	j++;
    }
    return newVector;
} // insertAt


// void possibleMatches(vector<string> current, const map<string, pair<int, vector<string>>>& mem, vector<vector<string>>& matches, const int& maxlen) {
//     if (current.size() > maxlen) {
// 	cout << "Reached an endpoint";
//         return;
//     }
//     for (int i = 0; i < current.size(); i++) {
// 	if (!isalpha(current[i][0])) {
// 	    auto [type, vectorCommand] = mem.at(current[i]);
// 	    if (type == 1) {
// 		vector<string> vectorCommand1;
// 		vector<string> vectorCommand2;
// 		bool secondHalf = false;
// 		for (string str : vectorCommand) {
// 		    if (!secondHalf) {
// 		        if (str[0] != '|') {
// 		            vectorCommand1.push_back(str);
// 		        } else {
// 		            secondHalf = true;
// 		        }
// 		    } else {
// 		        vectorCommand2.push_back(str);
// 		    }
// 		}
// 	        vector<string> newCurrent1 = insertAt(i, vectorCommand1, current);
// 		vector<string> newCurrent2 = insertAt(i, vectorCommand2, current);
//
// 		possibleMatches(newCurrent1, mem, matches, maxlen);
// 		possibleMatches(newCurrent2, mem, matches, maxlen);
// 		return;
// 	    } else {
// 		vector<string> newCurrent = insertAt(i, vectorCommand, current);
//
// 		possibleMatches(newCurrent, mem, matches, maxlen);
// 		return;
// 	    }
// 	}
//     }
//     matches.push_back(current);
// } // possibleMatches

void possibleMatches(vector<string> current, const map<string, pair<int, vector<string>>>& mem, vector<vector<string>>& matches, const int& maxlen) {
    if (current.size() > maxlen) {
	cout << "Reached an endpoint";
        return;
    }
    for (int i = 0; i < current.size(); i++) {
	if (!isalpha(current[i][0])) {
	    if (current[i] == "8") {
		cout << "Found 8...";
	    }
	    if (current[i] == "11") {
		cout << "Found 11...";
	    }
	    auto [type, vectorCommand] = mem.at(current[i]);
	    if (type == 1) {
		vector<string> vectorCommand1;
		vector<string> vectorCommand2;
		bool secondHalf = false;
		for (string str : vectorCommand) {
		    if (!secondHalf) {
		        if (str[0] != '|') {
		            vectorCommand1.push_back(str);
		        } else {
		            secondHalf = true;
		        }
		    } else {
		        vectorCommand2.push_back(str);
		    }
		}
	        vector<string> newCurrent1 = insertAt(i, vectorCommand1, current);
		vector<string> newCurrent2 = insertAt(i, vectorCommand2, current);

		possibleMatches(newCurrent1, mem, matches, maxlen);
		possibleMatches(newCurrent2, mem, matches, maxlen);
		return;
	    } else {
		vector<string> newCurrent = insertAt(i, vectorCommand, current);

		possibleMatches(newCurrent, mem, matches, maxlen);
		return;
	    }
	}
    }
    matches.push_back(current);
} // possibleMatches


int main() {
    auto [mem, messages] = readFile("input.txt");

    cout << "File read" << endl;

    int maxlen = 0;
    for (auto message : messages) {
	if (message.size() > maxlen) {
	    maxlen = message.size();
	}
    }
    cout << "max length: " << maxlen << endl;

    string initialstring = "42";
    vector<vector<string>> matches;
    vector<string> current = {initialstring};
    possibleMatches(current, mem, matches, maxlen);

    cout << "The number of matches which match from " << initialstring << " is: " << matches.size() << endl;

//    for (auto m : matches) {
//        for (auto c : m) {
//            cout << c;
//        }
//	cout << endl;
//    }


    int sum = 0;

    for (auto mes : messages) {
	bool breakflag = false;
        for (auto mat : matches) {
	    if (mat.size() > mes.size()) {
	        break;
	    }
	    bool same = true;
            for (int i = 0; i < mat.size(); i++) {
                if (mat[i][0] != mes[i]) {
		    same = false;
                    break;
                }
            }
	    if (same) {
		for (auto mat2 : matches) {
		    int plussize = mat.size() + mat2.size();
		    if (plussize > mes.size()) {
		        break;
		    }
		    bool same2 = true;
		    for (int i = mat.size(); i < plussize; i++) {
			if (mat2[i - mat.size()][0] != mes[i]) {
			    same2 = false;
			    break;
			}
		    }
		    if (same2) {
		        sum++;
			breakflag = true;
			break;
		    }
		}
	    }
	    if (breakflag) {
		break;
	    }
        }
    }

    cout << "Number of correct matches is: " << sum << endl;

    return 0;
} // main
