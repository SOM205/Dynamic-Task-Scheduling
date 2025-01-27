#include <iostream>
#include <vector>
#include <string>
#include <bn2.h>

int main(int argc, char *argv[]){
    std::cout << "[2]. Inside main." << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }
    
}