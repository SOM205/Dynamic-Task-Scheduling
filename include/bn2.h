#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

template <class T>
class matrix_t {
private:
    int m;                  // Number of rows
    int n;                  // Number of columns
    std::vector<T> data;    // Flat vector to store matrix elements

public:
    // Default constructor
    matrix_t() : m(0), n(0), data() {}

    // Parameterized constructor
    matrix_t(int rows, int cols) : m(rows), n(cols), data(rows * cols) {}

    matrix_t(const std::string& filename) {
        read_matrix(filename);
    }

    // Method to read matrix from a file
    void read_matrix(const std::string& filename){
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            throw std::runtime_error("Error opening file: " + filename);
        }

        // Read matrix dimensions
        infile >> m >> n;
        if (infile.fail()) {
            throw std::runtime_error("Error reading matrix dimensions from file.");
        }

        // Resize the data vector to hold m * n elements
        data.resize(m * n);

        // Read matrix elements
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                infile >> data[i * n + j];
                if (infile.fail()) {
                    throw std::runtime_error("Error reading matrix value at (" +
                                            std::to_string(i) + ", " + std::to_string(j) + ").");
                }
            }
        }

        T extra;
        if (infile >> extra) {
            throw std::runtime_error("Extra data found in the file after reading the matrix.");
        }

        infile.close();
    }

    // Accessor methods
    int rows() const { return m; }
    int cols() const { return n; }

    // Operator to access elements
    T& operator()(int row, int col) {
        if (row < 0 || row >= m || col < 0 || col >= n) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row * n + col];
    }

    const T& operator()(int row, int col) const {
        if (row < 0 || row >= m || col < 0 || col >= n) {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row * n + col];
    }

    inline T get(int row, int col) const {
        return data[row * n + col];
    }

    inline void set(int row, int col, T value) {
        data[row * n + col] = value;
    }

    inline T* data_ptr() {
        return data.data();
    }

    inline const T* data_ptr() const {
        return data.data();
    }

    void display() const{
        if (m == 0 || n == 0 || data.empty()) {
            std::cerr << "Matrix is not allocated.\n";
            return;
        }

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << data[i * n + j] << " ";
            }
        std::cout << "\n";
        }
    }

    void save(const std::string& filename) const {
        // Check if the matrix has been allocated (i.e., has non-zero dimensions)
        if (m == 0 || n == 0 || data.empty()) {
            std::cerr << "Matrix is not allocated.\n";
            return;
        }

        std::ofstream outfile(filename);
        if (!outfile.is_open()) {
            throw std::runtime_error("Error opening file for writing: " + filename);
        }

        // Write the matrix data
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                outfile << data[i * n + j];
                if (j < n - 1) {
                    outfile << " ";
                }
            }
            outfile << "\n";
            if (outfile.fail()) {
                throw std::runtime_error("Error writing matrix data to file: " + filename);
            }
        }

        outfile.close();
    }
};

