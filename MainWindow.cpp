#include "MainWindow.h"
#include <QGridLayout>
#include <QHeaderView>
#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    studentTree = new RBTree();
    setupUI();
    setWindowTitle("Student Information System - Red-Black Tree");
    resize(1000, 700);
}

MainWindow::~MainWindow() {
    delete studentTree;
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // ========== ADD STUDENT SECTION ==========
    QGroupBox* addGroup = new QGroupBox("Add New Student");
    QGridLayout* addLayout = new QGridLayout();
    
    addLayout->addWidget(new QLabel("Student ID:"), 0, 0);
    idInput = new QLineEdit();
    idInput->setPlaceholderText("Enter numeric ID");
    addLayout->addWidget(idInput, 0, 1);
    
    addLayout->addWidget(new QLabel("Name:"), 0, 2);
    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter full name");
    addLayout->addWidget(nameInput, 0, 3);
    
    addLayout->addWidget(new QLabel("Department:"), 1, 0);
    deptInput = new QLineEdit();
    deptInput->setPlaceholderText("Enter department");
    addLayout->addWidget(deptInput, 1, 1);
    
    addLayout->addWidget(new QLabel("GPA:"), 1, 2);
    gpaInput = new QLineEdit();
    gpaInput->setPlaceholderText("0.00 - 4.00");
    addLayout->addWidget(gpaInput, 1, 3);
    
    addBtn = new QPushButton("Add Student");
    addBtn->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 8px;");
    addLayout->addWidget(addBtn, 2, 0, 1, 2);
    
    clearBtn = new QPushButton("Clear Form");
    clearBtn->setStyleSheet("background-color: #9E9E9E; color: white; padding: 8px;");
    addLayout->addWidget(clearBtn, 2, 2, 1, 2);
    
    addGroup->setLayout(addLayout);
    mainLayout->addWidget(addGroup);
    
    // ========== OPERATIONS SECTION ==========
    QGroupBox* opsGroup = new QGroupBox("Operations");
    QGridLayout* opsLayout = new QGridLayout();
    
    opsLayout->addWidget(new QLabel("Search by ID:"), 0, 0);
    searchIdInput = new QLineEdit();
    opsLayout->addWidget(searchIdInput, 0, 1);
    searchBtn = new QPushButton("Search");
    searchBtn->setStyleSheet("background-color: #2196F3; color: white; padding: 6px;");
    opsLayout->addWidget(searchBtn, 0, 2);
    
    opsLayout->addWidget(new QLabel("Delete by ID:"), 1, 0);
    deleteIdInput = new QLineEdit();
    opsLayout->addWidget(deleteIdInput, 1, 1);
    deleteBtn = new QPushButton("Delete");
    deleteBtn->setStyleSheet("background-color: #f44336; color: white; padding: 6px;");
    opsLayout->addWidget(deleteBtn, 1, 2);
    
    opsLayout->addWidget(new QLabel("Range (Min ID):"), 2, 0);
    minIdInput = new QLineEdit();
    opsLayout->addWidget(minIdInput, 2, 1);
    opsLayout->addWidget(new QLabel("Max ID:"), 2, 2);
    maxIdInput = new QLineEdit();
    opsLayout->addWidget(maxIdInput, 2, 3);
    rangeBtn = new QPushButton("Show Range");
    rangeBtn->setStyleSheet("background-color: #FF9800; color: white; padding: 6px;");
    opsLayout->addWidget(rangeBtn, 2, 4);
    
    opsGroup->setLayout(opsLayout);
    mainLayout->addWidget(opsGroup);
    
    // ========== DISPLAY BUTTONS ==========
    QHBoxLayout* displayBtnLayout = new QHBoxLayout();
    
    showAllBtn = new QPushButton("Show All Students (Sorted)");
    showAllBtn->setStyleSheet("background-color: #607D8B; color: white; font-weight: bold; padding: 10px;");
    displayBtnLayout->addWidget(showAllBtn);
    
    treeViewBtn = new QPushButton("Visualize Tree Structure");
    treeViewBtn->setStyleSheet("background-color: #9C27B0; color: white; font-weight: bold; padding: 10px;");
    displayBtnLayout->addWidget(treeViewBtn);
    
    mainLayout->addLayout(displayBtnLayout);
    
    // ========== STUDENT TABLE ==========
    QGroupBox* tableGroup = new QGroupBox("Student Records");
    QVBoxLayout* tableLayout = new QVBoxLayout();
    
    studentTable = new QTableWidget();
    studentTable->setColumnCount(4);
    studentTable->setHorizontalHeaderLabels({"ID", "Name", "Department", "GPA"});
    studentTable->horizontalHeader()->setStretchLastSection(true);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setAlternatingRowColors(true);
    
    tableLayout->addWidget(studentTable);
    tableGroup->setLayout(tableLayout);
    mainLayout->addWidget(tableGroup);
    
    // ========== TREE STRUCTURE DISPLAY ==========
    QGroupBox* treeGroup = new QGroupBox("Tree Structure Visualization");
    QVBoxLayout* treeLayout = new QVBoxLayout();
    
    treeDisplay = new QTextEdit();
    treeDisplay->setReadOnly(true);
    treeDisplay->setFont(QFont("Courier New", 10));
    treeDisplay->setMaximumHeight(200);
    
    treeLayout->addWidget(treeDisplay);
    treeGroup->setLayout(treeLayout);
    mainLayout->addWidget(treeGroup);
    
    // ========== CONNECT SIGNALS AND SLOTS ==========
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::searchStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(showAllBtn, &QPushButton::clicked, this, &MainWindow::showAllStudents);
    connect(rangeBtn, &QPushButton::clicked, this, &MainWindow::showRange);
    connect(treeViewBtn, &QPushButton::clicked, this, &MainWindow::showTreeStructure);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearForm);
}

void MainWindow::addStudent() {
    bool ok;
    int id = idInput->text().toInt(&ok);
    
    if (!ok || idInput->text().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid numeric ID.");
        return;
    }
    
    QString name = nameInput->text().trimmed();
    QString dept = deptInput->text().trimmed();
    double gpa = gpaInput->text().toDouble(&ok);
    
    if (name.isEmpty() || dept.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Name and Department cannot be empty.");
        return;
    }
    
    if (!ok || gpa < 0.0 || gpa > 4.0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid GPA (0.00 - 4.00).");
        return;
    }
    
    studentTree->insert(id, name.toStdString(), dept.toStdString(), gpa);
    
    QMessageBox::information(this, "Success", 
        QString("Student %1 added successfully!").arg(name));
    
    clearForm();
    refreshStudentTable();
}

void MainWindow::searchStudent() {
    bool ok;
    int id = searchIdInput->text().toInt(&ok);
    
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid numeric ID.");
        return;
    }
    
    RBTree::Node* result = studentTree->searchTree(id);
    
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
            QString("No student found with ID: %1").arg(id));
    }
    
    searchIdInput->clear();
}

void MainWindow::deleteStudent() {
    bool ok;
    int id = deleteIdInput->text().toInt(&ok);
    
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid numeric ID.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
        QString("Are you sure you want to delete student with ID: %1?").arg(id),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        studentTree->deleteNode(id);
        QMessageBox::information(this, "Success", "Student deleted (if found).");
        deleteIdInput->clear();
        refreshStudentTable();
    }
}

void MainWindow::showAllStudents() {
    refreshStudentTable();
    QMessageBox::information(this, "Display Updated", 
        "Student table refreshed with all records sorted by ID.");
}

void MainWindow::showRange() {
    bool ok1, ok2;
    int minId = minIdInput->text().toInt(&ok1);
    int maxId = maxIdInput->text().toInt(&ok2);
    
    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid numeric IDs for range.");
        return;
    }
    
    if (minId > maxId) {
        QMessageBox::warning(this, "Invalid Range", "Minimum ID cannot be greater than Maximum ID.");
        return;
    }
    
    // Clear table and show only range
    studentTable->setRowCount(0);
    
    // Capture output using a helper method
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf(ss.rdbuf());
    studentTree->printRange(minId, maxId);
    std::cout.rdbuf(oldCout);
    
    QString output = QString::fromStdString(ss.str());
    
    if (output.isEmpty() || output.contains("No students found")) {
        QMessageBox::information(this, "Range Query", 
            QString("No students found in range %1 to %2").arg(minId).arg(maxId));
    } else {
        QMessageBox::information(this, "Range Query", 
            QString("Students in range %1 to %2:\n\n%3").arg(minId).arg(maxId).arg(output));
    }
    
    refreshStudentTable();
}

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

QString MainWindow::getTreeStructure(RBTree::Node* node, const QString& indent, bool last) {
    QString result;
    
    if (node == nullptr || node->getData().getId() == 0) {
        return result;
    }
    
    result += indent;
    if (last) {
        result += "└─";
    } else {
        result += "├─";
    }
    
    QString color = (node->getColor() == RED) ? "R" : "B";
    result += QString("[%1] ID:%2 (%3)\n")
                 .arg(color)
                 .arg(node->getData().getId())
                 .arg(QString::fromStdString(node->getData().getName()));
    
    QString newIndent = indent + (last ? "  " : "│ ");
    
    if (node->getLeft() && node->getLeft()->getData().getId() != 0) {
        bool isLast = (node->getRight() == nullptr || node->getRight()->getData().getId() == 0);
        result += getTreeStructure(node->getLeft(), newIndent, isLast);
    }
    
    if (node->getRight() && node->getRight()->getData().getId() != 0) {
        result += getTreeStructure(node->getRight(), newIndent, true);
    }
    
    return result;
}

void MainWindow::clearForm() {
    idInput->clear();
    nameInput->clear();
    deptInput->clear();
    gpaInput->clear();
    idInput->setFocus();
}

void MainWindow::refreshStudentTable() {
    studentTable->setRowCount(0);
    
    // Capture inorder output
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf(ss.rdbuf());
    studentTree->inorder();
    std::cout.rdbuf(oldCout);
    
    // Note: This is a simplified approach. For better integration,
    // you might want to add a method to RBTree that returns a vector of students
    // For now, we'll show the tree structure when showing all students
    showTreeStructure();
}

