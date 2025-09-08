#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

using namespace std;

// Function to check if a string matches the given rule sets
bool matches_rule(const string &message, const set<string> &A, const set<string> &B) {
    size_t a_len = A.begin()->size();
    size_t b_len = B.begin()->size();

    if (message.size() % a_len != 0) return false;

    int n_a = 0, n_b = 0;
    size_t pos = 0;

    while (pos < message.size() && A.count(message.substr(pos, a_len))) {
        ++n_a;
        pos += a_len;
    }

    while (pos < message.size() && B.count(message.substr(pos, b_len))) {
        ++n_b;
        pos += b_len;
    }

    return pos == message.size() && n_a >= 2 && n_b >= 1 && n_a > n_b;
}

// Parse the input into rules and messages
void parse_input(ifstream &file, unordered_map<int, string> &rules, vector<string> &messages) {
    string line;
    bool parsing_rules = true;

    while (getline(file, line)) {
        if (line.empty()) {
            parsing_rules = false;
            continue;
        }

        if (parsing_rules) {
            auto colon_pos = line.find(':');
            int rule_num = stoi(line.substr(0, colon_pos));
            string rule_content = line.substr(colon_pos + 2);
            rules[rule_num] = rule_content;
        } else {
            messages.push_back(line);
        }
    }
}

// Generate all possible strings from a rule
set<string> generate_matches(int rule_num, unordered_map<int, string> &rules, unordered_map<int, set<string>> &cache) {
    if (cache.count(rule_num)) return cache[rule_num];

    set<string> result;
    const string &rule = rules[rule_num];

    if (rule[0] == '"') {
        result.insert(rule.substr(1, rule.size() - 2));
    } else {
        istringstream stream(rule);
        string part;
        vector<string> options;

        while (getline(stream, part, '|')) {
            options.push_back(part);
        }

        for (const string &option : options) {
            istringstream opt_stream(option);
            vector<int> sub_rules;
            int sub_rule;
            while (opt_stream >> sub_rule) {
                sub_rules.push_back(sub_rule);
            }

            set<string> current_set = {""};
            for (int sub_rule : sub_rules) {
                set<string> sub_matches = generate_matches(sub_rule, rules, cache);
                set<string> new_set;
                for (const string &prefix : current_set) {
                    for (const string &suffix : sub_matches) {
                        new_set.insert(prefix + suffix);
                    }
                }
                current_set = move(new_set);
            }

            result.insert(current_set.begin(), current_set.end());
        }
    }

    cache[rule_num] = result;
    return result;
}

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open input file!" << endl;
        return 1;
    }

    unordered_map<int, string> rules;
    vector<string> messages;

    parse_input(file, rules, messages);
    file.close();

    unordered_map<int, set<string>> cache;
    set<string> A = generate_matches(42, rules, cache);
    set<string> B = generate_matches(31, rules, cache);

    int valid_count = 0;
    for (const string &message : messages) {
        if (matches_rule(message, A, B)) {
            ++valid_count;
        }
    }

    cout << "Valid messages count: " << valid_count << endl;
    return 0;
}

