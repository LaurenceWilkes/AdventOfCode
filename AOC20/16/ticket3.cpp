#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <regex>

using namespace std;

struct Range {
    int start;
    int end;

    bool contains(int value) const {
        return value >= start && value <= end;
    }
};

struct Rule {
    string field;
    Range range1;
    Range range2;

    bool isValid(int value) const {
        return range1.contains(value) || range2.contains(value);
    }
};

Range parseRange(const string& rangeStr) {
    int start, end;
    sscanf(rangeStr.c_str(), "%d-%d", &start, &end);
    return { start, end };
}

vector<Rule> parseRules(istream& input) {
    vector<Rule> rules;
    regex ruleRegex(R"((.+): (\d+-\d+) or (\d+-\d+))");
    smatch match;
    string line;

    while (getline(input, line) && !line.empty()) {
        if (regex_match(line, match, ruleRegex)) {
            Rule rule;
            rule.field = match[1];
            rule.range1 = parseRange(match[2]);
            rule.range2 = parseRange(match[3]);
            rules.push_back(rule);
        }
    }
    return rules;
}

vector<int> parseTicket(const string& line) {
    vector<int> ticket;
    istringstream ss(line);
    string num;
    while (getline(ss, num, ',')) {
        ticket.push_back(stoi(num));
    }
    return ticket;
}

bool isTicketValid(const vector<int>& ticket, const vector<Rule>& rules) {
    for (int value : ticket) {
        bool valid = false;
        for (const auto& rule : rules) {
            if (rule.isValid(value)) {
                valid = true;
                break;
            }
        }
        if (!valid) {
            return false;
        }
    }
    return true;
}

map<string, set<int>> determineFieldPositions(
    const vector<vector<int>>& validTickets, const vector<Rule>& rules) {
    
    int numFields = validTickets[0].size();
    map<string, set<int>> possiblePositions;

    for (const auto& rule : rules) {
        set<int> positions;
        for (int pos = 0; pos < numFields; ++pos) {
            bool allValid = true;
            for (const auto& ticket : validTickets) {
                if (!rule.isValid(ticket[pos])) {
                    allValid = false;
                    break;
                }
            }
            if (allValid) {
                positions.insert(pos);
            }
        }
        possiblePositions[rule.field] = positions;
    }
    return possiblePositions;
}

map<string, int> resolveFieldPositions(map<string, set<int>>& possiblePositions) {
    map<string, int> fieldPositions;

    while (!possiblePositions.empty()) {
        for (auto it = possiblePositions.begin(); it != possiblePositions.end(); ) {
            if (it->second.size() == 1) {
                int pos = *it->second.begin();
                fieldPositions[it->first] = pos;
                possiblePositions.erase(it++);
                for (auto& entry : possiblePositions) {
                    entry.second.erase(pos);
                }
            } else {
                ++it;
            }
        }
    }
    return fieldPositions;
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    auto rules = parseRules(inputFile);

    string line;
    getline(inputFile, line);  // Skip "your ticket:" line
    getline(inputFile, line);
    auto yourTicket = parseTicket(line);

    getline(inputFile, line);  // Skip empty line
    getline(inputFile, line);  // Skip "nearby tickets:" line

    vector<vector<int>> nearbyTickets;
    while (getline(inputFile, line)) {
        nearbyTickets.push_back(parseTicket(line));
    }

    vector<vector<int>> validTickets;
    for (const auto& ticket : nearbyTickets) {
        if (isTicketValid(ticket, rules)) {
            validTickets.push_back(ticket);
        }
    }

    auto possiblePositions = determineFieldPositions(validTickets, rules);

    // Print possible positions for each field
    cout << "Possible positions for each field before sorting:\n";
    for (const auto& [field, positions] : possiblePositions) {
        cout << field << ": ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }

    auto fieldPositions = resolveFieldPositions(possiblePositions);

    long long product = 1;
    for (const auto& [field, pos] : fieldPositions) {
        if (field.find("departure") == 0) {
            product *= yourTicket[pos];
        }
    }

    cout << "Product of 'departure' fields: " << product << endl;
    return 0;
}

