#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<long> getVec(string filename) {
    ifstream file(filename);
    
    string line;

    vector<long> Out;

    while (getline(file, line)) {
	Out.push_back(stol(line)); 
    }

    return Out;

} // getVec


long findError(int jump, const vector<long>& streaminput) {
    int len = streaminput.size();

    bool bflag = false;

    for (int i = jump; i < len; i++) {
        long target = streaminput[i];
	for (int j = i-jump; j < i-1; j++) {
	    for (int k = j+1; k < i; k++) {
		if (streaminput[j] + streaminput[k] == target && streaminput[j] != streaminput[k]) {
		    bflag = true;
		    break;
		} 	     
	    }
	    if (bflag) {
	        break;
	    } 
	}
	if (!bflag) {
	    return target;
	} else {
	    bflag = false;
	}
    }
    return -1;
} // findError

long findWeakness(long errval, const vector<long>& streaminput) {
    int sumlen = 3;
    int len = streaminput.size();

    while (sumlen <= len) {
        for (int i = 0; i < len-sumlen+1; i++) {
	    long tempsum = 0;
	    long selt = errval;
	    long lelt = 0;
	    for (int j = 0; j < sumlen; j++) {
		long csi = streaminput[i+j];
		tempsum += streaminput[i+j];
		if (selt > csi) {
		    selt = csi; 
		}
		if (lelt < csi) {
		    lelt = csi;
		} 
	    }
	    if (tempsum == errval) {
	        return selt + lelt;
	    }
        }
	sumlen++; 
    }
    return 1;
} // findWeakness


int main() {
    vector<long> streaminput = getVec("input.txt");

    long errval = findError(25, streaminput);

    cout << "Error value is: " << errval << endl;

    cout << "Weakness is: " << findWeakness(errval, streaminput);

    return 0;
} // main









