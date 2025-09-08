#include <iostream>
#include <string>
#include <fstream>
#include <regex>


using namespace std;

vector<pair<string, int>> readFile(const string& filename) {
    ifstream file(filename);

    string line;  
    smatch match;
    vector<pair<string, int>> Out;

    regex pattern(R"((\w+) ([+-]\d+))");

    while (getline(file, line)) {
	if (regex_match(line, match, pattern)) {
	    string command = match[1];
	    int arg = stoi(match[2]);
	    Out.push_back(make_pair(command, arg));
	} else {
	    cout << "error";
	} 
    }
    return Out;
} // readFile

int simTillLoop(const int& startpos, const vector<pair<string, int>>& Inst) {
    int currentpos = startpos;
    int acc = 0;

    map<int, bool> visited;

    while (!visited[currentpos]) {
	visited[currentpos] = true;
	auto [command, arg] = Inst[currentpos];
	if (command == "nop") {
	    currentpos++;
	} else if (command == "acc") {
	    acc += arg;
	    currentpos++;
	} else {
	    currentpos += arg;
	}
    }
    return acc;
} // simTillLoop

int isCorr(const int& startpos, const vector<pair<string, int>>& Inst) {
    int currentpos = startpos;
    int acc = 0;

    map<int, bool> visited;

    while (!visited[currentpos]) {
	visited[currentpos] = true;
	auto [command, arg] = Inst[currentpos];
	if (command == "nop") {
	    currentpos++;
	} else if (command == "acc") {
	    acc += arg;
	    currentpos++;
	} else {
	    currentpos += arg;
	}
	if (currentpos == Inst.size()) {
	    return true;
	}   
    }
    return false;
} // isCorr 

int simTillEnd(const vector<pair<string, int>>& Inst) {
    int acc = 0;
    int currentpos = 0;

    while (currentpos != Inst.size()) {
	auto [command, arg] = Inst[currentpos];
	if (command == "nop") {
	    currentpos++;
	} else if (command == "acc") {
	    acc += arg;
	    currentpos++;
	} else {
	    currentpos += arg;
	}
    }
    return acc;
} // simTillEnd
    
  // This may not be the right method as there could be multiple good start positions
  // But with my input it works at least!
  
int findFault(vector<pair<string, int>>& Inst) {
    int len = Inst.size();

    for (int i = 0; i < len; i++) {
        if (Inst[i].first == "nop") {
	    Inst[i].first = "jmp";
	    if (isCorr(i, Inst)) {
		break;
	    } else {
	        Inst[i].first = "nop";
	    }
	     
        } else if (Inst[i].first == "jmp") {
            Inst[i].first = "nop";
	    if (isCorr(i, Inst)) {
	        break;
	    } else {
	        Inst[i].first = "jmp";
	    }
        }          
    }

    return 0;
    
} // findFault



int main() {
    vector<pair<string, int>> Inst = readFile("input.txt");

//    int accvalatloop = simTillLoop(Inst);

//    cout << "Accumulator value at loop is: " << accvalatloop << endl;

    findFault(Inst);

    int endsim = simTillEnd(Inst);

    cout << "Value at end is: " << endsim << endl;

    return 0;
} // main

