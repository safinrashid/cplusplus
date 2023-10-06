//
// Created by safin on 11/19/2022.
//

#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <cstdint>
#include "tree_h.h"

using namespace std;


bool isOperator(string str){
    if(str == "+" || str == "-" || str == "*" || str == "/" || str == "%" ||
       str == "&&" || str == "||" || str == ">" || str == "<" || str == "==" ||
       str == "!=" || str == "<=" || str == ">="){
        return true;
    }
    return false;
}

bool isUnaryOperator(string str){
    if(str == "!" || str == "~"){
        return true;
    }
    return false;
}

bool isOperand(string str)
{
    for(char i : str) {
        if (not isdigit(i)) return false;
    }
    return true;
}

//create expression tree
tree::tree(vector<string> expression){
    root = makeTree(expression);
}

node *tree::makeTree(vector<string> &strVect) {

    if(strVect.empty()) {
        return nullptr; //finished parsing
    }

    string num = strVect[0]; //read first string
    strVect.erase(strVect.begin()); //remove read string

    if(isOperand(num)){ //operand
        return new node(stoi(num));
    }
    else if(isUnaryOperator(num)){ //operator with one operand
        node* left = makeTree(strVect);
        return new node(num, left, nullptr);
    }
    else if(isOperator(num)){ //operator with two operands
        // - (+ + * 6 7 0 1) (1)
        // - (+ + * 6 7 0 1) (+ 0 1)
        vector<string> leftVect, rightVect;
        node* left = makeTree(strVect);
        node* right = makeTree(strVect);
        return new node(num, left, right);
    }

    return nullptr;
}



//computes a node
int node::compute() const{//computes current node recursively

    //Operand (base case)
    if(not isOperator) return value;

    //recursive cases:
    int op1 = leftChild->compute();
    //Unary Operators
    if(symbol == "!"){if(op1 == 0){return 1;}return 0;}
    else if(symbol == "~") return (-1* op1);

    int op2 = rightChild->compute();
    //binary math operators
    if(symbol == "+") return (op1 + op2);
    else if(symbol == "-") return (op1 - op2);
    else if(symbol == "*") return (op1 * op2);
    else if(symbol == "/") return (op1 / op2);
    else if(symbol == "%") return (op1 % op2);
        //logic operators
    else if(symbol == "&&"){if(op1 != 0 && op2 != 0){return 1;}return 0;}
    else if(symbol == "||"){if(op1 != 0 || op2 != 0){return 1;}return 0;}
        //comparison Operators
    else if(symbol == ">"){if(op1 > op2){return 1;}return 0;}
    else if(symbol == "<"){if(op1 < op2){return 1;}return 0;}
    else if(symbol == "=="){if(op1 == op2){return 1;}return 0;}
    else if(symbol == "!="){if(op1 != op2){return 1;}return 0;}
    else if(symbol == "<="){if(op1 <= op2){return 1;}return 0;}
    else if(symbol == ">="){if(op1 >= op2){return 1;}return 0;}

    return 0;
}

//computes entire tree
int tree::computeTree() {
    return root->compute();
}



void tree::traverse(){
    traverse(this->root);
}
void tree::traverse(node* in){

    if(in == nullptr)return;
    //unary operator
    if(in->rightChild == nullptr && in->leftChild != nullptr){
        cout << in->symbol << " " << in->leftChild->value;
        return;
    }

    //dual operator
    traverse(in->leftChild);
    if(in->isOperator) cout << in->symbol << " ";
    else cout << in->value << " ";
    traverse(in->rightChild);
}


tree::~tree(){
    clear(root);
}
//frees all nodes in tree
void tree::reset(){
    clear(root);
}
void tree::clear(node *treenode){
    if(treenode == nullptr) return;//nothing to clear
    if(treenode->leftChild != nullptr){ //clear left tree
        clear(treenode->leftChild);
    }
    if(treenode->rightChild != nullptr){ //clear right tree
        clear(treenode->rightChild);
    }
    delete treenode; //destroy node
}