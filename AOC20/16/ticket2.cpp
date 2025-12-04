#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

vector<vector<int>> getTickets(const string& filename, vector<int>& yourTicket, vector<string>& ruleNames, vector<vector<int>>& rules) {
    ifstream file(filename);
    string line;
    regex rangepattern(R"((\w+): (\d+)-(\d+) or (\d+)-(\d+))");

    bool readingNearbyTickets = false;
    bool readingYourTicket = false;
    vector<vector<int>> nearbyTickets;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (regex_search(line, rangepattern)) {
            smatch match;
            regex_search(line, match, rangepattern);

	    ruleNames.push_back(match[1]);

            vector<int> rule = {
                stoi(match[2]), stoi(match[3]),
                stoi(match[4]), stoi(match[5])
            };
            rules.push_back(rule);

        } else if (line == "your ticket:") {
            readingYourTicket = true;
        } else if (readingYourTicket) {
            // Parse ticket values as integers
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                yourTicket.push_back(stoi(value));
            }
	    readingYourTicket = false;
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
    for (int el : ticket) {
	bool flag = true;
	for (auto r : rules) {
	    if (r[0] <= el && el <= r[1]) {
		flag = false;
	        break;
	    } else if (r[2] <= el && el <= r[3]) {
		flag = false;
		break;
	    }
	}
	if (flag) {
	    return false;
	}
    }
    return true;
} // checkTicket

void whittle(vector<int>& currentFields, const vector<int>& ticket, const vector<int>& rule) {
    vector<int> toRemove;
    for (int i = currentFields.size() - 1; i >= 0; i--) {
	if (!(rule[0] <= ticket[i] && ticket[i] <= rule[1]) && !(rule[2] <= ticket[i] && ticket[i] <= rule[3])) {
	    toRemove.push_back(i);
	}
    }
    for (int i : toRemove) {
	currentFields.erase(currentFields.begin() + i);
    }
} // whittle


bool bipartiteMatch(int u, vector<bool>& visited, vector<int>& assign, const vector<vector<int>>& bipartiteGraph) {
    for (int v : bipartiteGraph[u]) {  // for all jobs applicant u is interested in
        if (!visited[v]) {             // when job v is not visited
            visited[v] = true;         // mark job v as visited
            // if v is not assigned or previously assigned applicant can be reassigned
            if (assign[v] < 0 || bipartiteMatch(assign[v], visited, assign, bipartiteGraph)) {
                assign[v] = u;         // assign job v to applicant u
                return true;
            }
        }
    }
    return false;
}

pair<int, vector<pair<int, int>>> maxMatch(const vector<vector<int>>& bipartiteGraph, int M, int N) {
    vector<int> assign(N, -1);   // track which job is assigned to which applicant, -1 if unassigned
    vector<pair<int, int>> matching;   // to store the final matched pairs
    int jobCount = 0;

    for (int u = 0; u < M; u++) {
        vector<bool> visited(N, false);  // reset visited jobs for each applicant
        if (bipartiteMatch(u, visited, assign, bipartiteGraph)) {
            jobCount++;
        }
    }

    // Collect the matching pairs (applicant, job) based on final assignments
    for (int v = 0; v < N; v++) {
        if (assign[v] != -1) {
            matching.emplace_back(assign[v], v);  // pair applicant and job
        }
    }

    return {jobCount, matching};
}

int main() {
    vector<int> yourTicket;
    vector<string> ruleNames;
    vector<vector<int>> rules;
    vector<vector<int>> nearbyTickets = getTickets("input.txt", yourTicket, ruleNames, rules);

    vector<int> toRemove;
    for (int i = nearbyTickets.size() - 1; i >= 0; i--) {
	if (!checkTicket(nearbyTickets[i], rules)) {
	    toRemove.push_back(i);
	}
    }
    for (int j : toRemove) {
	nearbyTickets.erase(nearbyTickets.begin() + j);
    }

    vector<vector<int>> validFields;

    for (int i = 0; i < ruleNames.size(); i++) {
	vector<int> currentFields;
	for (int k = 0; k < yourTicket.size(); k++) {
	    currentFields.push_back(k);
	}
        for (auto t : nearbyTickets) {

	    whittle(currentFields, t, rules[i]);
        }
	validFields.push_back(currentFields);
    }

    for (int i = 0; i < ruleNames.size(); i++) {
        cout << ruleNames[i] << " - is valid for - [ ";
	for (int j = 0; j < validFields[i].size(); j++) {
	    cout << validFields[i][j] << " ";
	}
	cout << " ]" << endl;
    }

    int M = validFields.size();
    int N = M;

    auto [maxMatchingCount, matchingPairs] = maxMatch(validFields, M, N);
    cout << "Maximum number of applicants that can get jobs: " << maxMatchingCount << endl;
    cout << "Matching pairs (Applicant, Job):" << endl;
    unsigned long long prod = 1;
    for (const auto& [applicant, job] : matchingPairs) {
	cout << "(" << applicant << ", " << job << ")" << endl;
	if (applicant < 6) {
	    prod *= yourTicket[job];
	}
    }
    cout << "Product is: " << prod << endl;

    return 0;
} // main

