#include <iostream>
#include <vector>
#include <chrono>  // For time measurement
#include "read_file.h"  // File reading function


// Finding pairs
int method1(const std::vector<int>& numbers) {
    int size = numbers.size();
    
    for (int i = 0; i < size - 1; i++) {
		for (int j = i; j < size; j++) {
			if (numbers[i] + numbers[j] == 2020) {
				return numbers[i] * numbers[j];
			}  
		}
    }

    return -1;
}


int method2(const std::vector<int>& numbers) {
    bool seen[2020] = {false};

    // Traverse the list of numbers
    for (int num : numbers) {
        // Check if the complement (2020 - num) exists in the array
        int complement = 2020 - num;
        
        // Ensure the complement is within bounds (we can't have negative array indices)
        if (complement >= 0 && complement < 2020 && seen[complement]) {
            // If the complement is found, return the product of num and complement
            return num * complement;
        }

        // Mark the current number as seen
        if (num < 2020) {
            seen[num] = true;
        }
    }

    // If no such pair is found, return -1 or some error value
    return -1;
}

// Finding trios
int method3(const std::vector<int>& numbers) {
    int size = numbers.size();
    
    for (int i = 0; i < size - 2; i++) {
	for (int j = i + 1; j < size - 1; j++) {
	    for (int k = j + 1; k < size; k++) {
		if (numbers[i] + numbers[j] + numbers[k] == 2020) {
		    return numbers[i] * numbers[j] * numbers[k];
		}  
	    }
	}
    }

    return -1;
}


int method4(const std::vector<int>& numbers) {
    bool seen[2020] = {false};

    for (int num : numbers) {
	seen[num] = true;
    }
    
    // Traverse the list of numbers
    for (int i = 0; i < numbers.size() - 1; i++) {
	for (int j = i + 1; j < numbers.size(); j++) {
	    int complement = 2020 - numbers[i] - numbers[j];
	    if (complement >= 0 && seen[complement]) {
	       return numbers[i] * numbers[j] * complement;
	    }     
	}
    }

    // If no such trio is found, return -1 or some error value
    return -1;
}

// Function to measure execution time of a given function
template<typename Func>
void testFunction(const std::string& methodName, Func func, const std::vector<int>& numbers) {
    // Start time
    auto start = std::chrono::high_resolution_clock::now();
    
    // Call the function
    int result = func(numbers);  // Assuming your function returns an int (modify if needed)
    
    // End time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    // Output the result and time taken
    std::cout << methodName << " found the result: " << result 
              << " in " << elapsed.count() << " seconds." << std::endl;
}

int main() {
    try {
        // Read numbers from file
        std::vector<int> numbers = readFile("input.txt");

        // Test each method by passing the function and the numbers
	std::cout << "First part:" << std::endl;
        testFunction("Method 1", method1, numbers);
        testFunction("Method 2", method2, numbers);
        // Test each method by passing the function and the numbers
	std::cout << std::endl << "Second part:" << std::endl;
        testFunction("Method 1", method3, numbers);
        testFunction("Method 2", method4, numbers);
        // Add more testFunction calls for additional methods

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

