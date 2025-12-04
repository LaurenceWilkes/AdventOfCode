#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>


using namespace std;

vector<int> readFile(string filename) {
    ifstream file(filename);

    string line;
    vector<int> Out;

    while (getline(file, line)) {
	Out.push_back(stoi(line));
    }

    Out.push_back(0);

    return Out;
}

int processAdapters(vector<int>& Ad) {
    sort(Ad.begin(), Ad.end());

    vector<int> diffs;

    for (int i = 1; i < Ad.size(); i++) {
	diffs.push_back(Ad[i] - Ad[i-1]);
    }
    diffs.push_back(3);

    int count3 = 0;
    int count1 = 0;

    for (int el : diffs) {
	if (el == 1) {
	    count1++;
	}
	if (el == 3) {
	    count3++;
	}
    }

    return count1 * count3;
}

long countAdapters(int s, map<int, long>& memo, const vector<int>& Ad) {
    if (memo[s] != 0) {
       return memo[s]; 
    } 
    int len = Ad.size();

    if (s == len-1) {
        return 1;
    } 
    if (s < len-3 && Ad[s+3] <= Ad[s]+3) {
	memo[s] = countAdapters(s+3, memo, Ad) + 
		  countAdapters(s+2, memo, Ad) +
		  countAdapters(s+1, memo, Ad);
	return memo[s];
    } else if (s < len-2 && Ad[s+2] <= Ad[s]+3) {
	memo[s] = countAdapters(s+2, memo, Ad) +
		  countAdapters(s+1, memo, Ad);
	return memo[s];
    } else if (Ad[s+1] <= Ad[s]+3) {
	memo[s] = countAdapters(s+1, memo, Ad);
	return memo[s];
    }

    return -1;
} // countAdapters



int main() {
    vector<int> Ad = readFile("input.txt");

    // int diffmult = processAdapters(Ad);

    // cout << "diff 1 mult by diff 3: " << diffmult << endl;
    
    sort(Ad.begin(), Ad.end());
   
    map<int, long> memo;

    cout << "number of combinations is: " << countAdapters(0, memo, Ad) << endl;

}
