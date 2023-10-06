//
// Created by safin on 11/19/2022.
//

#ifndef PA8_TREE_H
#define PA8_TREE_H
#include <vector>
#include <string>
#include <iostream>
#include <string>

using namespace std;

struct node{
    bool isOperator;
    string symbol;
    int value;
    node* leftChild;
    node* rightChild;
    int compute() const;
    //empty node/tree
    node(){
        isOperator = false;
        value = 0;
        symbol = "";
        leftChild = nullptr;
        rightChild = nullptr;
    };
    //operand node
    node(int value){
        isOperator = false;
        this->value = value;
        symbol = "";
        leftChild = nullptr;
        rightChild = nullptr;
    };
    //operator node
    node(string symbol, node* left, node* right){
        isOperator = true;
        value = 0;
        this->symbol = symbol;
        this->leftChild = left;
        this->rightChild = right; //null if unary operation
    };
};
class tree {
private:
    node* root;
    node* makeTree(vector<string> &exp);
    void traverse(node* root);
    void clear(node *treenode);
public:
    tree(vector<string> expression);
    ~tree();
    int computeTree();
    void reset();
    void traverse();
};
#endif //PA8_TREE_H