#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int mod1(int x, int n) {return (((x % n) + n - 1) % n) + 1;}

vector<int> shuffleCups(vector<int> input, int moves) {
    int n = input.size();
    vector<int> nextCup(n);
    for (int i = 0; i < n; i++) {
        nextCup[input[i]] = input[(i + 1) % n];
    }
    int current = input[0];
    for (int m = 1; m <= moves; m++) {
	int next = nextCup[current];
	int nextt = nextCup[next];
	int nexttt = nextCup[nextt];
	nextCup[current] = nextCup[nexttt];

	int prev = mod1(current - 1, n);
	while (prev == next || prev == nextt || prev == nexttt) {
	    prev = mod1(prev - 1, n);
	}

	int placeholder = nextCup[prev];
	nextCup[prev] = next;
	nextCup[nexttt] = placeholder;

	current = nextCup[current];
    }
    return nextCup;
} // shuffleCups

string afterOne(vector<int> nextCup) {
    string outString = "";
    int current = 1;
    int n = nextCup.size();
    for (int i = 0; i < n-1; i++) {
	current = nextCup[current];
        outString += to_string(current);
    }
    return outString;
} // afterOne

vector<int> readInput(string filename) {
    ifstream f(filename);
    string line; 
    getline(f, line);
    vector<int> out;
    for (char c : line) {
	int ci = c - '0';
        out.push_back(ci);
    }
    return out;
}

int main() {
    vector<int> input = readInput("input.txt");

    int moves = 100;
    string outString = afterOne(shuffleCups(input, moves));

    cout << "After " << moves << " moves, the output is: " << outString << endl;

    for (int i = input.size()+1; i <= 1000000; i++) {
        input.push_back(i);
    }

    moves = 10000000;
    vector<int> nextCup = shuffleCups(input, moves);

    long long next = nextCup[1];
    long long nextt = nextCup[next];
    long long product = next * nextt;
    cout << "After " << moves << " moves, the product of the next two cups are: " << next << " and " << nextt << ". Their product is: " << product << endl;

    return 0;
} // main
