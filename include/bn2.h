#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <atomic>

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

class DependencyTable {
    size_t m;        // Number of rows
    size_t n;        // Number of columns
    bool* data;      // Pointer to the dynamically allocated bool array

    public:
    // Default constructor
    DependencyTable() : m(0), n(0), data(nullptr) {}

    // Constructor: Initializes the dependency table with given rows and columns
    DependencyTable(size_t total_task_rows, size_t total_task_cols) : m(0), n(0), data(nullptr) {
        init(total_task_rows, total_task_cols);
    }

    // Destructor: Frees the dynamically allocated memory
    ~DependencyTable() {
        delete[] data;
    }

    // Deleted copy constructor and copy assignment to prevent accidental copying
    DependencyTable(const DependencyTable&) = delete;
    DependencyTable& operator=(const DependencyTable&) = delete;

    // Move constructor
    DependencyTable(DependencyTable&& other) noexcept
        : m(other.m), n(other.n), data(other.data) {
        other.m = 0;
        other.n = 0;
        other.data = nullptr;
    }

    // Move assignment operator
    DependencyTable& operator=(DependencyTable&& other) noexcept {
        if (this != &other) {
            delete[] data;
            m = other.m;
            n = other.n;
            data = other.data;

            other.m = 0;
            other.n = 0;
            other.data = nullptr;
        }
        return *this;
    }

    // Initializes the dependency table with given rows and columns
    void init(size_t total_task_rows, size_t total_task_cols) {
        delete[] data; // Clean up existing data if any
        m = total_task_rows;
        n = total_task_cols;
        data = new bool[m * n]();

        for (size_t i = 0; i < m * n; ++i) {
            data[i] = false;
        }
    }

    // Retrieves the dependency value at (i, j)
    inline bool getDependency(size_t i, size_t j) const {
        size_t idx =  i * n + j;
        return data[idx];
    }

    // Sets the dependency value at (i, j)
    inline void setDependency(size_t i, size_t j, bool value) {
        size_t idx =  i * n + j;
        data[idx] = value;
    }

    // Overloaded operator() for safe indexing
    bool operator()(size_t i, size_t j) const {
        if (i >= m || j >= n) {
            throw std::out_of_range("Index out of bounds: (" + std::to_string(i) + ", " + std::to_string(j) + ")");
        }
        return getDependency(i, j);
    }

    // Overloaded operator() for safe setting with bounds checking
    void operator()(size_t i, size_t j, bool value) {
        if (i >= m || j >= n) {
            throw std::out_of_range("Index out of bounds: (" + std::to_string(i) + ", " + std::to_string(j) + ")");
        }
        setDependency(i, j, value);
    }

    // Prints the dependency table
    void printDependencyTable() const {
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                std::cout << (getDependency(i, j) ? "1 " : "0 ");
            }
            std::cout << "\n";
        }
    }

    // Accessors for rows and columns
    size_t rows() const { return m; }
    size_t cols() const { return n; }

};

class DependencyTableAtomic {
    size_t m;                   // Number of rows
    size_t n;                   // Number of columns
    std::atomic<bool>* data;    // Pointer to dynamically allocated atomic<bool> array

public:
    // Default constructor
    DependencyTableAtomic() : m(0), n(0), data(nullptr) {}

    // Constructor: Initializes the dependency table with given rows and columns
    DependencyTableAtomic(size_t total_task_rows, size_t total_task_cols)
        : m(0), n(0), data(nullptr)
    {
        init(total_task_rows, total_task_cols);
    }

    // Destructor: Frees the dynamically allocated memory
    ~DependencyTableAtomic() {
        delete[] data;
    }

    // Deleted copy constructor and copy assignment to prevent accidental copying
    DependencyTableAtomic(const DependencyTableAtomic&) = delete;
    DependencyTableAtomic& operator=(const DependencyTableAtomic&) = delete;

    // Move constructor
    DependencyTableAtomic(DependencyTableAtomic&& other) noexcept
        : m(other.m), n(other.n), data(other.data)
    {
        other.m = 0;
        other.n = 0;
        other.data = nullptr;
    }

    // Move assignment operator
    DependencyTableAtomic& operator=(DependencyTableAtomic&& other) noexcept {
        if (this != &other) {
            delete[] data;
            m = other.m;
            n = other.n;
            data = other.data;

            other.m = 0;
            other.n = 0;
            other.data = nullptr;
        }
        return *this;
    }

    // Initializes the dependency table with given rows and columns
    void init(size_t total_task_rows, size_t total_task_cols) {
        // Clean up existing data if any
        delete[] data;

        m = total_task_rows;
        n = total_task_cols;

        // Allocate new storage for atomic<bool>
        data = new std::atomic<bool>[m * n];

        // Initialize all values to false
        for (size_t i = 0; i < m * n; ++i) {
            data[i].store(false, std::memory_order_relaxed);
        }
    }

    // Retrieves the dependency value at (i, j) using an atomic load
    inline bool getDependency(size_t i, size_t j) const {
        size_t idx = i * n + j;
        // Acquire ordering to ensure we read the latest value from any writer
        return data[idx].load(std::memory_order_acquire);
    }

    // Sets the dependency value at (i, j) using an atomic store
    inline void setDependency(size_t i, size_t j, bool value) {
        size_t idx = i * n + j;
        // Release ordering to ensure that prior writes in this thread are visible
        // to other threads that subsequently acquire this location
        data[idx].store(value, std::memory_order_release);
    }

    // Overloaded operator() for safe indexing (read)
    bool operator()(size_t i, size_t j) const {
        if (i >= m || j >= n) {
            throw std::out_of_range("Index out of bounds: (" + std::to_string(i)
                                    + ", " + std::to_string(j) + ")");
        }
        return getDependency(i, j);
    }

    // Overloaded operator() for safe setting (write)
    void operator()(size_t i, size_t j, bool value) {
        if (i >= m || j >= n) {
            throw std::out_of_range("Index out of bounds: (" + std::to_string(i)
                                    + ", " + std::to_string(j) + ")");
        }
        setDependency(i, j, value);
    }

    // Prints the dependency table
    void printDependencyTable() const {
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                // For printing, an acquire load ensures we see the latest state
                bool val = data[i * n + j].load(std::memory_order_acquire);
                std::cout << (val ? "1 " : "0 ");
            }
            std::cout << "\n";
        }
    }

    // Accessors for rows and columns
    size_t rows() const { return m; }
    size_t cols() const { return n; }
};

struct Task {
    unsigned char type;
    unsigned char priority;
    bool enq_nxt_t1;
    size_t row_start;
    size_t row_end;
    size_t col_start;
    size_t col_end;
    size_t chunk_idx_i;
    size_t chunk_idx_j;
};