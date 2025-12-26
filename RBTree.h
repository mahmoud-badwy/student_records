#ifndef RBTREE_H
#define RBTREE_H

#include <string>

enum Color { RED, BLACK };

class RBTree {
public:
    class Student {
    private:
        int id;
        std::string name;
        std::string dept;
        double gpa;

    public:
        Student();
        Student(int i, std::string n, std::string d, double g);
        
        int getId() const;
        std::string getName() const;
        std::string getDept() const;
        double getGpa() const;
        
        void setId(int i);
        void setName(const std::string& n);
        void setDept(const std::string& d);
        void setGpa(double g);
    };

    class Node {
    private:
        Student data;
        Color color;
        Node *left;
        Node *right;
        Node *parent;

    public:
        Node(Student s);
        
        Student getData() const;
        Color getColor() const;
        Node* getLeft() const;
        Node* getRight() const;
        Node* getParent() const;
        
        void setData(const Student& s);
        void setColor(Color c);
        void setLeft(Node* node);
        void setRight(Node* node);
        void setParent(Node* node);
        
        friend class RBTree;
    };

private:
    Node *root;
    Node *TNULL;

    void initializeNULLNode(Node *node, Node *parent);
    void preOrderHelper(Node *node);
    void inOrderHelper(Node *node);
    Node *searchTreeHelper(Node *node, int key);
    void fixDelete(Node *x);
    void rbTransplant(Node *u, Node *v);
    void deleteNodeHelper(Node *node, int key);
    void fixInsert(Node *k);
    void printHelper(Node *root, std::string indent, bool last);
    void rangeQueryHelper(Node *node, int minID, int maxID);

public:
    RBTree();
    void preorder();
    void inorder();
    Node *searchTree(int k);
    Node *minimum(Node *node);
    Node *maximum(Node *node);
    void leftRotate(Node *x);
    void rightRotate(Node *x);
    void insert(int id, std::string name, std::string dept, double gpa);
    Node *getRoot();
    void deleteNode(int id);
    void printTree();
    void search(int id);
    void printRange(int minID, int maxID);
};

#endif
