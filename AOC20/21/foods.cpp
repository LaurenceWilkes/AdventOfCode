#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>
#include <set>
#include <unordered_map>

using namespace std;

// Function to parse input
void parseInput(const string& filename,
    vector<set<string>>& ingredientsList,
    vector<set<string>>& allergensList) {

    ifstream file(filename);
    string line;

    regex line_regex(R"((.+) \(contains (.+)\))");
    smatch match;

    while (getline(file, line)) {
        if (regex_match(line, match, line_regex)) {
            istringstream ingredientsStream(match[1].str());
            istringstream allergensStream(match[2].str());

            // Parse ingredients
            set<string> ingredients;
            string ingredient;
            while (ingredientsStream >> ingredient) {
                ingredients.insert(ingredient);
            }

            // Parse allergens
            set<string> allergens;
            string allergen;
            while (getline(allergensStream, allergen, ',')) {
                allergen.erase(0, allergen.find_first_not_of(' '));
                allergen.erase(allergen.find_last_not_of(' ') + 1);
                allergens.insert(allergen);
            }

            ingredientsList.push_back(ingredients);
            allergensList.push_back(allergens);
        }
    }
}

void displayFood(const set<string>& ingredients, const set<string>& allergens) {
    cout << "ingredients: ";
    for (auto in : ingredients) {
        cout << in << " ";
    }
    cout << endl;
    cout << "allergens: ";
    for (auto al : allergens) {
        cout << al << " ";
    }
    cout << endl;
    cout << endl;
} // displayFood

void allergenSafe(const vector<set<string>>& ingredientsList, const vector<set<string>>& allergensList, 
		  unordered_map<string, set<string>>& possibilities, set<string>& allIngredients, set<string>& allAllergens) {
    for (size_t i = 0; i < ingredientsList.size(); i++) {
	for (const auto& ing : ingredientsList[i]) { // create the list with all ingredients
	    allIngredients.insert(ing);
	}
	for (const auto& a : allergensList[i]) { // for each allergen for the food, whittle the possible allergens list down
	    allAllergens.insert(a);
	    if (possibilities[a].empty()) {
		possibilities[a] = ingredientsList[i];
	    } else {
		set<string> result;
		set_intersection(possibilities[a].begin(), possibilities[a].end(),
				 ingredientsList[i].begin(), ingredientsList[i].end(),
				 inserter(result, result.begin()));
		possibilities[a] = result;
	    }
	}
    } 
    // At this point, the set corresponding to each of the allergens will
    // have been reduced to the intersection of all those in the food stuffs.
    set<string> reducedIngredients = allIngredients;  
    for (const auto& a : allAllergens) {
	for (const auto& ing : possibilities[a]) {
	    reducedIngredients.erase(ing);
	}    
    }
    int countOut = 0;
    for (size_t i = 0; i < ingredientsList.size(); i++) {
        for (const auto& ing : ingredientsList[i]) {
            if (reducedIngredients.find(ing) != reducedIngredients.end()) {
		countOut += 1;
	    } 
        }
    }
    cout << "Unmatched appearances: " << countOut << endl;
} // allergenSafe

vector<string> answersVector(unordered_map<string, set<string>>& possibilities, set<string>& allIngredients, set<string>& allAllergens) {
    unordered_map<string, string> endMap;
    vector<string> allergenCycle(allAllergens.begin(), allAllergens.end());
    int i = 0;
    int safetystop = 1000000;
    while (allergenCycle.size() > 0 && safetystop > 0) {
	if (i == allergenCycle.size()) {i = 0;}
	string a = allergenCycle[i];
	if (possibilities[a].size() == 1) {
	    string result = *(possibilities[a].begin());
	    endMap[a] = result;
	    allergenCycle.erase(allergenCycle.begin() + i);
	    i--;
	    for (const auto& a : allAllergens) {
		possibilities[a].erase(result); 
	    } 
	}
	i++;
	safetystop--;
    } 
    if (safetystop == 0) {cerr << "While loop overflow";}
    vector<string> outList;
    vector<string> allergenVector(allAllergens.begin(), allAllergens.end());
    sort(allergenVector.begin(), allergenVector.end());
    for (const auto& a : allergenVector) {
	outList.push_back(endMap[a]);
    }
    return outList;
} // answersVector

string combineList(vector<string> list) {
    string partial = "";
    for (int i = 0; i < list.size()-1; i++) {
	partial += list[i] + ","; 
    }
    partial += list[list.size()-1];
    return partial;
} // combineList

int main() {
    vector<set<string>> ingredientsList;
    vector<set<string>> allergensList;


    parseInput("input.txt", ingredientsList, allergensList);

    unordered_map<string, set<string>> possibilities; 
    set<string> allIngredients;
    set<string> allAllergens;

    allergenSafe(ingredientsList, allergensList, possibilities, allIngredients, allAllergens);
    vector<string> outList = answersVector(possibilities, allIngredients, allAllergens);
    cout << "output is: " << combineList(outList) << endl;
    return 0;
} // main
