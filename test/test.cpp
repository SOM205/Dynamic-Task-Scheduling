#include <iostream>
#include <cassert>
#include <string>
#include "bn2.h"

#define GREEN "\033[32m"
#define RESET "\033[0m"

void test_operator_access() {
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

    assert(mat(0, 0) == 1);
    assert(mat(1, 1) == 5);
    assert(mat(2, 2) == 9);

    const matrix_t<int>& constMat = mat;
    assert(constMat(0, 0) == 1);
    assert(constMat(1, 1) == 5);
    assert(constMat(2, 2) == 9);

    std::cout << GREEN << "[Test] Operator Access Passed." << RESET << std::endl;
}

void test_get_set() {
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

    assert(mat.get(0, 0) == 1);
    assert(mat.get(1, 1) == 5);
    assert(mat.get(2, 2) == 9);

    std::cout << GREEN << "[Test] Getter/Setter Methods Passed." << RESET << std::endl;
}

int main(int argc, char *argv[]){
    std::cout << "[2]. Inside Test.\n" << std::endl;

    /*
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }
    matrix_t<double> data(argv[1]);
    std::cout << "Matrix Size: (" << data.rows() << "," << data.cols() << ")" << std::endl;
    */

    std::cout << "Testing Matrix Methods." << std::endl;
    test_operator_access();
    test_get_set();
    std::cout << std::endl;

    std::cout << GREEN << "All tests passed." << RESET << std::endl;

    return 0;
}