#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>    // Added for std::stringstream
#include <cstdlib>    // Added for std::remove
#include "bn2.h"     // Include the matrix_t class

// Define color codes
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Global counter for total failures
int total_failures = 0;

// Redefine the CHECK macro to accept a stream parameter
#define CHECK(condition, message, stream) \
    if (!(condition)) { \
        (stream) << RED << "Failure: " << message << RESET << std::endl; \
        ++total_failures; \
    }

// Test Function 1: Operator Access
void test_operator_access() {
    // Initialize a local stringstream to collect error messages
    std::stringstream errors;

    // Initialize a 3x3 matrix and set values
    matrix_t<int> mat(3, 3);
    mat(0, 0) = 1;
    mat(0, 1) = 2;
    mat(0, 2) = 3;
    mat(1, 0) = 4;
    mat(1, 1) = 5;
    mat(1, 2) = 6;
    mat(2, 0) = 7;
    mat(2, 1) = 8;
    mat(2, 2) = 9;

    // Perform checks using the CHECK macro, passing the errors stream
    CHECK(mat(0, 0) == 1, "mat(0, 0) should be 1", errors);
    CHECK(mat(1, 1) == 5, "mat(1, 1) should be 5", errors);
    CHECK(mat(2, 2) == 9, "mat(2, 2) should be 9", errors);

    const matrix_t<int>& constMat = mat;
    CHECK(constMat(0, 0) == 1, "constMat(0, 0) should be 1", errors);
    CHECK(constMat(1, 1) == 5, "constMat(1, 1) should be 5", errors);
    CHECK(constMat(2, 2) == 9, "constMat(2, 2) should be 9", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(40) << "[1]. Test Operator Access."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(40) << "[1]. Test Operator Access."
                  << RED << "[Failed]" << RESET << std::endl;
        // Print the collected error messages
        std::cout << errors.str() << std::endl;
    }
}

// Test Function 2: Getter/Setter Methods
void test_get_set() {
    // Initialize a local stringstream to collect error messages
    std::stringstream errors;

    // Initialize a 3x3 matrix and set values using set()
    matrix_t<int> mat(3, 3);
    mat.set(0, 0, 1);
    mat.set(0, 1, 2);
    mat.set(0, 2, 3);
    mat.set(1, 0, 4);
    mat.set(1, 1, 5);
    mat.set(1, 2, 6);
    mat.set(2, 0, 7);
    mat.set(2, 1, 8);
    mat.set(2, 2, 9);

    // Perform checks using the CHECK macro, passing the errors stream
    CHECK(mat.get(0, 0) == 1, "mat.get(0, 0) should be 1", errors);
    CHECK(mat.get(1, 1) == 5, "mat.get(1, 1) should be 5", errors);
    CHECK(mat.get(2, 2) == 9, "mat.get(2, 2) should be 9", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(40) << "[2]. Test Getter/Setter Methods."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(40) << "[2]. Test Getter/Setter Methods."
                  << RED << "[Failed]" << RESET << std::endl;
        // Print the collected error messages
        std::cout << errors.str() << std::endl;
    }
}

// Test Function 3: Matrix Save Method
void test_save() {
    // Initialize a local stringstream to collect error messages
    std::stringstream errors;

    // Create a 3x3 matrix and set values
    matrix_t<int> mat(3, 3);
    mat.set(0, 0, 1);
    mat.set(0, 1, 2);
    mat.set(0, 2, 3);
    mat.set(1, 0, 4);
    mat.set(1, 1, 5);
    mat.set(1, 2, 6);
    mat.set(2, 0, 7);
    mat.set(2, 1, 8);
    mat.set(2, 2, 9);

    // Save the matrix to a file
    std::string filename = "test_matrix.txt";
    mat.save(filename);

    // Read the file and verify its contents
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        errors << RED << "Failure: Unable to open file " << filename << RESET << std::endl;
        ++total_failures;
    } else {
        int value;
        // Perform checks using the CHECK macro, passing the errors stream
        CHECK(infile >> value && value == 1, "First value should be 1", errors);
        CHECK(infile >> value && value == 2, "Second value should be 2", errors);
        CHECK(infile >> value && value == 3, "Third value should be 3", errors);
        CHECK(infile >> value && value == 4, "Fourth value should be 4", errors);
        CHECK(infile >> value && value == 5, "Fifth value should be 5", errors);
        CHECK(infile >> value && value == 6, "Sixth value should be 6", errors);
        CHECK(infile >> value && value == 7, "Seventh value should be 7", errors);
        CHECK(infile >> value && value == 8, "Eighth value should be 8", errors);
        CHECK(infile >> value && value == 9, "Ninth value should be 9", errors);

        infile.close();

        // Clean up the test file
        if (std::remove(filename.c_str()) != 0) {
            errors << RED << "Failure: Unable to delete file " << filename << RESET << std::endl;
            ++total_failures;
        }
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(40) << "[3]. Test Matrix Save Method."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(40) << "[3]. Test Matrix Save Method."
                  << RED << "[Failed]" << RESET << std::endl;
        // Print the collected error messages
        std::cout << errors.str()<< std::endl;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "Inside Test.\n" << std::endl;

    /*
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }
    matrix_t<double> data(argv[1]);
    std::cout << "Matrix Size: (" << data.rows() << "," << data.cols() << ")" << std::endl;
    */

    std::cout << "Testing Matrix Methods." << std::endl;

    // Run test functions
    test_operator_access();
    test_get_set();
    test_save();

    std::cout << std::endl;

    // Summary of test results
    if (total_failures == 0) {
        std::cout << GREEN << "All tests passed." << RESET << std::endl;
    } else {
        std::cout << RED << "Some tests failed. Total Failures: " << total_failures << RESET << std::endl;
    }

    return (total_failures == 0) ? 0 : 1;
}
