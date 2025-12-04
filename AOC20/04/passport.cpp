#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

bool isValidPassport(const std::unordered_map<std::string, std::string>& passport) {
    const std::vector<std::string> requiredFields = {
        "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
    };

    for (const auto& field : requiredFields) {
        if (passport.find(field) == passport.end()) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line, passportData;
    int validPassports = 0;
    std::unordered_map<std::string, std::string> passport;

    while (std::getline(file, line)) {
        if (line.empty()) {
            // Process the current passport
            if (isValidPassport(passport)) {
                validPassports++;
            }
            passport.clear(); // Reset for next passport
        } else {
            std::istringstream iss(line);
            std::string keyValue;
            while (iss >> keyValue) {
                std::string key = keyValue.substr(0, keyValue.find(':'));
                std::string value = keyValue.substr(keyValue.find(':') + 1);
                passport[key] = value;
            }
        }
    }

    // Process the last passport (in case the file doesn't end with a blank line)
    if (isValidPassport(passport)) {
        validPassports++;
    }

    std::cout << "Number of valid passports: " << validPassports << std::endl;
    return 0;
}

