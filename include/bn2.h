#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <chrono>
#include <functional>
#include <type_traits>

#include <cmath>
#include <algorithm>

#include <mutex>
#include <optional>
#include <atomic>

// Helper function to get a string representation of the time unit.
template <typename Duration>
constexpr const char* get_time_unit() {
    if constexpr (std::is_same_v<Duration, std::chrono::nanoseconds>) {
        return "nanoseconds";
    } else if constexpr (std::is_same_v<Duration, std::chrono::microseconds>) {
        return "microseconds";
    } else if constexpr (std::is_same_v<Duration, std::chrono::milliseconds>) {
        return "milliseconds";
    } else if constexpr (std::is_same_v<Duration, std::chrono::seconds>) {
        return "seconds";
    } else {
        return "time units";
    }
}

// Function to measure execution time of a given function
// The default is std::chrono::nanoseconds.
template <typename TimeUnit = std::chrono::nanoseconds, typename Func, typename... Args>
auto measure_exec_time(Func&& func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();

    // Handle functions that return void separately.
    if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>) {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<TimeUnit>(end - start);
        std::cout << "Time elapsed: " << elapsed.count() << " " 
                  << get_time_unit<TimeUnit>() << "\n";
        // No value is returned.
    } else {
        auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<TimeUnit>(end - start);
        std::cout << "Time elapsed: " << elapsed.count() << " " 
                  << get_time_unit<TimeUnit>() << "\n";
        return result;
    }
}

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

class TaskTable {
private:
    int m;                     // number of task rows
    int n;                     // number of task columns
    std::vector<Task*> data;      // vector holding pointers to Task objects

public:
    TaskTable()
        : m(0), n(0)
    {
        // 'data' is initially empty.
    }

    // Parameterized constructor that calls init().
    template <typename T>
    TaskTable(int total_task_rows, int total_task_cols, int alpha, int beta, matrix_t<T>& mat)
        : m(0), n(0)
    {
        init(total_task_rows, total_task_cols, alpha, beta, mat);
    }

    ~TaskTable() {
        for (Task* t : data) {
            delete t;
        }
    }

    // Disallow copy construction and copy assignment.
    TaskTable(const TaskTable&) = delete;
    TaskTable& operator=(const TaskTable&) = delete;

    // (Optional) Use default move construction and move assignment.
    TaskTable(TaskTable&&) noexcept = default;
    TaskTable& operator=(TaskTable&&) noexcept = default;

    template <typename T>
    void init(int total_task_rows, int total_task_cols, int alpha, int beta, matrix_t<T>& mat) {
        // Delete any previously allocated tasks.
        for (Task* t : data) {
            delete t;
        }
        data.clear();

        m = total_task_rows;
        n = total_task_cols;
        data.resize(m * n, nullptr);

        int beta_div_alpha = beta / alpha;

        int ctr = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                Task* new_task = new Task();

                // Set the type and possibly the enq_nxt_t1 flag based on the indices and beta_by_alpha.
                if (i * beta_div_alpha <= j && j < (i+1) * beta_div_alpha){
                    new_task->type = 1;
                    ctr = i + 1;
                } else {
                    new_task->type = 2;
                    if (i == ctr && ((i-1) * beta_div_alpha <= j && j < i * beta_div_alpha)){
                        new_task->enq_nxt_t1 = true;
                    } else {
                        new_task->enq_nxt_t1 = false;
                    }
                }

                // If j is outside the designated range, free new_task and leave the cell as nullptr.
                if (j >= (i+1) * beta_div_alpha) {
                    delete new_task;
                    continue;
                }

                // Set the boundaries for the task.
                new_task->row_start   = alpha * j + 1;
                new_task->row_end     = std::min(alpha *(j + 1) + 1, mat.rows());
                new_task->col_start   = beta * i + 1;
                new_task->col_end     = std::min(beta  *(i + 1) + 1, mat.rows());
                new_task->chunk_idx_i = i;
                new_task->chunk_idx_j = j;

                // Store the task pointer in the vector.
                data[i * n + j] = new_task;
            }
        }
    }

    inline Task* getTask(int i, int j) const {
        return data[i * n + j];
    }

    // Overloaded operator() for accessing the task at (i, j) with bounds checking.
    Task* operator()(int i, int j) const {
        if (i >= m || j >= n)
            throw std::out_of_range("Index out of bounds in TaskTable::operator()");
        return data[i * n + j];
    }

    // Prints the task table.
    // For each cell, it prints the task type (or "N" if the pointer is nullptr).
    void display() const {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                Task* t = data[i * n + j];
                if (t)
                    std::cout << static_cast<int>(t->type) << " ";
                else
                    std::cout << "N ";
            }
            std::cout << "\n";
        }
    }

    // Accessors for the number of rows and columns.
    int rows() const { return m; }
    int cols() const { return n; }
};

template <class T>
class CircularQueueMtx {
    std::vector<T> buffer;      // Internal storage.
    const size_t capacity;      // Maximum number of elements.
    size_t front;               // Index of the first element.
    size_t rear;                // Index one past the last element.
    size_t count;               // Number of elements in the queue.
    mutable std::mutex mutex;   // Mutex for thread-safety.

    public:
    // Construct a circular queue with fixed capacity.
    explicit CircularQueueMtx(size_t capacity)
        : buffer(capacity), capacity(capacity),
          front(0), rear(0), count(0)
    { }

    // Returns true if the queue is empty.
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return count == 0;
    }

    // Returns true if the queue is full.
    bool full() const {
        std::lock_guard<std::mutex> lock(mutex);
        return count == capacity;
    }

    // Returns the current number of elements.
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return count;
    }

    // Push an element at the front of the queue.
    // Returns true if the operation was successful (i.e. the queue wasn’t full).
    bool push_front(const T &value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (count == capacity) {
            return false;  // Queue is full.
        }
        // Move front backward (circularly) and store the value.
        front = (front + capacity - 1) % capacity;
        buffer[front] = value;
        ++count;
        return true;
    }

    // Push an element at the back of the queue.
    // Returns true if the operation was successful (i.e. the queue wasn’t full).
    bool push_back(const T &value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (count == capacity) {
            return false;  // Queue is full.
        }
        buffer[rear] = value;
        rear = (rear + 1) % capacity;
        ++count;
        return true;
    }

    // An alias for push_back.
    inline bool push(const T &value) {
        return push_back(value);
    }

    // Pop an element from the front.
    // If the queue is empty, returns std::nullopt.
    std::optional<T> pop_front() {
        std::lock_guard<std::mutex> lock(mutex);
        if (count == 0) {
            return std::nullopt;  // Queue is empty.
        }
        T value = buffer[front];
        front = (front + 1) % capacity;
        --count;
        return value;
    }

    // Pop an element from the back.
    // If the queue is empty, returns std::nullopt.
    std::optional<T> pop_back() {
        std::lock_guard<std::mutex> lock(mutex);
        if (count == 0) {
            return std::nullopt;  // Queue is empty.
        }
        rear = (rear + capacity - 1) % capacity;
        T value = buffer[rear];
        --count;
        return value;
    }

    // An alias for pop_front.
    std::optional<T> pop() {
        return pop_front();
    }
};

template<class T>
class CircularQueueAtomic {
    T* buffer;          // Raw array storage for elements.
    const size_t capacity;    // Maximum number of elements allowed.

    // Two atomic counters (which may be negative) track the logical indices.
    std::atomic<int64_t> left;  // Points to the first valid element.
    std::atomic<int64_t> right; // Points to one past the last valid element.

    // A mutex used only for query operations.
    mutable std::mutex query_mutex;
    
public:
    // Constructs the deque with the given capacity.
    // (Capacity is the maximum number of elements that can be stored.)
    explicit CircularQueueAtomic(size_t cap)
      : capacity(cap), left(0), right(0)
    {
        // Allocate a raw array for storage.
        buffer = new T[capacity];
    }

    ~CircularQueueAtomic() {
        delete[] buffer;
    }

    // Returns true if the deque is empty.
    bool empty() const {
        std::lock_guard<std::mutex> lock(query_mutex);
        return (right.load(std::memory_order_acquire) - left.load(std::memory_order_acquire)) == 0;
    }

    // Returns true if the deque is full.
    bool full() const {
        std::lock_guard<std::mutex> lock(query_mutex);
        return (right.load(std::memory_order_acquire) - left.load(std::memory_order_acquire)) >= static_cast<int64_t>(capacity);
    }

    // Returns the current number of elements.
    size_t size() const {
        std::lock_guard<std::mutex> lock(query_mutex);
        return static_cast<size_t>(right.load(std::memory_order_acquire) - left.load(std::memory_order_acquire));
    }

    // ---- Lock-free update methods (using atomics & compare_exchange) ----

    // push_back: Append an element at the back.
    // Returns false if the deque is full.
    bool push_back(const T &value) {
        while (true) {
            int64_t current_right = right.load(std::memory_order_relaxed);
            int64_t current_left  = left.load(std::memory_order_acquire);
            if ((current_right - current_left) >= static_cast<int64_t>(capacity))
                return false;  // full

            if (right.compare_exchange_weak(current_right, current_right + 1,
                                            std::memory_order_acq_rel, std::memory_order_relaxed))
            {
                size_t index = static_cast<size_t>(current_right % capacity);
                buffer[index] = value;
                return true;
            }
        }
    }

    // push_front: Insert an element at the front.
    // Returns false if the deque is full.
    bool push_front(const T &value) {
        while (true) {
            int64_t current_left  = left.load(std::memory_order_relaxed);
            int64_t current_right = right.load(std::memory_order_acquire);
            if ((current_right - current_left) >= static_cast<int64_t>(capacity))
                return false;  // full

            int64_t new_left = current_left - 1;
            if (left.compare_exchange_weak(current_left, new_left,
                                           std::memory_order_acq_rel, std::memory_order_relaxed))
            {
                size_t index = static_cast<size_t>(((new_left % capacity) + capacity) % capacity);
                buffer[index] = value;
                return true;
            }
        }
    }

    // pop_front: Remove and return an element from the front.
    // Returns std::nullopt if the deque is empty.
    std::optional<T> pop_front() {
        while (true) {
            int64_t current_left  = left.load(std::memory_order_relaxed);
            int64_t current_right = right.load(std::memory_order_acquire);
            if (current_left == current_right)
                return std::nullopt;  // empty

            if (left.compare_exchange_weak(current_left, current_left + 1,
                                           std::memory_order_acq_rel, std::memory_order_relaxed))
            {
                size_t index = static_cast<size_t>(((current_left % capacity) + capacity) % capacity);
                T value = buffer[index];
                return value;
            }
        }
    }

    // pop_back: Remove and return an element from the back.
    // Returns std::nullopt if the deque is empty.
    std::optional<T> pop_back() {
        while (true) {
            int64_t current_right = right.load(std::memory_order_relaxed);
            int64_t current_left  = left.load(std::memory_order_acquire);
            if (current_left == current_right)
                return std::nullopt;  // empty

            int64_t new_right = current_right - 1;
            if (right.compare_exchange_weak(current_right, new_right,
                                            std::memory_order_acq_rel, std::memory_order_relaxed))
            {
                size_t index = static_cast<size_t>(((new_right % capacity) + capacity) % capacity);
                T value = buffer[index];
                return value;
            }
        }
    }

    // push: Alias for push_back so that the deque can be used as a normal FIFO queue.
    bool push(const T &value) {
        return push_back(value);
    }

    // pop: Alias for pop_front so that the deque can be used as a normal FIFO queue.
    std::optional<T> pop() {
        return pop_front();
    }
};
