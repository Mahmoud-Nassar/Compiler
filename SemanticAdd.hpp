#ifndef SemanticAdd_H
#define SemanticAdd_H

#include "semantics.hpp"
#include "scoping.hpp"
#include "Registers.hpp"
#include "bp.hpp"
#include <assert.h>
using namespace std;
#include <iostream>


/** Exp: Exp ADDSUBOP Exp */ 

Node* expToNum(Node* node,Registers& global_registers, CodeBuffer& Buffer,bool is_byte);
	
/*************************/

/** Exp: Exp ADDSUBOP Exp */
Node* expArthExp(Node* res_node,Node* node1,Node* node2,Node* node3,Registers& global_registers, CodeBuffer& Buffer);
/************************/

/** Exp: Exp EQOP Exp and Exp: Exp CMPOP Exp */
Node* ExpEQOPExp(Node* res_node,Node* exp1,Node* eq_op,Node* exp2,Registers&
    global_registers, CodeBuffer& Buffer);

/** Exp: NOT Exp */
Node* ExpNotExp(Node* node2,Registers&
    global_registers, CodeBuffer& Buffer);

Node* ExptoTrue(Registers&
global_registers, CodeBuffer& Buffer);


Node* ExptoFalse(Registers&
global_registers,CodeBuffer& Buffer);

Node* MarkerHandel(CodeBuffer& Buffer);

Node* ExpAndExp(Node* exp1,Node* mr,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer);

Node* ExpOrExp(Node* exp1,Node* mr,Node* exp2,Registers&
    global_registers, CodeBuffer& Buffer);

Node* MIFSTART(Node* exp1,Node* ,Node* exp2,Registers&
    global_registers, CodeBuffer& Buffer);


Node* If1(Node* exp1,Node* mr);

Node* If2(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer);

Node* If5(Node* exp1,Node* st,Registers& global_registers, CodeBuffer& Buffer);

Node* If4(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer);

Node* StatmentHandler1(Node* st1,Registers&
global_registers, CodeBuffer& Buffer);

Node* StatmentHandler2(Node* st1,Registers&
global_registers, CodeBuffer& Buffer);

Node* StatmentHandler3(Node* id,Node* num_node,Registers&
global_registers, CodeBuffer& Buffer,scopeTables& symbol_table);

Node* StatmentHandler11(Node* sts,Node* st,Node* mr,Registers&
global_registers, CodeBuffer& Buffer);

Node* IfElse1(Node* exp1,int indecator,Registers&
    global_registers, CodeBuffer& Buffer);

Node* IfElse1(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer);

Node* MIfEnd1(int indecator,Registers&
global_registers, CodeBuffer& Buffer);

Node* MarkeToSkipElse(Registers&
global_registers, CodeBuffer& Buffer);

Node* IfElseStatment(Node* exp1,Node* node_n,Node* node_m2,Node*
node_statment1,Node* node_statment2,Registers& global_registers, CodeBuffer&
Buffer);

Node* ElseM(Registers& global_registers, CodeBuffer& Buffer);

Node* AssignHandel1();

void FunctionHandler1(Registers& global_registers, CodeBuffer& Buffer);

Node* DeclareID(Node* type,Node* id,Registers& global_registers, CodeBuffer&
Buffer,scopeTables& symbol_table);

void assign(int value,Node* id,Registers& global_registers, CodeBuffer&
Buffer,scopeTables& symbol_table);

void check(int i,CodeBuffer& Buffer);
#endif