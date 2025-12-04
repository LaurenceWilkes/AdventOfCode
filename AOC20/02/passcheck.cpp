#include <iostream>
#include <tuple>
#include "read_file.h"

int occurences(std::string string, char character) {
   int count = 0;
   for (char c : string) {
       if (c == character) {
           count++;
       } 
   }
   return count;
} // occurences

bool prevcorrect(int sr, int er, char character, std::string str) {
    int o = occurences(str, character);
    if (sr <= o && o <= er) {
        return true;
    } 
    return false;
} // correct

bool correct(int sr, int er, char character, std::string str) {
    if ((character == str[sr-1]) != (character == str[er-1])) {
       return true; 
    } 
    return false;
} // correct



int test(std::string filename) {
    auto data = readFile(filename);

    std::vector<int> startranges = std::get<0>(data);
    std::vector<int> endranges = std::get<1>(data);
    std::vector<char> chars = std::get<2>(data);
    std::vector<std::string> passwords = std::get<3>(data);

    int count = 0;

    for (int i = 0; i < startranges.size(); i++) {
       if (correct(startranges[i], endranges[i], chars[i], passwords[i])) {
           count++;
       }  
    }
    
    return count;

} // test

int main() {
    std::cout << "Count of good passwords is: " << test("input.txt") << std::endl;
    return 0;
} // main

