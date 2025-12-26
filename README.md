# Student Information System - Red-Black Tree Implementation

## 📋 Project Overview

This project implements a **Student Information System (SIS)** using a **Red-Black Tree** data structure in C++. Unlike simple arrays or linked lists that become slow with large datasets, this implementation maintains **O(log n)** time complexity for all operations, ensuring lightning-fast performance even with thousands of student records.

---

## 🎯 Project Objectives

1. **Efficient Data Management**: Handle large student databases with guaranteed logarithmic time complexity
2. **Self-Balancing Structure**: Automatic tree balancing after insertions and deletions
3. **Range Queries**: Leverage the sorted nature of the tree for efficient range-based searches
4. **Professional Architecture**: Demonstrate Object-Oriented Programming principles with proper encapsulation

---

## 🏗️ Architecture

### Project Structure

```
student_records/
├── RBTree.h            # RBTree class with nested Student and Node classes
├── RBTree.cpp          # Implementation of all classes
├── main.cpp            # Console application entry point
├── main_gui.cpp        # GUI application entry point (Qt)
├── MainWindow.h        # GUI main window header
├── MainWindow.cpp      # GUI main window implementation
├── CMakeLists.txt      # Build configuration (supports both console & GUI)
└── README.md           # This file
```

### Class Hierarchy

```
┌─────────────────────────────────┐
│          RBTree                 │
│  ┌───────────────────────────┐  │
│  │ Nested: Student           │  │  - Data container
│  └───────────────────────────┘  │
│  ┌───────────────────────────┐  │
│  │ Nested: Node              │  │  - Tree node (uses Student)
│  └───────────────────────────┘  │
│                                 │
│  Tree operations and logic      │
└─────────────────────────────────┘
```

---

## 📚 Core Components

### 1. RBTree::Student Class (Nested in RBTree)

**Purpose**: Encapsulates student information with proper data hiding.

**Private Members**:
- `int id` - Unique identifier (primary key)
- `string name` - Student full name
- `string dept` - Department name
- `double gpa` - Grade point average

**Public Methods**:
- Constructors: Default and parameterized
- Getters: `getId()`, `getName()`, `getDept()`, `getGpa()`
- Setters: `setId()`, `setName()`, `setDept()`, `setGpa()`

### 2. RBTree::Node Class (Nested in RBTree)

**Purpose**: Represents a single node in the Red-Black Tree.

**Private Members**:
- `Student data` - The student record
- `Color color` - RED or BLACK (RBT property)
- `Node* left` - Left child pointer
- `Node* right` - Right child pointer
- `Node* parent` - Parent pointer (essential for RBT rotations)

**Key Feature**: Uses `friend class RBTree` to allow the parent RBTree class direct access to private members while maintaining encapsulation from external code.

### 3. RBTree Class

**Purpose**: Implements the Red-Black Tree with all balancing logic. Contains Student and Node as nested classes for better encapsulation.

**Core Operations**:

| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `insert()` | O(log n) | Add student with auto-balancing |
| `search()` | O(log n) | Find student by ID |
| `delete()` | O(log n) | Remove student with rebalancing |
| `printRange()` | O(log n + k) | Print k students in ID range |
| `inorder()` | O(n) | Display all students sorted |

---

## 🔧 Red-Black Tree Properties

The implementation maintains these five critical properties:

1. **Property 1**: Every node is either RED or BLACK
2. **Property 2**: The root is always BLACK
3. **Property 3**: All leaves (NULL nodes) are BLACK
4. **Property 4**: RED nodes cannot have RED children (no "double red")
5. **Property 5**: All paths from root to leaves contain the same number of BLACK nodes

These properties guarantee the tree remains balanced with height ≤ 2 log(n+1).

---

## ⚙️ Key Algorithms

### Insertion Algorithm

1. **Standard BST Insert**: Insert node as RED
2. **Fix Violations**: Check for "Double Red" violations
3. **Rebalancing Cases**:
   - **Case 1**: Uncle is RED → Recolor
   - **Case 2**: Uncle is BLACK, node is inner child → Rotate
   - **Case 3**: Uncle is BLACK, node is outer child → Rotate and recolor
4. **Final Step**: Ensure root is BLACK

### Rotation Operations

**Left Rotation**:
```
    x                y
   / \              / \
  a   y     →      x   c
     / \          / \
    b   c        a   b
```

**Right Rotation**:
```
    y                x
   / \              / \
  x   c     →      a   y
 / \                  / \
a   b                b   c
```

### Range Query (Killer Feature)

Uses **pruned in-order traversal**:
```cpp
if (node->id > minID) → explore left
if (minID ≤ node->id ≤ maxID) → print
if (node->id < maxID) → explore right
```

This is **more efficient than hash tables** for range queries because the sorted structure allows skipping entire subtrees.

---

## 🚀 Features

### Menu Options

1. **Add Student** - Insert with validation for duplicate IDs
2. **Search Student by ID** - Fast O(log n) lookup
3. **Print Students in ID Range** - Display students between two IDs (sorted)
4. **Delete Student** - Remove with automatic rebalancing
5. **Display All Students (Sorted)** - In-order traversal output
6. **Visualize Tree Structure** - Debug view showing colors and structure
7. **Exit** - Graceful program termination

### Input Validation

- Prevents duplicate student IDs
- Handles not-found cases gracefully
- Uses sentinel nodes (TNULL) to avoid null pointer checks

---

## 🔨 Building and Running

### Requirements

- C++ compiler with C++20 support
- CMake 4.1 or higher
- CLion IDE (recommended) or command-line tools

### Build Instructions

```bash
# Using CMake
mkdir build
cd build
cmake ..
cmake --build .

# Run
./student_records
```

### Using CLion

1. Open the project folder in CLion
2. CLion will automatically detect CMakeLists.txt
3. Click the green "Run" button or press Shift+F10

---

## 💡 Example Usage

```
1. Add Student
   ID: 101
   Name: Ahmed Ali
   Department: Computer Science
   GPA: 3.85

2. Add Student
   ID: 150
   Name: Sara Mohammed
   Department: Engineering
   GPA: 3.92

3. Print Students in ID Range
   Min ID: 100
   Max ID: 200
   
   Output:
   ID: 101 | Name: Ahmed Ali | Dept: Computer Science | GPA: 3.85
   ID: 150 | Name: Sara Mohammed | Dept: Engineering | GPA: 3.92
```

---

## 🧪 Testing Recommendations

### Test Cases to Verify

1. **Sequential Insertion**: Insert IDs 1, 2, 3, 4, 5 → Tree should stay balanced
2. **Reverse Insertion**: Insert IDs 5, 4, 3, 2, 1 → Tree should stay balanced
3. **Duplicate Prevention**: Try adding same ID twice → Should show error
4. **Range Query**: Add 10 students, query range [3, 7] → Should print only matching IDs
5. **Deletion**: Delete nodes with 0, 1, and 2 children → Tree should rebalance
6. **Tree Visualization**: After each operation, use option 6 to verify colors

---

## 📊 Performance Analysis

### Time Complexity

| Operation | Average | Worst Case |
|-----------|---------|------------|
| Search | O(log n) | O(log n) |
| Insert | O(log n) | O(log n) |
| Delete | O(log n) | O(log n) |
| Range Query | O(log n + k) | O(log n + k) |

**k** = number of elements in range

### Space Complexity

- **Tree Storage**: O(n) where n = number of students
- **Each Node**: ~56 bytes (Student data + pointers + color)
- **For 10,000 students**: ~560 KB

---

## 🎓 Academic Value

### Data Structures Concepts Demonstrated

✅ Self-balancing binary search trees  
✅ Tree rotations (left and right)  
✅ Color-based balancing (Red-Black properties)  
✅ Recursion (traversals)  
✅ Pointer manipulation  
✅ Object-Oriented Design (encapsulation, separation of concerns)  
✅ Template-free generic programming  
✅ Time complexity analysis  

### Why This Project is Impressive

1. **Real-World Application**: Solves actual database problem (student records)
2. **Complex Algorithm**: RBT deletion is one of the hardest algorithms in CS courses
3. **Performance**: Beats linear structures (arrays, linked lists) for large datasets
4. **Demonstrates Mastery**: Shows understanding of both theory and implementation
5. **Professional Code**: Uses header/implementation separation, proper naming, encapsulation

---

## 🔮 Possible Extensions

### Bonus Features (for Extra Credit)

1. **File Persistence**
   - Save tree to `students.txt` on exit
   - Load tree from file on startup
   - Use in-order traversal for saving

2. **Secondary Indexes**
   - Additional tree sorted by name (not just ID)
   - Allows searching by name with O(log n)

3. **Statistics**
   - Calculate average GPA for department
   - Find top N students by GPA
   - Count students per department

4. **Bulk Operations**
   - Load CSV file with 1000+ students
   - Benchmark insertion time
   - Compare with array-based implementation

---

## �️ GUI Version (Qt)

This project now includes a **graphical user interface** built with Qt6 Widgets!

### Features
- ✨ Modern, intuitive interface
- 📊 Visual tree structure display
- 📋 Table view of all students
- 🔍 Real-time search and filtering
- ➕ Easy add/delete operations
- 📈 Range query visualization

### Installing Qt6

**Windows:**
```bash
# Download Qt Online Installer from https://www.qt.io/download-qt-installer
# Install Qt 6.x with MinGW or MSVC compiler
# Add Qt to PATH: C:\Qt\6.x.x\mingw_64\bin
```

**Linux:**
```bash
sudo apt-get install qt6-base-dev libqt6widgets6
```

**macOS:**
```bash
brew install qt@6
```

### Building the GUI Version

```bash
# Navigate to project directory
cd student_records

# Create build directory
mkdir build
cd build

# Configure with CMake (Qt6 will be auto-detected)
cmake ..

# Build both console and GUI versions
cmake --build .

# Run GUI version
./student_records_gui          # Linux/Mac
student_records_gui.exe        # Windows

# Run console version
./student_records              # Linux/Mac
student_records.exe            # Windows
```

### Disabling GUI Build

If you don't want to build the GUI version:
```bash
cmake -DBUILD_GUI=OFF ..
```

---

## �📝 Code Quality Features

- **Const Correctness**: Getters marked `const`
- **Memory Management**: Proper `new`/`delete` usage
- **Encapsulation**: Private data members with public interface
- **Separation of Concerns**: Each class has single responsibility
- **Meaningful Names**: Self-documenting function and variable names
- **Error Handling**: Validates input and handles edge cases
- **Nested Classes**: Student and Node are nested within RBTree for better encapsulation and logical grouping

---

## 👨‍💻 Author

Data Structures Course Project  
December 2025

---

## 📖 References

1. **Introduction to Algorithms** (CLRS) - Chapter 13: Red-Black Trees
2. **Data Structures and Algorithm Analysis in C++** (Mark Allen Weiss)
3. Red-Black Tree Visualization: https://www.cs.usfca.edu/~galles/visualization/RedBlack.html

---

## ⚖️ License

This project is for educational purposes as part of a Data Structures university course.
