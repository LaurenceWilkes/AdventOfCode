#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <array>
#include <vector>


using namespace std;

array<bool, 36> intToBool(long inp) {
    long n = inp;
    int r;
    array<bool, 36> out;
    out.fill(false);
    int i = 0;
    while (n != 0) {
	r = n % 2; 
	n = n / 2;
	out[i] = r;
	i++;
    }
    return out; 
} // intToBool

long boolToInt(array<bool, 36>& arr) {
    long mult = 1; 
    long out = 0;
    for (bool b : arr) {
        if (b) {
            out += mult;
        } 
	mult *= 2;
    }
    return out; 
} // boolToInt


pair<array<bool, 36>, array<bool, 36>> createAddress(long memloc, const array<bool, 36>& mask_active, const array<bool, 36>& mask_vals) {
    array<bool, 36> addr_active;
    array<bool, 36> addr_vals = intToBool(memloc);
    addr_active.fill(true);
    for (int i = 0; i < 36; i++) {
        if (mask_active[i]) {
	    if (mask_vals[i]) {
		addr_vals[i] = true; 
	    } 
	} else {
	    addr_active[i] = false;
	    addr_vals[i] = false;
	}
    }
    return make_pair(addr_active, addr_vals);
} // createAddress

void writeAddress(const long& inp, map<array<bool, 36>, long>& mem, const pair<array<bool, 36>, array<bool, 36>>& current) {
    for (int i = 0; i < 36; i++) {
        if (!current.first[i]) {
	    auto new1 = current;
	    auto new2 = current;
	    new1.first[i] = true;
	    new2.first[i] = true;
	    new1.second[i] = true;
	    new2.second[i] = false;
            writeAddress(inp, mem, new1);
            writeAddress(inp, mem, new2);
	    return;
        } 
    }
    mem[current.second] = inp;
    return;
} // writeAddress


long performDocking(string filename) {
    ifstream file(filename);

    string line;
    array<bool, 36> mask_active;
    array<bool, 36> mask_vals;

    regex pattern(R"(mem\[(\d+)\] = (\d+))");
    smatch matches;

    map<array<bool, 36>, long> mem;

    while (getline(file, line)) {
        if (line[1] == 'a') {	  // update mask
            string bitmask = line.substr(7);
	    int i = 0;
	    mask_active.fill(false);
	    mask_vals.fill(false);
	    for (char c : bitmask) {
	        if (c == '0') {
	           mask_active[35 - i] = true; 
	        } else if (c == '1') {
	            mask_active[35 - i] = true;
		    mask_vals[35 - i] = true;
	        }  
		i++;
	    }
        } else if (regex_match(line, matches, pattern)) {
	    int memloc = stoi(matches[1]);
	    long inp = stol(matches[2]);

	    auto current = createAddress(memloc, mask_active, mask_vals);

	    writeAddress(inp, mem, current);

        } else {
            cout << "unknown instruction" << endl;
        }
        
    }

    long sum = 0;

    for (const auto& pair : mem) {
        sum += pair.second;
    }
    return sum;
} // performDocking

int main() {
    long sum = performDocking("input.txt");
    cout << "Sum of dock vals: " << sum << endl;
    return 0;
} // main

