//
// Created by Basel on 12/16/2019.
//

#ifndef HW3_SEMANTICS_HPP
#define HW3_SEMANTICS_HPP
#include "hw3_output.hpp"
#include <sstream>
#include <stdlib.h>
#include <string>
#include "Registers.hpp"
using namespace std;
#include <iostream>

/************************* ENUMS ***********************/ 
typedef enum {DIV,SUM,SUB,MUL} BOpreation;

/*******************************************************/

extern int yylineno;

struct Node {
public:
    string id;
    string type;
    Node(string id, string type) : id(id), type(type) {}
    Node() : id(""), type("") {}
    virtual ~Node() {}
};


class Statement: public Node {
public:
    vector<pair<int,BranchLabelIndex>> next_list;

    Statement(vector<pair<int,BranchLabelIndex>> nl) {
        for (int i=0;i<nl.size();i++){
            this->next_list.push_back(nl[i]);
        }
    }
    Statement() : next_list() {};

    void addToNextList(vector<pair<int,BranchLabelIndex>> nl){
        for (int i=0;i<nl.size();i++){
            this->next_list.push_back(nl[i]);
        }
    }

    void print(){
       /// cout << "printing statment";
    }
};



class NodePair : public Node {
public:
    pair<Node*,Node*> p;

    NodePair(Node* first,Node* secound){
        this->p=make_pair(first,secound);
    }

};


class BoolTerm : Node {
public :
    vector<pair<int,BranchLabelIndex>> true_list;
    vector<pair<int,BranchLabelIndex>> false_list;


    BoolTerm() : Node("", "BOOL"){};

    BoolTerm(vector<pair<int,BranchLabelIndex>> true_list,
            vector<pair<int,BranchLabelIndex>> false_list) : Node("", "BOOL"){
        this->true_list=true_list;
        this->false_list=false_list;
    }
};

class BoolOp : public Node{
public:
    string op;
    BoolOp(string operation) : Node(), op(operation){};
};

class Marker : public Node {
public:
    string quad;
    Marker(string s) : quad(s) {};
};

class BinaryOp : public Node {
    public :
    BOpreation bOpreation;

    BinaryOp(BOpreation bOpreation) : Node() {
        this->bOpreation=bOpreation;
    }
    BinaryOp(char* bOpreation): Node() {
        string bOpreation_str(bOpreation);
        if ( bOpreation_str=="+") this->bOpreation=SUM;
        if ( bOpreation_str=="/") this->bOpreation=DIV;
        if ( bOpreation_str=="-") this->bOpreation=SUB;
        if ( bOpreation_str=="*") this->bOpreation=MUL;
    }
};

class Num : public Node {
public: 
    string val;
    int is_Byte;
    string reg;
    int is_var;
    Num(string val) : is_var(0),val(val), is_Byte(0), Node("", "INT") {
        reg="NONE";
    }

    void move_to_byte(){
        if (atoi((this->val).c_str()) > 255){
            output::errorByteTooLarge(yylineno, this->val);
            exit(1);
        }
        this->is_Byte = 1;
        this->type = "BYTE";
    }
};

class ExpList : public Node {
public:
    vector<Node*> exp_Nodes;
};

class EnumValList : public Node {
public:
    vector<Node*> enumval_Nodes;
    int checkAlreadyExists(string id){
        for(vector<Node*>::iterator it = enumval_Nodes.begin(); it != enumval_Nodes.end(); ++it) {
            if(it.operator*()->id == id)
                return 1;
        }
        return 0;
    }
    vector<string>* toStrings(){
      vector<string>* v_s= new vector<string>;
      for(int i=0; i<enumval_Nodes.size();i++){
        (*(v_s))[i] = (enumval_Nodes[i])->id;
      }
      return v_s;
    }
};


#define YYSTYPE Node*

#endif //HW3_SEMANTICS_HPP
