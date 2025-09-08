#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <regex>

// Helper function to validate a field based on the given rules
bool isValidField(const std::string& key, const std::string& value) {
    if (key == "byr") {
        int year = std::stoi(value);
        return year >= 1920 && year <= 2002;
    } else if (key == "iyr") {
        int year = std::stoi(value);
        return year >= 2010 && year <= 2020;
    } else if (key == "eyr") {
        int year = std::stoi(value);
        return year >= 2020 && year <= 2030;
    } else if (key == "hgt") {
        if (value.find("cm") != std::string::npos) {
            int height = std::stoi(value.substr(0, value.size() - 2));
            return height >= 150 && height <= 193;
        } else if (value.find("in") != std::string::npos) {
            int height = std::stoi(value.substr(0, value.size() - 2));
            return height >= 59 && height <= 76;
        }
        return false;
    } else if (key == "hcl") {
        return std::regex_match(value, std::regex("^#[0-9a-f]{6}$"));
    } else if (key == "ecl") {
        const std::vector<std::string> validEcl = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        return std::find(validEcl.begin(), validEcl.end(), value) != validEcl.end();
    } else if (key == "pid") {
        return std::regex_match(value, std::regex("^[0-9]{9}$"));
    }
    return true; // Ignore "cid" or any other fields
}

// Function to check if the passport contains all required fields and valid values
bool isValidPassport(const std::unordered_map<std::string, std::string>& passport) {
    const std::vector<std::string> requiredFields = {
        "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
    };

    // Check for presence of required fields
    for (const auto& field : requiredFields) {
        if (passport.find(field) == passport.end()) {
            return false;
        }
    }

    // Validate each field
    for (const auto& [key, value] : passport) {
        if (!isValidField(key, value)) {
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


