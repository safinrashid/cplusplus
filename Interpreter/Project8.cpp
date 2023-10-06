#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <string>
#include "Parse.h"
#include "tree_h.h"
#include <map>

using namespace std;

vector <string> line;
map <string, int> variables;
string input, temp;
bool stop = false;

void text();
void var();
void output();
void set();

//convert pointer to literal string
string stringify(const char *ptr){
    string out = "";
    for(int i = 0; ptr[i] != '\0'; i++){
        out.push_back(ptr[i]);
    }
    return out;
}

void next_token_string(){
    read_next_token();
    input = stringify(next_token());
}

//turns line to vector of strings
void vectorize(){
    bool isEnd = 0;
    while(input != "text" && input != "output" && input != "var" && input != "set"){
        //cout << ":)";
        if(input == "//"){skip_line(); break;}//skip rest of line if comment
        line.push_back(input);
        next_token_string();
        if (next_token_type == END) isEnd = 1;
    }

    for(int i = 0; i < line.size(); i++){
        if(variables.count(line[i])){
            line[i] = to_string(variables.at(line[i]));
        }
    }

    if(isEnd){
        stop = true;
    }
    //cout << "DEBUG: " << input <<  " " << next_token_type <<endl;

}


void text(){
    read_next_token();
    string print = stringify(next_token()); //convert text to string

    if(print == "//") skip_line(); //don't print rest of line if comment

    cout << print;

    //if(input == "text") text();
    if(input == "output") output();
    else if(input == "var") var();
    else if(input == "set") set();
}

void output(){

    next_token_string();
    vectorize();

    //line = varToValues(line);
    //for(int i = 0; i<line.size(); i++) cout << line[i] << " ";
    tree parse(line);//turn line into expression tree
    //parse.traverse();
    int out = parse.computeTree();
    cout << parse.computeTree();
    //test tree
    //parse.traverse();
    //for(int i = 0; i<line.size(); i++) cout << line[i] << " ";
//cout<<input;

    line.clear();

    if(input == "text") text();
    else if(stop) return;
    else if(input == "output") output();
    else if(input == "var") var();
    else if(input == "set") set();

}

void var(){
    next_token_string();
    string varName = input; //store variable name
    bool exists = variables.count(varName);

    next_token_string();
    vectorize();
    //for(int i = 0; i<line.size(); i++) cout << line[i] << " ";
    tree parse(line);//turn line into expression tree
    //parse.traverse();
    int varValue = parse.computeTree();
    //cout<<varName<<"="<<varValue;
    if(exists){//modify with warning
        variables.at(varName) = varValue;
        cout << "variable " << varName << " incorrectly re-initialized" << endl;
    }else{//declare
        variables.emplace(varName, varValue);
    }

    line.clear();

    if(input == "text") text();
    else if(input == "output") output();
    else if(stop) return;
    else if(input == "var") var();
    else if(input == "set") set();

}

void set(){
    next_token_string();
    string varName = input; //store variable name

    //if variable doesnt exist
    bool dne = ! variables.count(varName);
    //cout << dne;
    if(dne){
        cout << "variable " << varName << " not declared" << endl;
        return;
    }
    next_token_string();
    vectorize(); //turn line into str vector

    tree parse(line);
    variables.erase(varName);
    variables.emplace(varName, parse.computeTree());

    line.clear();

    if(input == "text") text();
    else if(input == "output") output();
    else if(input == "var") var();
    else if(stop) return;
    else if(input == "set") set();
}

void run(){

    while(next_token_type != END){
        input = stringify(next_token());//convert token to string
        //cout<<input;
        if(input == "text") text();
        else if(input == "output") output();
        else if(input == "var") var();
        else if(input == "set") set();
        //comment
        if(input == "//") skip_line();

        if(stop) break;
        read_next_token();
    }

    stop = false;
    line.clear();               //reset string vector
    variables.clear();          //reset variable map
    next_token_type = INVALID;  //reset token status
}

void stattest(){
    
}


int main() {
    static int test = 0;
    for(int i = 0; i < 5; i++){
        test++;
        cout << test << " ";
    }


    set_input("test1.blip");
    run();

    return 0;
}