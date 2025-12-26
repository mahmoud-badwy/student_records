#include "RBTree.h"
#include <iostream>
#include <iomanip>

using namespace std;

RBTree::Student::Student() : id(0), name(""), dept(""), gpa(0.0) {}

RBTree::Student::Student(int i, string n, string d, double g) 
    : id(i), name(n), dept(d), gpa(g) {}

int RBTree::Student::getId() const {
    return id;
}

string RBTree::Student::getName() const {
    return name;
}

string RBTree::Student::getDept() const {
    return dept;
}

double RBTree::Student::getGpa() const {
    return gpa;
}

void RBTree::Student::setId(int i) {
    id = i;
}

void RBTree::Student::setName(const string& n) {
    name = n;
}

void RBTree::Student::setDept(const string& d) {
    dept = d;
}

void RBTree::Student::setGpa(double g) {
    gpa = g;
}

RBTree::Node::Node(Student s) 
    : data(s), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}

RBTree::Student RBTree::Node::getData() const {
    return data;
}

Color RBTree::Node::getColor() const {
    return color;
}

RBTree::Node* RBTree::Node::getLeft() const {
    return left;
}

RBTree::Node* RBTree::Node::getRight() const {
    return right;
}

RBTree::Node* RBTree::Node::getParent() const {
    return parent;
}

void RBTree::Node::setData(const Student& s) {
    data = s;
}

void RBTree::Node::setColor(Color c) {
    color = c;
}

void RBTree::Node::setLeft(RBTree::Node* node) {
    left = node;
}

void RBTree::Node::setRight(RBTree::Node* node) {
    right = node;
}

void RBTree::Node::setParent(RBTree::Node* node) {
    parent = node;
}

void RBTree::initializeNULLNode(RBTree::Node *node, RBTree::Node *parent) {
    node->data = Student();
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = BLACK;
}

void RBTree::preOrderHelper(RBTree::Node *node) {
    if (node != TNULL) {
        cout << node->data.getId() << " ";
        preOrderHelper(node->left);
        preOrderHelper(node->right);
    }
}

void RBTree::inOrderHelper(RBTree::Node *node) {
    if (node != TNULL) {
        inOrderHelper(node->left);
        cout << "ID: " << node->data.getId() 
             << " | Name: " << node->data.getName() 
             << " | Dept: " << node->data.getDept() 
             << " | GPA: " << fixed << setprecision(2) << node->data.getGpa() << endl;
        inOrderHelper(node->right);
    }
}

RBTree::Node *RBTree::searchTreeHelper(RBTree::Node *node, int key) {
    if (node == TNULL || key == node->data.getId()) {
        return node;
    }

    if (key < node->data.getId()) {
        return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
}

void RBTree::fixDelete(RBTree::Node *x) {
    Node *s;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }

            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }

            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void RBTree::rbTransplant(RBTree::Node *u, RBTree::Node *v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void RBTree::deleteNodeHelper(RBTree::Node *node, int key) {
    Node *z = TNULL;
    Node *x, *y;
    while (node != TNULL) {
        if (node->data.getId() == key) {
            z = node;
        }

        if (node->data.getId() <= key) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (z == TNULL) {
        cout << "Student with ID " << key << " not found in the tree.\n";
        return;
    }

    y = z;
    Color y_original_color = y->color;
    if (z->left == TNULL) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
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
    if (y_original_color == BLACK) {
        fixDelete(x);
    }
}

void RBTree::fixInsert(RBTree::Node *k) {
    Node *u;
    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        } else {
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
        if (k == root) {
            break;
        }
    }
    root->color = BLACK;
}

void RBTree::printHelper(RBTree::Node *root, string indent, bool last) {
    if (root != TNULL) {
        cout << indent;
        if (last) {
            cout << "R----";
            indent += "   ";
        } else {
            cout << "L----";
            indent += "|  ";
        }

        string sColor = root->color ? "RED" : "BLACK";
        cout << root->data.getId() << "(" << sColor << ")" << endl;
        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
    }
}

void RBTree::rangeQueryHelper(RBTree::Node *node, int minID, int maxID) {
    if (node == TNULL) {
        return;
    }

    if (node->data.getId() > minID) {
        rangeQueryHelper(node->left, minID, maxID);
    }

    if (node->data.getId() >= minID && node->data.getId() <= maxID) {
        cout << "ID: " << node->data.getId() 
             << " | Name: " << node->data.getName() 
             << " | Dept: " << node->data.getDept() 
             << " | GPA: " << fixed << setprecision(2) << node->data.getGpa() << endl;
    }

    if (node->data.getId() < maxID) {
        rangeQueryHelper(node->right, minID, maxID);
    }
}

RBTree::RBTree() {
    TNULL = new Node(Student());
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
}

void RBTree::preorder() {
    preOrderHelper(this->root);
}

void RBTree::inorder() {
    inOrderHelper(this->root);
}

RBTree::Node *RBTree::searchTree(int k) {
    return searchTreeHelper(this->root, k);
}

RBTree::Node *RBTree::minimum(RBTree::Node *node) {
    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

RBTree::Node *RBTree::maximum(RBTree::Node *node) {
    while (node->right != TNULL) {
        node = node->right;
    }
    return node;
}

void RBTree::leftRotate(RBTree::Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(RBTree::Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        this->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void RBTree::insert(int id, string name, string dept, double gpa) {
    Node *node = new Node(Student(id, name, dept, gpa));
    node->parent = nullptr;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;

    Node *y = nullptr;
    Node *x = this->root;

    while (x != TNULL) {
        y = x;
        if (node->data.getId() < x->data.getId()) {
            x = x->left;
        } else if (node->data.getId() > x->data.getId()) {
            x = x->right;
        } else {
            cout << "Error: Student with ID " << id << " already exists.\n";
            delete node;
            return;
        }
    }

    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->data.getId() < y->data.getId()) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == nullptr) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == nullptr) {
        return;
    }

    fixInsert(node);
}

RBTree::Node *RBTree::getRoot() {
    return this->root;
}

void RBTree::deleteNode(int id) {
    deleteNodeHelper(this->root, id);
}

void RBTree::printTree() {
    if (root) {
        printHelper(this->root, "", true);
    }
}

void RBTree::search(int id) {
    Node *result = searchTree(id);
    if (result == TNULL) {
        cout << "Student with ID " << id << " not found.\n";
    } else {
        cout << "\n--- Student Found ---\n";
        cout << "ID: " << result->data.getId() << endl;
        cout << "Name: " << result->data.getName() << endl;
        cout << "Department: " << result->data.getDept() << endl;
        cout << "GPA: " << fixed << setprecision(2) << result->data.getGpa() << endl;
    }
}

void RBTree::printRange(int minID, int maxID) {
    cout << "\n--- Students with IDs from " << minID << " to " << maxID << " ---\n";
    rangeQueryHelper(this->root, minID, maxID);
}
