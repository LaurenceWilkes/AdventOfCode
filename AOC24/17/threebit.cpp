#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <sstream>
#include <cmath>

using namespace std;

class Computer {
public:
    int instruction;
    int A;
    int B;
    int C;
    vector<int> program;

    void readFile(string filename) {
	ifstream file(filename);
	string line;
	getline(file, line);
	A = stoi(line.substr(12));
	getline(file, line);
	B = stoi(line.substr(12));
	getline(file, line);
	C = stoi(line.substr(12));
	getline(file, line);
	getline(file, line);
	stringstream ss(line.substr(9));
	int i;
	while (ss >> i) {
	    program.push_back(i);
	    if (ss.peek() == ',') {
		ss.ignore();
	    }
	}
    } // readFile

    int combo(int op) {
	if (op <= 3) return op;
	if (op == 4) return A;
	if (op == 5) return B;
	if (op == 6) return C;
	if (op == 7) cout << "error";
	return 1;
    }

    void outDisp(int op) {
        cout << op << ',';
    } // outDisp

    // 0
    void adv(int op) {
	A /= pow(2, combo(op));
    } // adv

    // 1
    void bxl(int op) {
	B = B ^ op;
    } // bxl

    // 2
    void bst(int op) {
        B = combo(op) % 8;
    } // bst

    // 3
    void jnz(int op) {
        if (!(A == 0)) {
	    instruction = op - 2; // possibly -2
        }
    } // jnz

    // 4
    void bxc(int op) {
        B ^= C;
    } // bxc

    // 5
    void out(int op) {
        outDisp(combo(op) % 8);
    } // out

    // 6
    void bdv(int op) {
	B = A / pow(2, combo(op));
    } // adv

    // 7
    void cdv(int op) {
	C = A / pow(2, combo(op));
    } // adv

    void wrapper(int opcode, int op) {
        switch (opcode) {
	    case 0: adv(op); break;
	    case 1: bxl(op); break;
	    case 2: bst(op); break;
	    case 3: jnz(op); break;
	    case 4: bxc(op); break;
	    case 5: out(op); break;
	    case 6: bdv(op); break;
	    case 7: cdv(op); break;
	}
    } // wrapper

    void runProgram() {
	while (instruction < program.size()) {
	    wrapper(program[instruction], program[instruction + 1]);
	    instruction += 2;
	}
    } // runProgram
};



int main() {
    Computer computer;
    computer.readFile("input.txt");
    computer.runProgram();
    return 0;
} // main
