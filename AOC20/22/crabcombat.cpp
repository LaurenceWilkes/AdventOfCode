#include <queue>
#include <fstream>
#include <iostream>
#include <set>


using namespace std;

void parseInput(const string& filename, queue<int>& deck1, queue<int>& deck2) {
    ifstream file(filename);
    string line;

    bool p2flag = false;
    while (getline(file, line)) {
	if (line[0] == 'P') {continue;}
	if (line.empty()) {p2flag = true; continue;}
	if (p2flag) {
	    deck2.push(stoi(line));
	} else {
	    deck1.push(stoi(line));
	}
    }
}

void displayDeck(queue<int> deck) {
    int dlen = deck.size();
    while (dlen > 0) {
	cout << deck.front() << " ";
	deck.pop();
	dlen--;
    }
    cout << endl;
} // displayDeck


void sim(queue<int>& deck1, queue<int>& deck2) { 
    int round = 1;
    while (!deck1.empty() && !deck2.empty()) {
// 	cout << "round: " << round << endl;
// 	round++;
// 	cout << "deck 1 : ";
// 	displayDeck(deck1);
// 	cout << "deck 2 : ";
// 	displayDeck(deck2);
// 	cout << endl;
// 
	int p1 = deck1.front();
	deck1.pop();
	int p2 = deck2.front();
	deck2.pop();
	if (p1 > p2) {
	    deck1.push(p1);
	    deck1.push(p2);
	} else {
	    deck2.push(p2);
	    deck2.push(p1);
	}
    }
} // sim 
   
int tallyUp(queue<int> deck) {
    int out = 0;
    int len = deck.size();
    while (len > 0) {
	out += deck.front() * len;
	deck.pop();
	len--;
    }
    return out;
} // tallyUp

pair<bool, int> recursiveSim(queue<int> deck1, queue<int> deck2) {
    set<pair<queue<int>, queue<int>>> seenStates;
    while (!deck1.empty() && !deck2.empty()) {
	pair<queue<int>, queue<int>> d12 = {deck1, deck2};
	if (seenStates.find(d12) != seenStates.end()) {return make_pair(true, 0);}
	seenStates.insert(d12);
	int p1 = deck1.front();
	deck1.pop();
	int p2 = deck2.front();
	deck2.pop();
	if (p1 <= deck1.size() && p2 <= deck2.size()) {
	    queue<int> d1c = deck1;
	    queue<int> d2c = deck2;
	    queue<int> nd1;
	    queue<int> nd2;
	    for (int i = 0; i < p1; i++) {nd1.push(d1c.front()); d1c.pop();}
	    for (int i = 0; i < p2; i++) {nd2.push(d2c.front()); d2c.pop();}
	    if (recursiveSim(nd1, nd2).first) {
		deck1.push(p1);
		deck1.push(p2);
	    } else {
		deck2.push(p2);
		deck2.push(p1);
	    }
	} else if (p1 > p2) {
	    deck1.push(p1);
	    deck1.push(p2);
	} else {
	    deck2.push(p2);
	    deck2.push(p1);
	}
    }
    if (deck1.empty()) {
	return make_pair(false, tallyUp(deck2));
    } else {
        return make_pair(true, tallyUp(deck1));
    }
} // recursiveSim

void simplegame(queue<int> deck1, queue<int> deck2) {
    sim(deck1, deck2);

    int score;
    if (deck1.empty()) {
	cout << "deck2 wins combat with size: " << deck2.size();
	score = tallyUp(deck2);
    } else {
	cout << "deck1 wins combat with size: " << deck1.size();
        score = tallyUp(deck1);
    }
    cout << " score: " << score << endl;
} // simplegame

int main() {
    queue<int> deck1;
    queue<int> deck2;
    parseInput("input.txt", deck1, deck2);

    simplegame(deck1, deck2);

    auto [p1, outscore] = recursiveSim(deck1, deck2);
    string winner = p1 ? "1 " : "2 ";
    cout << "Player " + winner + "wins with a score of " << outscore << endl;

    return 0;
} // main
