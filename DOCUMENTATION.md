# Student Information System - Red-Black Tree Implementation

## Table of Contents
1. [Project Overview](#project-overview)
2. [System Architecture](#system-architecture)
3. [Data Structures](#data-structures)
4. [Red-Black Tree Properties](#red-black-tree-properties)
5. [Core Functions Explained](#core-functions-explained)
6. [GUI Implementation](#gui-implementation)
7. [Algorithm Analysis](#algorithm-analysis)
8. [Usage Guide](#usage-guide)

---

## Project Overview

This is a **Student Information System** built using a **Red-Black Tree** data structure in C++. It provides both a **console interface** and a **graphical user interface (GUI)** built with Qt6.

### Features
- ✅ Add, Search, and Delete student records
- ✅ Range queries (find students within ID range)
- ✅ Sorted display of all students
- ✅ Visual tree structure representation
- ✅ Self-balancing Red-Black Tree ensures O(log n) operations
- ✅ Modern Qt6 GUI with real-time table updates

---

## System Architecture

```
student_records/
│
├── Core Data Structure (RBTree)
│   ├── RBTree.h          # Red-Black Tree declarations
│   └── RBTree.cpp        # Red-Black Tree implementation
│
├── Console Interface
│   └── main.cpp          # Console-based menu system
│
├── GUI Interface
│   ├── MainWindow.h      # GUI class declarations
│   ├── MainWindow.cpp    # GUI implementation
│   └── main_gui.cpp      # GUI entry point
│
└── Build System
    └── CMakeLists.txt    # CMake build configuration
```

---

## Data Structures

### 1. Student Class

```cpp
class Student {
    int id;           // Unique student identifier
    string name;      // Student's full name
    string dept;      // Department
    double gpa;       // Grade Point Average (0.0 - 4.0)
};
```

**Purpose**: Encapsulates student information with getter/setter methods.

**Key Methods**:
- `getId()`, `getName()`, `getDept()`, `getGpa()` - Accessors
- `setId()`, `setName()`, `setDept()`, `setGpa()` - Mutators

---

### 2. Node Class

```cpp
class Node {
    Student data;     // Student information
    Color color;      // RED or BLACK
    Node *left;       // Left child pointer
    Node *right;      // Right child pointer
    Node *parent;     // Parent pointer
};
```

**Purpose**: Represents a single node in the Red-Black Tree.

**Key Features**:
- **Color field**: Essential for maintaining Red-Black Tree properties
- **Parent pointer**: Required for tree rotations and deletions
- **Encapsulation**: Private members with public getter/setter methods

---

### 3. RBTree Class

```cpp
class RBTree {
    Node *root;       // Root of the tree
    Node *TNULL;      // Sentinel NIL node
};
```

**Purpose**: Implements the Red-Black Tree with all balancing operations.

---

## Red-Black Tree Properties

A **Red-Black Tree** is a self-balancing Binary Search Tree with these properties:

### The 5 Rules:

1. **Every node is either RED or BLACK**
2. **The root is always BLACK**
3. **All leaf nodes (NIL) are BLACK**
4. **RED nodes cannot have RED children** (No two consecutive RED nodes on any path)
5. **Every path from root to leaf has the same number of BLACK nodes** (Black-Height property)

### Why Red-Black Trees?

| Operation | Average | Worst Case |
|-----------|---------|------------|
| Search    | O(log n) | O(log n) |
| Insert    | O(log n) | O(log n) |
| Delete    | O(log n) | O(log n) |

**Compared to Regular BST**: Worst case O(n) when unbalanced (e.g., inserting sorted data).

---

## Core Functions Explained

### 1. Insertion Process

#### **Function Flow:**

```
insert(id, name, dept, gpa)
    ↓
1. Create new RED node
    ↓
2. Standard BST insertion
    ↓
3. fixInsert(node) - Restore RB properties
    ↓
4. Done
```

#### **Code Breakdown:**

```cpp
void RBTree::insert(int id, string name, string dept, double gpa) {
    // Step 1: Create new node (always RED initially)
    Node *node = new Node(Student(id, name, dept, gpa));
    node->color = RED;
    node->left = TNULL;
    node->right = TNULL;
    
    // Step 2: Find insertion position (like BST)
    Node *y = nullptr;  // Parent of insertion point
    Node *x = root;     // Current position
    
    while (x != TNULL) {
        y = x;
        if (node->data.getId() < x->data.getId())
            x = x->left;   // Go left
        else if (node->data.getId() > x->data.getId())
            x = x->right;  // Go right
        else {
            // Duplicate ID - reject insertion
            delete node;
            return;
        }
    }
    
    // Step 3: Link new node to parent
    node->parent = y;
    if (y == nullptr)
        root = node;  // Tree was empty
    else if (node->data.getId() < y->data.getId())
        y->left = node;
    else
        y->right = node;
    
    // Step 4: Fix violations
    if (node->parent == nullptr) {
        node->color = BLACK;  // Root must be BLACK
        return;
    }
    
    if (node->parent->parent == nullptr)
        return;  // No violations if parent is root
    
    fixInsert(node);  // Restore RB properties
}
```

---

### 2. Fix Insert - Restoring Balance

#### **Cases Handled:**

**Case 1: Uncle is RED**
- Recolor parent, uncle, grandparent
- Move up the tree

```
      [B]Grandparent          [R]Grandparent
      /  \                    /  \
   [R]P  [R]U    →        [B]P  [B]U
    /                      /
  [R]N                  [R]N
```

**Case 2: Uncle is BLACK (Triangle)**
- Rotate to convert to Case 3

```
      [B]G                [B]G
      /  \                /  \
   [R]P  [B]U   →      [R]N  [B]U
      \                /
     [R]N          [R]P
```

**Case 3: Uncle is BLACK (Line)**
- Rotate and recolor

```
      [B]G                [B]P
      /  \                /  \
   [R]P  [B]U   →      [R]N  [R]G
    /                           \
  [R]N                         [B]U
```

#### **Code:**

```cpp
void RBTree::fixInsert(Node *k) {
    Node *u;
    while (k->parent->color == RED) {  // Violation: RED parent
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;  // Uncle
            
            if (u->color == RED) {
                // Case 1: Uncle is RED
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;  // Move up
            } else {
                if (k == k->parent->left) {
                    // Case 2: Triangle - convert to line
                    k = k->parent;
                    rightRotate(k);
                }
                // Case 3: Line - rotate and recolor
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        } else {
            // Mirror cases (left side)
            u = k->parent->parent->right;
            
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }
        if (k == root)
            break;
    }
    root->color = BLACK;  // Ensure root is always BLACK
}
```

---

### 3. Tree Rotations

Rotations maintain BST property while restructuring the tree.

#### **Left Rotate:**

```
Before:              After:
   [x]                 [y]
   / \                 / \
  A  [y]     →      [x]  C
     / \             / \
    B   C           A   B
```

```cpp
void RBTree::leftRotate(Node *x) {
    Node *y = x->right;
    
    // Turn y's left subtree into x's right subtree
    x->right = y->left;
    if (y->left != TNULL)
        y->left->parent = x;
    
    // Link x's parent to y
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    // Put x on y's left
    y->left = x;
    x->parent = y;
}
```

#### **Right Rotate:** (Mirror of left rotate)

```cpp
void RBTree::rightRotate(Node *x) {
    Node *y = x->left;
    x->left = y->right;
    
    if (y->right != TNULL)
        y->right->parent = x;
    
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    
    y->right = x;
    x->parent = y;
}
```

---

### 4. Search Operation

**Complexity**: O(log n) - follows BST property

```cpp
Node* RBTree::searchTreeHelper(Node *node, int key) {
    // Base cases
    if (node == TNULL || key == node->data.getId())
        return node;
    
    // Recursive search
    if (key < node->data.getId())
        return searchTreeHelper(node->left, key);  // Search left
    else
        return searchTreeHelper(node->right, key); // Search right
}
```

**Flow Example:**
```
Search for ID = 45 in tree:

        [50]
        /  \
     [30]  [70]
      / \   / \
   [20][40][60][80]

Step 1: 45 < 50 → Go LEFT
Step 2: 45 > 30 → Go RIGHT  
Step 3: 45 > 40 → Go RIGHT
Step 4: TNULL → Not found
```

---

### 5. Deletion Process

#### **Flow:**

```
deleteNode(id)
    ↓
1. Find node with given ID
    ↓
2. Determine replacement node
    ↓
3. Perform BST deletion
    ↓
4. If deleted node was BLACK → fixDelete()
    ↓
5. Done
```

#### **Three Cases:**

**Case 1: Node has no children**
- Simply remove it

**Case 2: Node has one child**
- Replace node with its child

**Case 3: Node has two children**
- Find successor (minimum in right subtree)
- Replace node with successor
- Delete successor from its original position

```cpp
void RBTree::deleteNodeHelper(Node *node, int key) {
    Node *z = TNULL;  // Node to delete
    Node *x, *y;
    
    // Step 1: Find the node
    while (node != TNULL) {
        if (node->data.getId() == key)
            z = node;
        
        if (node->data.getId() <= key)
            node = node->right;
        else
            node = node->left;
    }
    
    if (z == TNULL) {
        cout << "Student not found.\n";
        return;
    }
    
    y = z;
    Color y_original_color = y->color;
    
    // Case 1 & 2: Node has at most one child
    if (z->left == TNULL) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        // Case 3: Node has two children
        y = minimum(z->right);  // Find successor
        y_original_color = y->color;
        x = y->right;
        
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    delete z;
    
    // Fix violations if BLACK node was deleted
    if (y_original_color == BLACK)
        fixDelete(x);
}
```

---

### 6. Range Query

**Purpose**: Find all students with IDs in range [minID, maxID]

**Algorithm**: Modified In-Order Traversal

```cpp
void RBTree::rangeQueryHelper(Node *node, int minID, int maxID) {
    if (node == TNULL)
        return;
    
    // Only traverse left if there might be values in range
    if (node->data.getId() > minID)
        rangeQueryHelper(node->left, minID, maxID);
    
    // Print if in range
    if (node->data.getId() >= minID && node->data.getId() <= maxID) {
        cout << "ID: " << node->data.getId() 
             << " | Name: " << node->data.getName() 
             << " | Dept: " << node->data.getDept() 
             << " | GPA: " << node->data.getGpa() << endl;
    }
    
    // Only traverse right if there might be values in range
    if (node->data.getId() < maxID)
        rangeQueryHelper(node->right, minID, maxID);
}
```

**Example:**
```
Find students with ID in range [25, 45]

        [50]
        /  \
     [30]  [70]
      / \   / \
   [20][40][60][80]

Traversal:
1. Start at 50 → 50 > 25, explore left
2. At 30 → In range! Print. Check both sides
3. At 20 → 20 < 25, skip left, check right
4. At 40 → In range! Print. 40 < 45, check right
5. Back to 50 → 50 > 45, skip right subtree

Result: [30], [40]
```

---

### 7. In-Order Traversal (Sorted Output)

**Purpose**: Display all students sorted by ID

**Algorithm**: Left → Root → Right

```cpp
void RBTree::inOrderHelper(Node *node) {
    if (node != TNULL) {
        inOrderHelper(node->left);   // Visit left subtree
        
        // Process current node
        cout << "ID: " << node->data.getId() 
             << " | Name: " << node->data.getName() 
             << " | Dept: " << node->data.getDept() 
             << " | GPA: " << node->data.getGpa() << endl;
        
        inOrderHelper(node->right);  // Visit right subtree
    }
}
```

**Example:**
```
Tree:        [50]
            /    \
         [30]    [70]
         /  \    /  \
      [20][40][60][80]

In-Order Output: 20, 30, 40, 50, 60, 70, 80 (Sorted!)
```

---

## GUI Implementation

### MainWindow Class Structure

```cpp
class MainWindow : public QMainWindow {
    RBTree* studentTree;           // Core data structure
    
    // Input widgets
    QLineEdit* idInput;
    QLineEdit* nameInput;
    QLineEdit* deptInput;
    QLineEdit* gpaInput;
    
    // Display widgets
    QTableWidget* studentTable;    // Shows all students
    QTextEdit* treeDisplay;        // Shows tree structure
};
```

---

### GUI Functions Explained

#### 1. **setupUI()**

**Purpose**: Creates the entire GUI layout

**Sections**:
1. **Add Student Form**: Input fields for student data
2. **Operations Panel**: Search, Delete, Range query buttons
3. **Student Table**: Displays all records in tabular format
4. **Tree Visualization**: Text-based tree structure

**Layout Hierarchy**:
```
MainWindow
└── Central Widget
    ├── Add Student GroupBox
    │   └── Grid Layout (ID, Name, Dept, GPA inputs)
    ├── Operations GroupBox
    │   └── Grid Layout (Search, Delete, Range inputs)
    ├── Button Layout
    │   ├── Show All Button
    │   └── Visualize Tree Button
    ├── Student Table Widget
    └── Tree Display Text Edit
```

---

#### 2. **addStudent()**

**Flow**:
```
User clicks "Add Student"
    ↓
Validate input (ID, Name, Dept, GPA)
    ↓
Call studentTree->insert()
    ↓
Show success message
    ↓
Clear form
    ↓
refreshStudentTable()
```

**Code Logic**:
```cpp
void MainWindow::addStudent() {
    // Step 1: Validate ID
    bool ok;
    int id = idInput->text().toInt(&ok);
    if (!ok || idInput->text().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Enter valid ID");
        return;
    }
    
    // Step 2: Validate other fields
    QString name = nameInput->text().trimmed();
    QString dept = deptInput->text().trimmed();
    double gpa = gpaInput->text().toDouble(&ok);
    
    if (name.isEmpty() || dept.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Fill all fields");
        return;
    }
    
    if (!ok || gpa < 0.0 || gpa > 4.0) {
        QMessageBox::warning(this, "Invalid Input", "GPA must be 0.00-4.00");
        return;
    }
    
    // Step 3: Insert into tree
    studentTree->insert(id, name.toStdString(), dept.toStdString(), gpa);
    
    // Step 4: Update UI
    QMessageBox::information(this, "Success", "Student added!");
    clearForm();
    refreshStudentTable();  // Update table display
}
```

---

#### 3. **searchStudent()**

**Purpose**: Find and display student details by ID

```cpp
void MainWindow::searchStudent() {
    // Get ID and validate
    bool ok;
    int id = searchIdInput->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Enter valid ID");
        return;
    }
    
    // Search in tree
    RBTree::Node* result = studentTree->searchTree(id);
    
    // Display result
    if (result != nullptr && result->getData().getId() != 0) {
        RBTree::Student student = result->getData();
        QString msg = QString("Student Found!\n\n"
                             "ID: %1\n"
                             "Name: %2\n"
                             "Department: %3\n"
                             "GPA: %4")
                             .arg(student.getId())
                             .arg(QString::fromStdString(student.getName()))
                             .arg(QString::fromStdString(student.getDept()))
                             .arg(student.getGpa(), 0, 'f', 2);
        
        QMessageBox::information(this, "Search Result", msg);
    } else {
        QMessageBox::information(this, "Not Found", 
            QString("No student with ID: %1").arg(id));
    }
    
    searchIdInput->clear();
}
```

---

#### 4. **refreshStudentTable()**

**Purpose**: Update the table widget with all students from the tree

**Flow**:
```
Clear existing table
    ↓
Collect all students via in-order traversal
    ↓
Populate table rows
    ↓
Update tree visualization
```

**Code**:
```cpp
void MainWindow::refreshStudentTable() {
    studentTable->setRowCount(0);  // Clear table
    
    // Collect all students (sorted by ID)
    QList<RBTree::Student> students;
    collectStudents(studentTree->getRoot(), students);
    
    // Populate table
    studentTable->setRowCount(students.size());
    
    for (int i = 0; i < students.size(); ++i) {
        const RBTree::Student& student = students[i];
        
        // Create table items
        QTableWidgetItem* idItem = new QTableWidgetItem(
            QString::number(student.getId()));
        QTableWidgetItem* nameItem = new QTableWidgetItem(
            QString::fromStdString(student.getName()));
        QTableWidgetItem* deptItem = new QTableWidgetItem(
            QString::fromStdString(student.getDept()));
        QTableWidgetItem* gpaItem = new QTableWidgetItem(
            QString::number(student.getGpa(), 'f', 2));
        
        // Add to table
        studentTable->setItem(i, 0, idItem);
        studentTable->setItem(i, 1, nameItem);
        studentTable->setItem(i, 2, deptItem);
        studentTable->setItem(i, 3, gpaItem);
    }
    
    // Also update tree visualization
    showTreeStructure();
}
```

---

#### 5. **collectStudents()** - Helper Function

**Purpose**: Recursively collect all students in sorted order

```cpp
void MainWindow::collectStudents(RBTree::Node* node, 
                                 QList<RBTree::Student>& students) {
    if (node == nullptr || node->getData().getId() == 0)
        return;
    
    // In-order traversal: Left → Root → Right
    collectStudents(node->getLeft(), students);   // Left
    students.append(node->getData());             // Root
    collectStudents(node->getRight(), students);  // Right
}
```

---

#### 6. **showTreeStructure()**

**Purpose**: Display visual representation of the tree

**Output Format**:
```
Red-Black Tree Structure:
(R = Red node, B = Black node)

└─[B] ID:50 (John Doe)
  ├─[R] ID:30 (Jane Smith)
  │ ├─[B] ID:20 (Bob Johnson)
  │ └─[B] ID:40 (Alice Brown)
  └─[B] ID:70 (Charlie Davis)
    ├─[R] ID:60 (Eve Wilson)
    └─[R] ID:80 (Frank Miller)
```

**Code**:
```cpp
void MainWindow::showTreeStructure() {
    RBTree::Node* root = studentTree->getRoot();
    
    if (root == nullptr || root->getData().getId() == 0) {
        treeDisplay->setText("Tree is empty.");
        return;
    }
    
    QString structure = "Red-Black Tree Structure:\n";
    structure += "(R = Red node, B = Black node)\n\n";
    structure += getTreeStructure(root, "", true);
    
    treeDisplay->setText(structure);
}
```

---

#### 7. **getTreeStructure()** - Recursive Helper

**Purpose**: Build tree visualization string recursively

```cpp
QString MainWindow::getTreeStructure(RBTree::Node* node, 
                                     const QString& indent, 
                                     bool last) {
    QString result;
    
    if (node == nullptr || node->getData().getId() == 0)
        return result;
    
    // Add branch characters
    result += indent;
    if (last) {
        result += "└─";
    } else {
        result += "├─";
    }
    
    // Add node info with color
    QString color = (node->getColor() == RED) ? "R" : "B";
    result += QString("[%1] ID:%2 (%3)\n")
                 .arg(color)
                 .arg(node->getData().getId())
                 .arg(QString::fromStdString(node->getData().getName()));
    
    // Prepare indent for children
    QString newIndent = indent + (last ? "  " : "│ ");
    
    // Recursively add left and right children
    if (node->getLeft() && node->getLeft()->getData().getId() != 0) {
        bool isLast = (node->getRight() == nullptr || 
                      node->getRight()->getData().getId() == 0);
        result += getTreeStructure(node->getLeft(), newIndent, isLast);
    }
    
    if (node->getRight() && node->getRight()->getData().getId() != 0) {
        result += getTreeStructure(node->getRight(), newIndent, true);
    }
    
    return result;
}
```

---

## Algorithm Analysis

### Time Complexity

| Operation | Best Case | Average Case | Worst Case |
|-----------|-----------|--------------|------------|
| Insert    | O(log n)  | O(log n)     | O(log n)   |
| Search    | O(log n)  | O(log n)     | O(log n)   |
| Delete    | O(log n)  | O(log n)     | O(log n)   |
| Range Query| O(log n + k) | O(log n + k) | O(log n + k) |
| Display All| O(n)      | O(n)         | O(n)       |

*k = number of elements in range*

### Space Complexity

- **Tree Storage**: O(n) - Each student requires one node
- **Recursion Stack**: O(log n) - Maximum tree height

### Why Red-Black Tree?

**Advantages**:
- ✅ Guaranteed O(log n) height
- ✅ Faster insertions than AVL trees (fewer rotations)
- ✅ Used in Java TreeMap, C++ std::map, Linux kernel

**Comparison**:
| Tree Type | Rotations (Insert) | Balance Factor |
|-----------|-------------------|----------------|
| RB-Tree   | ≤ 2               | Approximate    |
| AVL Tree  | ≤ 2               | Strict (±1)    |
| BST       | 0                 | None (can be O(n)) |

---

## Usage Guide

### Console Version

```bash
# Run console program
./student_records

# Menu options:
1. Add Student
2. Search Student by ID
3. Print Students in ID Range
4. Delete Student
5. Display All Students (Sorted)
6. Visualize Tree Structure
7. Exit
```

### GUI Version

```bash
# Run GUI program
./student_records_gui
```

**GUI Workflow**:
1. **Add Students**: Fill form → Click "Add Student"
2. **Search**: Enter ID → Click "Search"
3. **Delete**: Enter ID → Click "Delete" → Confirm
4. **Range Query**: Enter Min/Max ID → Click "Show Range"
5. **View All**: Click "Show All Students" (auto-updates table)
6. **Visualize**: Click "Visualize Tree Structure" (auto-updates)

---

## Example Scenario

### Adding Students:

```cpp
insert(101, "Alice Johnson", "Computer Science", 3.85);
insert(105, "Bob Smith", "Mathematics", 3.92);
insert(103, "Charlie Brown", "Physics", 3.67);
insert(107, "Diana Prince", "Chemistry", 3.98);
insert(102, "Eve Davis", "Biology", 3.74);
```

### Resulting Tree Structure:

```
        [B] 103 (Charlie Brown)
        /                      \
   [R] 101 (Alice)        [R] 105 (Bob)
        \                  /         \
     [B] 102 (Eve)   [B] 107 (Diana)
```

### Operations:

**Search ID=105**: O(log 5) ≈ 3 comparisons
- 103 → 105 → Found!

**Range Query [102-106]**: Returns Alice, Eve, Bob, Charlie

**Delete ID=103**: 
- Replace with successor (105)
- Rebalance tree

---

## Conclusion

This Student Information System demonstrates:
- ✅ Advanced data structure implementation (Red-Black Tree)
- ✅ Self-balancing algorithm with guaranteed O(log n) performance
- ✅ Modern GUI development with Qt6
- ✅ Real-world application of theoretical concepts

**Key Takeaways**:
- Red-Black Trees ensure logarithmic operations even in worst case
- Rotations and recoloring maintain balance efficiently
- GUI provides user-friendly interface for complex data structure
- Code is modular, reusable, and well-documented

---

## References

- **Introduction to Algorithms** (CLRS) - Chapter 13: Red-Black Trees
- **Qt Documentation**: https://doc.qt.io/
- **Wikipedia**: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree

---

**Project by**: Student Information System Team  
**Date**: December 26, 2025  
**Version**: 1.0
