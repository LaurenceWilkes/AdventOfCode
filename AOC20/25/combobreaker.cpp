#include <iostream>
#include <fstream>
#include <string>

using namespace std;

pair<int, int> readFile(string filename) {
    ifstream f(filename);
    string line;
    getline(f, line);
    int x = stoi(line);
    getline(f, line);
    int y = stoi(line);
    pair<int, int> out = {x, y};
    return out;
} // readFile

int transform(int subject, int loopSize) {
    long long v = 1;
    for (int i = 0; i < loopSize; i++) {
        v = v * subject;
	v = v % 20201227;
    }
    return v;
} // transform

int brute(int res) {
    const int subject = 7;
    long long v = 1;
    int loops = 0;
    while (v != res) {
        v = v * subject;
	v = v % 20201227;
	loops++;
    }
    return loops;
} // brute

int main() {
    auto [card, door] = readFile("input.txt");

    int cardloop = brute(card);
    int doorloop = brute(door);

    int ekcard = transform(door, cardloop);
    int ekdoor = transform(card, doorloop);

    cout << "The encryption key produced by the card is: " << ekcard << endl;
    cout << "The encryption key produced by the door is: " << ekdoor << endl;

    return 0;
} // main
