#include <iostream>
#include <string>
#include "RBTree.h"

using namespace std;

int main() {
    RBTree sis;
    int choice, id, minID, maxID;
    string name, dept;
    double gpa;

    cout << "========================================\n";
    cout << "  Student Information System (RB-Tree)\n";
    cout << "========================================\n";

    while (true) {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Add Student\n";
        cout << "2. Search Student by ID\n";
        cout << "3. Print Students in ID Range\n";
        cout << "4. Delete Student\n";
        cout << "5. Display All Students (Sorted)\n";
        cout << "6. Visualize Tree Structure\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "\n--- Add New Student ---\n";
            cout << "Enter Student ID: ";
            cin >> id;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Department: ";
            getline(cin, dept);
            cout << "Enter GPA: ";
            cin >> gpa;
            sis.insert(id, name, dept, gpa);
            cout << "Student added successfully!\n";
            break;

        case 2:
            cout << "\n--- Search Student ---\n";
            cout << "Enter Student ID to search: ";
            cin >> id;
            sis.search(id);
            break;

        case 3:
            cout << "\n--- Range Query ---\n";
            cout << "Enter Minimum ID: ";
            cin >> minID;
            cout << "Enter Maximum ID: ";
            cin >> maxID;
            sis.printRange(minID, maxID);
            break;

        case 4:
            cout << "\n--- Delete Student ---\n";
            cout << "Enter Student ID to delete: ";
            cin >> id;
            sis.deleteNode(id);
            cout << "Student deleted successfully (if found).\n";
            break;

        case 5:
            cout << "\n--- All Students (Sorted by ID) ---\n";
            sis.inorder();
            break;

        case 6:
            cout << "\n--- Tree Structure Visualization ---\n";
            cout << "(L = Left Child, R = Right Child)\n\n";
            sis.printTree();
            break;

        case 7:
            cout << "\nExiting Student Information System. Goodbye!\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}