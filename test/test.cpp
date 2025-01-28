#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>    // Added for std::stringstream
#include <cstdlib>    // Added for std::remove
#include "bn2.h"     // Include the matrix_t class

// Define color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

// Global counter for total failures
int total_failures = 0;

// Redefine the CHECK macro to accept a stream parameter
#define CHECK(condition, message, stream) \
    if (!(condition)) { \
        (stream) << RED << "Failure: " << message << RESET << std::endl; \
        ++total_failures; \
    }


// ========================= Matrix_t Tests =========================

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
        std::cout << std::left << std::setw(50) << "[MT1]. Test Operator Access."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[MT1]. Test Operator Access."
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
        std::cout << std::left << std::setw(50) << "[MT2]. Test Getter/Setter Methods."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[MT2]. Test Getter/Setter Methods."
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
        std::cout << std::left << std::setw(50) << "[MT3]. Test Matrix Save Method."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[MT3]. Test Matrix Save Method."
                  << RED << "[Failed]" << RESET << std::endl;
        // Print the collected error messages
        std::cout << errors.str()<< std::endl;
    }
}

// ========================= DependencyTable Tests =========================

// Test Function 4: Default Constructor
void test_default_constructor() {
    std::stringstream errors;

    DependencyTable dt;

    // Check initial dimensions
    CHECK(dt.rows() == 0, "Default constructor should initialize rows to 0", errors);
    CHECK(dt.cols() == 0, "Default constructor should initialize columns to 0", errors);

    // Check data pointer is nullptr (Not directly accessible, so we rely on rows and cols)
    // Alternatively, if you add a method to check data pointer, you can test it here

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT1]. Test Default Constructor."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT1]. Test Default Constructor."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 5: Parameterized Constructor
void test_parameterized_constructor() {
    std::stringstream errors;

    size_t rows = 3;
    size_t cols = 4;
    DependencyTable dt(rows, cols);

    // Check dimensions
    CHECK(dt.rows() == rows, "Parameterized constructor should set correct number of rows", errors);
    CHECK(dt.cols() == cols, "Parameterized constructor should set correct number of columns", errors);

    // Check all dependencies are initialized to false (default)
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            CHECK(dt.getDependency(i, j) == false, "All dependencies should be initialized to false", errors);
        }
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT2]. Test Parameterized Constructor."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT2]. Test Parameterized Constructor."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 6: Init Method
void test_init() {
    std::stringstream errors;

    DependencyTable dt;
    size_t initial_rows = 2;
    size_t initial_cols = 2;
    dt.init(initial_rows, initial_cols);

    // Check dimensions after init
    CHECK(dt.rows() == initial_rows, "Init method should set correct number of rows", errors);
    CHECK(dt.cols() == initial_cols, "Init method should set correct number of columns", errors);

    // Check all dependencies are initialized to false
    for (size_t i = 0; i < initial_rows; ++i) {
        for (size_t j = 0; j < initial_cols; ++j) {
            CHECK(dt.getDependency(i, j) == false, "All dependencies should be initialized to false after init", errors);
        }
    }

    // Re-initialize with different dimensions
    size_t new_rows = 4;
    size_t new_cols = 5;
    dt.init(new_rows, new_cols);

    // Check new dimensions
    CHECK(dt.rows() == new_rows, "Init method should correctly reinitialize number of rows", errors);
    CHECK(dt.cols() == new_cols, "Init method should correctly reinitialize number of columns", errors);

    // Check all dependencies are reset to false
    for (size_t i = 0; i < new_rows; ++i) {
        for (size_t j = 0; j < new_cols; ++j) {
            CHECK(dt.getDependency(i, j) == false, "All dependencies should be reset to false after re-init", errors);
        }
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT3]. Test Init Method."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT3]. Test Init Method."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 7: Move Constructor
void test_move_constructor() {
    std::stringstream errors;

    size_t rows = 3;
    size_t cols = 3;
    DependencyTable dt1(rows, cols);
    dt1.setDependency(0, 0, true);
    dt1.setDependency(1, 1, true);
    dt1.setDependency(2, 2, true);

    // Move construct dt2 from dt1
    DependencyTable dt2(std::move(dt1));

    // Check dt2 has the data
    CHECK(dt2.rows() == rows, "Move constructor should transfer correct number of rows", errors);
    CHECK(dt2.cols() == cols, "Move constructor should transfer correct number of columns", errors);
    CHECK(dt2.getDependency(0, 0) == true, "Move constructor should transfer dependency at (0,0)", errors);
    CHECK(dt2.getDependency(1, 1) == true, "Move constructor should transfer dependency at (1,1)", errors);
    CHECK(dt2.getDependency(2, 2) == true, "Move constructor should transfer dependency at (2,2)", errors);

    // Check dt1 is in a valid state (rows and cols should be 0)
    CHECK(dt1.rows() == 0, "After move, source should have rows set to 0", errors);
    CHECK(dt1.cols() == 0, "After move, source should have columns set to 0", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT4]. Test Move Constructor."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT4]. Test Move Constructor."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 8: Move Assignment Operator
void test_move_assignment() {
    std::stringstream errors;

    size_t rows1 = 2;
    size_t cols1 = 2;
    DependencyTable dt1(rows1, cols1);
    dt1.setDependency(0, 0, true);
    dt1.setDependency(1, 1, true);

    size_t rows2 = 4;
    size_t cols2 = 4;
    DependencyTable dt2(rows2, cols2);
    dt2.setDependency(0, 0, true);
    dt2.setDependency(3, 3, true);

    // Move assign dt1 to dt2
    dt2 = std::move(dt1);

    // Check dt2 has the data from dt1
    CHECK(dt2.rows() == rows1, "Move assignment should transfer correct number of rows", errors);
    CHECK(dt2.cols() == cols1, "Move assignment should transfer correct number of columns", errors);
    CHECK(dt2.getDependency(0, 0) == true, "Move assignment should transfer dependency at (0,0)", errors);
    CHECK(dt2.getDependency(1, 1) == true, "Move assignment should transfer dependency at (1,1)", errors);

    // Check dt1 is in a valid state
    CHECK(dt1.rows() == 0, "After move assignment, source should have rows set to 0", errors);
    CHECK(dt1.cols() == 0, "After move assignment, source should have columns set to 0", errors);

    // Ensure dt2 no longer has old data (size2 was 4x4, now 2x2)
    // Attempting to access (3,3) should throw
    try {
        dt2.getDependency(3, 3);
        errors << RED << "Failure: Accessing (3,3) should throw after move assignment." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected exception
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type when accessing out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT5]. Test Move Assignment Operator."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT5]. Test Move Assignment Operator."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 9: Get and Set Dependency
void test_get_set_dependency() {
    std::stringstream errors;

    size_t rows = 3;
    size_t cols = 3;
    DependencyTable dt(rows, cols);

    // Set some dependencies
    dt.setDependency(0, 0, true);
    dt.setDependency(1, 1, true);
    dt.setDependency(2, 2, true);
    dt.setDependency(0, 2, true);

    // Get and check dependencies
    CHECK(dt.getDependency(0, 0) == true, "Dependency at (0,0) should be true", errors);
    CHECK(dt.getDependency(1, 1) == true, "Dependency at (1,1) should be true", errors);
    CHECK(dt.getDependency(2, 2) == true, "Dependency at (2,2) should be true", errors);
    CHECK(dt.getDependency(0, 2) == true, "Dependency at (0,2) should be true", errors);
    CHECK(dt.getDependency(0, 1) == false, "Dependency at (0,1) should be false", errors);
    CHECK(dt.getDependency(1, 0) == false, "Dependency at (1,0) should be false", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT6]. Test Get and Set Dependency."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT6]. Test Get and Set Dependency."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 10: Operator Overloading
void test_operator_overloading() {
    std::stringstream errors;

    size_t rows = 2;
    size_t cols = 2;
    DependencyTable dt(rows, cols);

    // Use operator() to set dependencies
    try {
        dt(0, 0, true);
        dt(1, 1, true);
        dt(0, 1, false);
        dt(1, 0, true);
    } catch (...) {
        errors << RED << "Failure: Operator() threw an unexpected exception during setting." << RESET << std::endl;
        ++total_failures;
    }

    // Use operator() to get dependencies and verify
    try {
        CHECK(dt(0, 0) == true, "Operator() should return true for (0,0)", errors);
        CHECK(dt(1, 1) == true, "Operator() should return true for (1,1)", errors);
        CHECK(dt(0, 1) == false, "Operator() should return false for (0,1)", errors);
        CHECK(dt(1, 0) == true, "Operator() should return true for (1,0)", errors);
    } catch (...) {
        errors << RED << "Failure: Operator() threw an unexpected exception during getting." << RESET << std::endl;
        ++total_failures;
    }

    // Attempt to access out-of-bounds using operator() and expect exceptions
    try {
        dt(2, 2); // Should throw
        errors << RED << "Failure: Accessing (2,2) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected exception
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type when accessing out-of-bounds with operator()." << RESET << std::endl;
        ++total_failures;
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT7]. Test Operator Overloading."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT7]. Test Operator Overloading."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// Test Function 11: Out of Bounds Access
void test_out_of_bounds() {
    std::stringstream errors;

    size_t rows = 2;
    size_t cols = 2;
    DependencyTable dt(rows, cols);

    /*
    // Test getDependency out-of-bounds
    try {
        dt.getDependency(2, 0);
        errors << RED << "Failure: getDependency(2,0) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected exception
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type in getDependency out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Test setDependency out-of-bounds
    try {
        dt.setDependency(0, 2, true);
        errors << RED << "Failure: setDependency(0,2) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected exception
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type in setDependency out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }
    */

    // Test operator() get out-of-bounds
    try {
        dt(0, 3);
        errors << RED << "Failure: operator()(0,3) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected exception
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type in operator() get out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Test operator() set out-of-bounds
    try {
        dt(3, 0, true);
        errors << RED << "Failure: operator()(3,0, true) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected exception
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type in operator() set out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50) << "[DT8]. Test Out of Bounds Access."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50) << "[DT8]. Test Out of Bounds Access."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str()<< std::endl;
    }
}

// =================== DependencyTableAtomic Tests ======================== //

// Test 1: Default Constructor
void test_default_constructor_atomic() {
    std::stringstream errors;

    DependencyTableAtomic dt;

    // Check initial dimensions
    CHECK(dt.rows() == 0, "Default constructor should initialize rows to 0", errors);
    CHECK(dt.cols() == 0, "Default constructor should initialize cols to 0", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA1]. Test Default Constructor."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA1]. Test Default Constructor."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 2: Parameterized Constructor
void test_parameterized_constructor_atomic() {
    std::stringstream errors;

    size_t rows = 3;
    size_t cols = 4;
    DependencyTableAtomic dt(rows, cols);

    // Check dimensions
    CHECK(dt.rows() == rows, "Parameterized constructor should set correct number of rows", errors);
    CHECK(dt.cols() == cols, "Parameterized constructor should set correct number of columns", errors);

    // Check all dependencies are initialized to false by default
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            CHECK(dt.getDependency(i, j) == false,
                  "All dependencies should be initialized to false", errors);
        }
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA2]. Test Parameterized Constructor."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA2]. Test Parameterized Constructor."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 3: Init Method
void test_init_atomic() {
    std::stringstream errors;

    DependencyTableAtomic dt;
    size_t initial_rows = 2;
    size_t initial_cols = 2;
    dt.init(initial_rows, initial_cols);

    // Check dimensions after init
    CHECK(dt.rows() == initial_rows, "Init method should set correct number of rows", errors);
    CHECK(dt.cols() == initial_cols, "Init method should set correct number of columns", errors);

    // Check all dependencies are initialized to false
    for (size_t i = 0; i < initial_rows; ++i) {
        for (size_t j = 0; j < initial_cols; ++j) {
            CHECK(dt.getDependency(i, j) == false,
                  "All dependencies should be initialized to false after init", errors);
        }
    }

    // Re-initialize with different dimensions
    size_t new_rows = 4;
    size_t new_cols = 5;
    dt.init(new_rows, new_cols);

    // Check new dimensions
    CHECK(dt.rows() == new_rows, "Init method should correctly reinitialize number of rows", errors);
    CHECK(dt.cols() == new_cols, "Init method should correctly reinitialize number of columns", errors);

    // Check all dependencies are reset to false
    for (size_t i = 0; i < new_rows; ++i) {
        for (size_t j = 0; j < new_cols; ++j) {
            CHECK(dt.getDependency(i, j) == false,
                  "All dependencies should be reset to false after re-init", errors);
        }
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA3]. Test Init Method."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA3]. Test Init Method."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 4: Move Constructor
void test_move_constructor_atomic() {
    std::stringstream errors;

    size_t rows = 3;
    size_t cols = 3;
    DependencyTableAtomic dt1(rows, cols);
    dt1.setDependency(0, 0, true);
    dt1.setDependency(1, 1, true);
    dt1.setDependency(2, 2, true);

    // Move construct dt2 from dt1
    DependencyTableAtomic dt2(std::move(dt1));

    // Check dt2 has the data
    CHECK(dt2.rows() == rows, "Move constructor should transfer correct number of rows", errors);
    CHECK(dt2.cols() == cols, "Move constructor should transfer correct number of cols", errors);
    CHECK(dt2.getDependency(0, 0) == true,  "Move constructor should transfer dependency at (0,0)", errors);
    CHECK(dt2.getDependency(1, 1) == true,  "Move constructor should transfer dependency at (1,1)", errors);
    CHECK(dt2.getDependency(2, 2) == true,  "Move constructor should transfer dependency at (2,2)", errors);

    // Check dt1 is in a valid moved-from state
    CHECK(dt1.rows() == 0, "After move, source should have rows set to 0", errors);
    CHECK(dt1.cols() == 0, "After move, source should have cols set to 0", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA4]. Test Move Constructor."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA4]. Test Move Constructor."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 5: Move Assignment Operator
void test_move_assignment_atomic() {
    std::stringstream errors;

    size_t rows1 = 2;
    size_t cols1 = 2;
    DependencyTableAtomic dt1(rows1, cols1);
    dt1.setDependency(0, 0, true);
    dt1.setDependency(1, 1, true);

    size_t rows2 = 4;
    size_t cols2 = 4;
    DependencyTableAtomic dt2(rows2, cols2);
    dt2.setDependency(0, 0, true);
    dt2.setDependency(3, 3, true);

    // Move assign dt2 <- dt1
    dt2 = std::move(dt1);

    // Check dt2 has the data from dt1
    CHECK(dt2.rows() == rows1, "Move assignment should transfer correct number of rows", errors);
    CHECK(dt2.cols() == cols1, "Move assignment should transfer correct number of cols", errors);
    CHECK(dt2.getDependency(0, 0) == true,  "Move assignment should transfer dependency at (0,0)", errors);
    CHECK(dt2.getDependency(1, 1) == true,  "Move assignment should transfer dependency at (1,1)", errors);

    // Check dt1 is in a valid moved-from state
    CHECK(dt1.rows() == 0, "After move assignment, source should have rows set to 0", errors);
    CHECK(dt1.cols() == 0, "After move assignment, source should have cols set to 0", errors);

    // Ensure dt2 no longer has old data (it should be 2x2 now)
    // Accessing (3,3) should throw
    try {
        dt2.getDependency(3, 3);
        errors << RED << "Failure: Accessing (3,3) should throw after move assignment." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type when accessing out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA5]. Test Move Assignment Operator."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA5]. Test Move Assignment Operator."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 6: Get and Set Dependency
void test_get_set_dependency_atomic() {
    std::stringstream errors;

    size_t rows = 3;
    size_t cols = 3;
    DependencyTableAtomic dt(rows, cols);

    // Set some dependencies
    dt.setDependency(0, 0, true);
    dt.setDependency(1, 1, true);
    dt.setDependency(2, 2, true);
    dt.setDependency(0, 2, true);

    // Verify using getDependency
    CHECK(dt.getDependency(0, 0) == true,  "Dependency at (0,0) should be true", errors);
    CHECK(dt.getDependency(1, 1) == true,  "Dependency at (1,1) should be true", errors);
    CHECK(dt.getDependency(2, 2) == true,  "Dependency at (2,2) should be true", errors);
    CHECK(dt.getDependency(0, 2) == true,  "Dependency at (0,2) should be true", errors);
    CHECK(dt.getDependency(0, 1) == false, "Dependency at (0,1) should be false", errors);
    CHECK(dt.getDependency(1, 0) == false, "Dependency at (1,0) should be false", errors);

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA6]. Test Get and Set Dependency."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA6]. Test Get and Set Dependency."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 7: Operator Overloading
void test_operator_overloading_atomic() {
    std::stringstream errors;

    size_t rows = 2;
    size_t cols = 2;
    DependencyTableAtomic dt(rows, cols);

    // Use operator() to set dependencies
    try {
        dt(0, 0, true);
        dt(1, 1, true);
        dt(0, 1, false);
        dt(1, 0, true);
    } catch (...) {
        errors << RED << "Failure: Operator() threw an unexpected exception during set." << RESET << std::endl;
        ++total_failures;
    }

    // Use operator() to get dependencies
    try {
        CHECK(dt(0, 0) == true,  "Operator() should return true for (0,0)", errors);
        CHECK(dt(1, 1) == true,  "Operator() should return true for (1,1)", errors);
        CHECK(dt(0, 1) == false, "Operator() should return false for (0,1)", errors);
        CHECK(dt(1, 0) == true,  "Operator() should return true for (1,0)", errors);
    } catch (...) {
        errors << RED << "Failure: Operator() threw an unexpected exception during get." << RESET << std::endl;
        ++total_failures;
    }

    // Attempt out-of-bounds access for get
    try {
        dt(2, 2); // Should throw
        errors << RED << "Failure: Accessing (2,2) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type when accessing out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA7]. Test Operator Overloading."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA7]. Test Operator Overloading."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
    }
}

// Test 8: Out of Bounds Access
void test_out_of_bounds_atomic() {
    std::stringstream errors;

    size_t rows = 2;
    size_t cols = 2;
    DependencyTableAtomic dt(rows, cols);

    // Operator() get out-of-bounds
    try {
        dt(0, 3); // Should throw
        errors << RED << "Failure: operator()(0,3) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type in operator() get out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Operator() set out-of-bounds
    try {
        dt(3, 0, true); // Should throw
        errors << RED << "Failure: operator()(3,0,true) should throw an exception." << RESET << std::endl;
        ++total_failures;
    } catch (const std::out_of_range&) {
        // Expected
    } catch (...) {
        errors << RED << "Failure: Unexpected exception type in operator() set out-of-bounds." << RESET << std::endl;
        ++total_failures;
    }

    // Determine if the test passed or failed
    if (errors.str().empty()) {
        std::cout << std::left << std::setw(50)
                  << "[DTA8]. Test Out of Bounds Access."
                  << GREEN << "[Passed]" << RESET << std::endl;
    } else {
        std::cout << std::left << std::setw(50)
                  << "[DTA8]. Test Out of Bounds Access."
                  << RED << "[Failed]" << RESET << std::endl;
        std::cout << errors.str() << std::endl;
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

    std::cout << YELLOW << "Testing Matrix Methods." << RESET << std::endl;

    // Run matrix_t test functions
    test_operator_access();
    test_get_set();
    test_save();

    std::cout << YELLOW << "\nTesting DependencyTable Methods." << RESET << std::endl;

    // Run DependencyTable test functions
    test_default_constructor();
    test_parameterized_constructor();
    test_init();
    test_move_constructor();
    test_move_assignment();
    test_get_set_dependency();
    test_operator_overloading();
    test_out_of_bounds();

    std::cout << YELLOW << "\nTesting DependencyTableAtomic Methods." << RESET << std::endl;

    // Run DependencyTableAtomic test functions
    test_default_constructor_atomic();
    test_parameterized_constructor_atomic();
    test_init_atomic();
    test_move_constructor_atomic();
    test_move_assignment_atomic();
    test_get_set_dependency_atomic();
    test_operator_overloading_atomic();
    test_out_of_bounds_atomic();

    std::cout << std::endl;

    // Summary of test results
    if (total_failures == 0) {
        std::cout << GREEN << "All tests passed." << RESET << std::endl;
    } else {
        std::cout << RED << "Some tests failed. Total Failures: " << total_failures << RESET << std::endl;
    }

    return (total_failures == 0) ? 0 : 1;
}