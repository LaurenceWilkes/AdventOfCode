#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <array>

using namespace std;

array<bool, 36> intToBool(long long inp) {
    long long n = inp;
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

long long boolToInt(array<bool, 36>& arr) {
    long long mult = 1; 
    long long out = 0;
    for (bool b : arr) {
        if (b) {
            out += mult;
        } 
	mult *= 2;
    }
    return out; 
} // boolToInt


long long applyMask(long long inp, const array<bool, 36>& mask_active, const array<bool, 36>& mask_vals) {
    array<bool, 36> arr = intToBool(inp);
    for (int i = 0; i < 36; i++) {
        if (mask_active[i]) {
            if (mask_vals[i]) {
                arr[i] = true;
            } else {
                arr[i] = false;
            }
        } 
    }
    return boolToInt(arr);
} // applyMask


long long performDocking(string filename) {
    ifstream file(filename);

    string line;
    array<bool, 36> mask_active;
    array<bool, 36> mask_vals;

    regex pattern(R"(mem\[(\d+)\] = (\d+))");
    smatch matches;

    map<int, long long> mem;

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
	    long long inp = stol(matches[2]);
	    long long out = applyMask(inp, mask_active, mask_vals);
	    mem[memloc] = out;
        } else {
            cout << "unknown instruction" << endl;
        }
        
    }

    long long sum = 0;

    for (const auto& pair : mem) {
        sum += pair.second;
    }
    return sum;
} // performDocking

int main() {
    long long sum = performDocking("input.txt");
    cout << "Sum of dock vals: " << sum << endl;
    return 0;
} // main

