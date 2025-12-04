#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>
#include <regex>


using namespace std;

void readFile(const string& filename, vector<pair<long, long>>& buttonA, vector<pair<long, long>>& buttonB, vector<pair<long, long>>& prize) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open input file." << endl;
        return;
    }

    string line;
    regex regexA("Button A: X\\+(-?\\d+), Y\\+(-?\\d+)");
    regex regexB("Button B: X\\+(-?\\d+), Y\\+(-?\\d+)");
    regex regexP("Prize: X=(-?\\d+), Y=(-?\\d+)");
    smatch match;

    while (getline(inputFile, line)) {
        if (line.empty()) continue; // Skip empty lines.

        pair<long, long> a, b, p;

        // Parse Button A line.
        if (regex_match(line, match, regexA)) {
            a.first = stoi(match[1]);
            a.second = stoi(match[2]);
            buttonA.push_back(a);
        }

        // Read next line for Button B.
        if (regex_match(line, match, regexB)) {
            b.first = stoi(match[1]);
            b.second = stoi(match[2]);
            buttonB.push_back(b);
        }

        // Read next line for Prize.
        if (regex_match(line, match, regexP)) {
            p.first = stoi(match[1]);
            p.second = stoi(match[2]);
            prize.push_back(p);
        }
    }
}

long gcd(long a, long b) {
    // Find Minimum of a and b
    long res = min(a, b);

      // Testing divisiblity with all numbers starting from
    // min(a, b) to 1

    while (res > 1) {

        // If any number divide both a and b, so we
        // got the answer
        if (a % res == 0 && b % res == 0)
            break;
        res--;
    }
    return res;
}

tuple<long, long, long> gcdWithBezout(long p, long q) {
    if (q == 0)
        return {p, 1, 0};

    auto [d, a, b] = gcdWithBezout(q, p % q);
    return {d, b, a - (p / q) * b};
}

void printPair(const pair<long, long>& p) {
    cout << p.first << " " << p.second << endl;
} // printPair


bool solveSystem(const pair<long, long>& a, const pair<long, long>& b, const pair<long, long>& p, pair<long, long>& solution) {
    long determinant = a.first * b.second - b.first * a.second;
    if (determinant != 0) {
	solution = {b.second * p.first - b.first * p.second, -a.second * p.first + a.first * p.second};
	if ((solution.first % determinant) == 0 && (solution.second % determinant) == 0) {
	    solution.first /= determinant;
	    solution.second /= determinant;
	    if (solution.first >= 0 && solution.second >= 0) {
		return true;
	    }
	}
    } else {
	cout << "See code at end of file" << endl;
    }
    return false;
} // solveSystem


int main() {
    vector<pair<long, long>> buttonA;
    vector<pair<long, long>> buttonB;
    vector<pair<long, long>> prize;

    readFile("input.txt", buttonA, buttonB, prize);

    long sumCoins = 0;
    for (int i = 0; i < buttonA.size(); i++) {
	pair<long, long> solution;
	long mult = 10000000000000;
	pair<long, long> farPrize = {prize[i].first + mult, prize[i].second + mult};
	if (solveSystem(buttonA[i], buttonB[i], farPrize, solution)) {
	    sumCoins += 3 * solution.first + solution.second;
	}
    }

    cout << "The answer is: " << sumCoins << endl;

    return 0;
}

//	if (a.first * b.second == b.first * a.second && b.first * p.second == b.second * p.first) {
//	    auto [g, alpha, beta] = gcdWithBezout(a.first, b.first);
//	    if (p.first % g == 0) {
//		pair<int, int> hs = {b.first / g, -a.first / g};
//		alpha *= p.first / g;
//		beta *= p.first / g;
//
//		int ba;
//		int bb;
//		if (alpha < 0 || beta < 0) {
//		    bool bFlag = false;
//		    int alpha1 = alpha;
//		    int beta1 = beta;
//		    int alpha2 = alpha;
//		    int beta2 = beta;
//		    for (int i = 1; i < 1000; i++) {
//			alpha1 += hs.first;
//			beta1 += hs.second;
//			alpha2 -= hs.first;
//			beta2 -= hs.second;
//			if (alpha1 >= 0 && beta1 >= 0) {
//			    ba = alpha1;
//			    bb = beta1;
//			    bFlag = true;
//			    break;
//			}
//			if (alpha2 >= 0 && beta2 >= 0) {
//			    ba = alpha2;
//			    bb = beta2;
//			    bFlag = true;
//			    break;
//			}
//		    }
//		    if (!bFlag) {
//		        return false;
//		    }
//		} else {
//		    ba = alpha;
//		    bb = beta;
//		}
//
//		int alpha1 = alpha;
//		int beta1 = beta;
//		int alpha2 = alpha;
//		int beta2 = beta;
//		for (int i = 1; i < 1000; i++) {
//		    alpha1 += hs.first;
//		    beta1 += hs.second;
//		    alpha2 -= hs.first;
//		    beta2 -= hs.second;
//		    if (alpha1 >= 0 && beta1 >= 0 && alpha1 < ba) {
//			ba = alpha1;
//			bb = beta1;
//		    }
//		    if (alpha2 >= 0 && beta2 >= 0 && alpha1 < ba) {
//			ba = alpha2;
//			bb = beta2;
//		    }
//		}
//
//		solution.first = ba;
//		solution.second = bb;
//		return true;
