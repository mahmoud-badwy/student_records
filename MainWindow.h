#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QMessageBox>
#include "RBTree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addStudent();
    void searchStudent();
    void deleteStudent();
    void showAllStudents();
    void showRange();
    void showTreeStructure();
    void clearForm();

private:
    void setupUI();
    void refreshStudentTable();
    void collectStudents(RBTree::Node* node, QList<RBTree::Student>& students);
    QString getTreeStructure(RBTree::Node* node, const QString& indent, bool last);
    
    // Core data structure
    RBTree* studentTree;
    
    // UI Components - Input Form
    QLineEdit* idInput;
    QLineEdit* nameInput;
    QLineEdit* deptInput;
    QLineEdit* gpaInput;
    
    // UI Components - Search/Delete/Range
    QLineEdit* searchIdInput;
    QLineEdit* deleteIdInput;
    QLineEdit* minIdInput;
    QLineEdit* maxIdInput;
    
    // UI Components - Display
    QTableWidget* studentTable;
    QTextEdit* treeDisplay;
    
    // UI Components - Buttons
    QPushButton* addBtn;
    QPushButton* searchBtn;
    QPushButton* deleteBtn;
    QPushButton* showAllBtn;
    QPushButton* rangeBtn;
    QPushButton* treeViewBtn;
    QPushButton* clearBtn;
};

#endif // MAINWINDOW_H
