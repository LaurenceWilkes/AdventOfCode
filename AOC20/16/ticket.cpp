#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

vector<vector<int>> getTickets(const string& filename, vector<vector<int>>& rules) {
    ifstream file(filename);
    string line;
    regex rangepattern(R"(\w+: (\d+)-(\d+) or (\d+)-(\d+))");

    bool readingNearbyTickets = false;
    vector<vector<int>> nearbyTickets;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (regex_search(line, rangepattern)) {
            smatch match;
            regex_search(line, match, rangepattern);

            // Extract rule ranges and store in the rules vector
            vector<int> rule = {
                stoi(match[1]), stoi(match[2]),
                stoi(match[3]), stoi(match[4])
            };
            rules.push_back(rule);

        } else if (line == "nearby tickets:") {
            readingNearbyTickets = true;
        } else if (readingNearbyTickets) {
            // Parse ticket values as integers
            stringstream ss(line);
            string value;
            vector<int> ticket;

            while (getline(ss, value, ',')) {
                ticket.push_back(stoi(value));
            }
            nearbyTickets.push_back(ticket);
        }
    }

    file.close();
    return nearbyTickets;
}


int checkTicket(const vector<int>& ticket, const vector<vector<int>>& rules) {
    int sum = 0;
    for (int el : ticket) {
	bool flag = true;
	for (auto r : rules) {
	    if (r[0] <= el && el <= r[1]) {
		flag = false;
	        break;
	    }
	    if (r[2] <= el && el <= r[3]) {
		flag = false;
		break;
	    }
	}
	if (flag) {
	    sum += el;
	}
    }
    return sum;
} // checkTicket

int main() {
    vector<vector<int>> rules;
    vector<vector<int>> nearbytickets = getTickets("input.txt", rules);

    int sum = 0;
    for (auto t : nearbytickets) {
	sum += checkTicket(t, rules);
    }

    cout << "Sum of bad elements is: " << sum;

    return 0;
} // main

