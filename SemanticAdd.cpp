/**

#include "SemanticAdd.hpp"
#include "Registers.hpp"
#include "bp.hpp"
#include <iostream>

using namespace std;


Node* expToNum(Node* node,Registers& global_registers, CodeBuffer& Buffer,bool is_byte){
    Num* number_ptr=((Num*)(node));
    std::string number= number_ptr->val;
    string reg_str=global_registers.allocateRegister();
    if (is_byte){
        Buffer.emit("%"+reg_str+" = add i8 "+number+", "+"0");
        Buffer.emit("sext i8 %" +reg_str+" to i32");
    } else {
        Buffer.emit("%"+reg_str+" = add i32 "+number+", "+"0");
    }
    number_ptr->reg=reg_str;
    return number_ptr;
}


Node* expArthExp(Node* res_node,Node* node1,Node* node2,Node* node3,Registers& global_registers, CodeBuffer& Buffer){
    std::string reg_str1=((Num*)node1)->reg;
    std::string reg_str2=((Num*)node3)->reg;
    BOpreation bOpreation=((BinaryOp*)(node2))->bOpreation;
    string res_reg_str=global_registers.allocateRegister();
    string opreation="";
    switch (bOpreation){
        case SUM : opreation="add";break;
        case SUB : opreation="sub";break;
        case DIV : opreation="udiv";break;
        case MUL : opreation="mul";break;
    }
    opreation="%"+res_reg_str+" = "+opreation+" i32 %"+reg_str1+", %"+reg_str2;
    Buffer.emit(opreation);
    Num* res_num=((Num*)(res_node));
    assert(((Num*)node1)->is_Byte==((Num*)node3)->is_Byte);
    if (((Num*)node1)->is_Byte) res_num->is_Byte=1;
    res_num->reg=res_reg_str;
    return ((Node*)res_num);
}

Node* ExpEQOPExp(Node* res_node,Node* exp1,Node* eq_op,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){
    int true_list_address;
    int false_list_address;


    string exp1_str=((Num*)exp1)->reg;
    string exp2_str=((Num*)exp2)->reg;
    string op_str=((BoolOp*)exp2)->op;
    string operation=((BoolOp*)eq_op)->op;


    true_list_address=Buffer.emit("if "+ exp1_str+ operation+exp2_str+" goto @"
    );
    false_list_address=Buffer.emit("goto @");

    pair<int,BranchLabelIndex> true_pair(true_list_address,SECOND);
    pair<int,BranchLabelIndex> false_pair(false_list_address,FIRST);

    vector<pair<int,BranchLabelIndex>> true_list=Buffer.makelist(true_pair);
    vector<pair<int,BranchLabelIndex>> false_list=Buffer.makelist(false_pair);

    BoolTerm* term_object=new BoolTerm(true_list,false_list);
    return ((Node*)term_object);
}

Node* ExpNotExp(Node* node2,Registers&
global_registers, CodeBuffer& Buffer){
    BoolTerm *boolTerm=((BoolTerm*)node2);
    BoolTerm* term_object=new BoolTerm(boolTerm->false_list,boolTerm->true_list);
    return ((Node*)term_object);
}

Node* ExptoTrue(Registers&
global_registers, CodeBuffer& Buffer){
    int address=Buffer.emit("goto @");
    pair<int,BranchLabelIndex> true_pair(address,SECOND);
    vector<pair<int,BranchLabelIndex>> true_list=Buffer.makelist(true_pair);
    vector<pair<int,BranchLabelIndex>> empty_list;
    BoolTerm* term_object=new BoolTerm(true_list,empty_list);
    return ((Node*)term_object);
}

Node* ExptoFalse(Registers&
global_registers,CodeBuffer& Buffer){
    int address=Buffer.emit("goto @");
    pair<int,BranchLabelIndex> false_pair(address,SECOND);
    vector<pair<int,BranchLabelIndex>> false_list=Buffer.makelist(false_pair);
    vector<pair<int,BranchLabelIndex>> empty_list;
    BoolTerm* term_object=new BoolTerm(empty_list,false_list);
    return ((Node*)term_object);
}

Node* MarkerHandel(CodeBuffer& Buffer){
    string new_label=Buffer.genLabel();
    Marker* marker=new Marker(new_label);
    return marker;
}

Node* ExpAndExp(Node* exp1,Node* mr,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){

    BoolTerm* term1=((BoolTerm*)exp1);
    BoolTerm* term2=((BoolTerm*)exp2);
    Marker* marker=((Marker*)mr);
    Buffer.bpatch(term1->true_list,marker->quad);
    BoolTerm* term=new BoolTerm(term2->true_list,Buffer.merge(term1->false_list,
                                                              term2->false_list));
    return ((Node*)term);
}

Node* ExpOrExp(Node* exp1,Node* mr,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){

    BoolTerm* term1=((BoolTerm*)exp1);
    BoolTerm* term2=((BoolTerm*)exp2);
    Marker* marker=((Marker*)mr);
    Buffer.bpatch(term1->false_list,marker->quad);

    BoolTerm* term=new BoolTerm(Buffer.merge(term1->true_list,
                                             term2->true_list),term2->false_list);

    return ((Node*)term);
}

Node* MIFSTART(Node* exp1,Node* ,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){

}

Node* If1(Node* exp1,Node* mr,Node *if_statement,Registers&
global_registers, CodeBuffer& Buffer){
    BoolTerm* boolTerm=((BoolTerm*)exp1);
    Buffer.bpatch(boolTerm->true_list,((Marker*)mr)->quad);
    Statement* statement=new Statement(boolTerm->false_list);
    ///  Statement* statement1=((Statement*)if_statement);
    /// statement->addToNextList(statement1->next_list);
    return statement;

}

Node* If2(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer){
    Statement* statements=((Statement*)exp1);
    return statements;
}

Node* If4(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer){
    Statement* statement_st1=((Statement*)exp1);

    Buffer.bpatch(statement_st1->next_list,Buffer.genLabel());

    Statement* new_statement=new Statement();
    return new_statement;
}

Node* MarkerHandel2(CodeBuffer& Buffer){
    Marker* marker=new Marker(Buffer.genLabel());
    return marker;
}

Node* StatmentHandler1(Node* st1,Registers&
global_registers, CodeBuffer& Buffer) {

    Statement *statement_st1 = ((Statement *) st1);


    Statement* new_statement=new Statement();
    return new_statement;
}

Node* StatmentHandler11(Node* st1,Node* mr,Node* st2,Registers&
global_registers, CodeBuffer& Buffer) {
    Statement *statement_st1 = ((Statement *) st1);

    Statement* new_statement=new Statement(statement_st1->next_list);
    return new_statement;
}

Node* StatmentHandler2(Node* st1,Registers&
global_registers, CodeBuffer& Buffer){

    Statement* statement_st1=((Statement*)st1);
    /// Statement* new_statement=new Statement(statement_st1->next_list);
    return statement_st1;
}

Node* StatmentHandler3(Registers&
global_registers, CodeBuffer& Buffer){
    Statement* statement=new Statement();
    return statement;
}


*/

/**
void printBuffer(CodeBuffer& Buffer){
	std::cout << "************* BUFFER ***********"
    <<std::endl;
    Buffer.printCodeBuffer();
    std::cout << "********************************"
    <<std::endl;
}
*/



#include "SemanticAdd.hpp"
#include "Registers.hpp"
#include "bp.hpp"
#include <iostream>
#include "scoping.hpp"

using namespace std;


Node* expToNum(Node* node,Registers& global_registers, CodeBuffer& Buffer,bool is_byte){
	Num* number_ptr=((Num*)(node));
	std::string number= number_ptr->val;
    	string reg_str=global_registers.allocateRegister();
	if (is_byte){
		Buffer.emit("%"+reg_str+" = add i8 "+number+", "+"0");
		Buffer.emit("sext i8 %" +reg_str+" to i32");
	} else {
		Buffer.emit("%"+reg_str+" = add i32 "+number+", "+"0");
	}
	number_ptr->reg=reg_str;
return number_ptr;
}


Node* expArthExp(Node* res_node,Node* node1,Node* node2,Node* node3,Registers& global_registers, CodeBuffer& Buffer){
    std::string reg_str1=((Num*)node1)->reg;
    std::string reg_str2=((Num*)node3)->reg;
    BOpreation bOpreation=((BinaryOp*)(node2))->bOpreation;
    string res_reg_str=global_registers.allocateRegister();
    string opreation="";
    switch (bOpreation){
        case SUM : opreation="add";break;
        case SUB : opreation="sub";break;
        case DIV : opreation="udiv";break;
        case MUL : opreation="mul";break;
    }
    opreation="%"+res_reg_str+" = "+opreation+" i32 %"+reg_str1+", %"+reg_str2;
    Buffer.emit(opreation);
    Num* res_num=((Num*)(res_node));
    assert(((Num*)node1)->is_Byte==((Num*)node3)->is_Byte);
    if (((Num*)node1)->is_Byte) res_num->is_Byte=1;
    res_num->reg=res_reg_str;
    return ((Node*)res_num);
}

Node* ExpEQOPExp(Node* res_node,Node* exp1,Node* eq_op,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){
    int true_list_address;
    int false_list_address;


    string exp1_str=((Num*)exp1)->reg;
    string exp2_str=((Num*)exp2)->reg;
    string op_str=((BoolOp*)exp2)->op;
    string operation=((BoolOp*)eq_op)->op;


    true_list_address=Buffer.emit("if "+ exp1_str+ operation+exp2_str+" goto @"
    );
    false_list_address=Buffer.emit("goto @");

    pair<int,BranchLabelIndex> true_pair(true_list_address,SECOND);
    pair<int,BranchLabelIndex> false_pair(false_list_address,FIRST);

    vector<pair<int,BranchLabelIndex>> true_list=Buffer.makelist(true_pair);
    vector<pair<int,BranchLabelIndex>> false_list=Buffer.makelist(false_pair);

    BoolTerm* term_object=new BoolTerm(true_list,false_list);
    return ((Node*)term_object);
}

Node* ExpNotExp(Node* node2,Registers&
global_registers, CodeBuffer& Buffer){
    BoolTerm *boolTerm=((BoolTerm*)node2);
    BoolTerm* term_object=new BoolTerm(boolTerm->false_list,boolTerm->true_list);
    return ((Node*)term_object);
}

Node* ExptoTrue(Registers&
global_registers, CodeBuffer& Buffer){
    int address=Buffer.emit("goto @");
    pair<int,BranchLabelIndex> true_pair(address,SECOND);
    vector<pair<int,BranchLabelIndex>> true_list=Buffer.makelist(true_pair);
    vector<pair<int,BranchLabelIndex>> empty_list;
    BoolTerm* term_object=new BoolTerm(true_list,empty_list);
    return ((Node*)term_object);
}

Node* ExptoFalse(Registers&
global_registers,CodeBuffer& Buffer){
    int address=Buffer.emit("goto @");
    pair<int,BranchLabelIndex> false_pair(address,SECOND);
    vector<pair<int,BranchLabelIndex>> false_list=Buffer.makelist(false_pair);
    vector<pair<int,BranchLabelIndex>> empty_list;
    BoolTerm* term_object=new BoolTerm(empty_list,false_list);
    return ((Node*)term_object);
}

Node* MarkerHandel(CodeBuffer& Buffer){
    string new_label=Buffer.genLabel();
    int i=(new_label=="label_6");
    Marker* marker=new Marker(new_label);
    return marker;
}

Node* ExpAndExp(Node* exp1,Node* mr,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){

    BoolTerm* term1=((BoolTerm*)exp1);
    BoolTerm* term2=((BoolTerm*)exp2);
    Marker* marker=((Marker*)mr);
    Buffer.bpatch(term1->true_list,marker->quad);
    BoolTerm* term=new BoolTerm(term2->true_list,Buffer.merge(term1->false_list,
                                                              term2->false_list));
    return ((Node*)term);
}

Node* ExpOrExp(Node* exp1,Node* mr,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){

    BoolTerm* term1=((BoolTerm*)exp1);
    BoolTerm* term2=((BoolTerm*)exp2);
    Marker* marker=((Marker*)mr);
    Buffer.bpatch(term1->false_list,marker->quad);

    BoolTerm* term=new BoolTerm(Buffer.merge(term1->true_list,
                                             term2->true_list),term2->false_list);

    return ((Node*)term);
}

Node* MIFSTART(Node* exp1,Node* ,Node* exp2,Registers&
global_registers, CodeBuffer& Buffer){

}

Node* If1(Node* exp1,Node* mr){
    return new NodePair(exp1,mr);
  /**  Buffer.bpatch(boolTerm->true_list,((Marker*)mr)->quad);
     Statement* statement=new Statement(boolTerm->false_list);
      Statement* statement1=((Statement*)if_statement);
     statement->addToNextList(statement1->next_list);
    return ((Node*)boolTerm);*/
}

Node* If5(Node* exp1,Node* st,Registers& global_registers, CodeBuffer& Buffer)
    { /** exp1 is pair of boolexp and marker for the if_statment */

    NodePair* nodePair=((NodePair*)exp1);
    Marker* mr=((Marker*)(nodePair->p.second));
    BoolTerm* boolTerm=((BoolTerm*)(nodePair->p.first));

    Buffer.bpatch(boolTerm->true_list,((Marker*)mr)->quad);

    Statement* if_statment=((Statement*)st);
    Statement* statement=new Statement(boolTerm->false_list);
    statement->addToNextList(if_statment->next_list);

    return statement;
}

Node* If2(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer){
    Statement* statements=((Statement*)exp1);
    return statements;
}

Node* IfElse1(Node* exp1,Registers&
    global_registers, CodeBuffer& Buffer){
    /** exp1 is pair of boolexp and marker for the if_statment */

    NodePair* nodePair=((NodePair*)exp1);
    Marker* mr=((Marker*)(nodePair->p.second));
    BoolTerm* boolTerm=((BoolTerm*)(nodePair->p.first));

    Buffer.bpatch(boolTerm->true_list,((Marker*)mr)->quad);

    /** bpatch the false list with the secound marker */

    /** TODO: fill new_statement with S1.nextlist N.nextlist and S.nextlist*/
    Statement* new_statement=new Statement();
    return new_statement;
}

Node* If4(Node* exp1,Registers&
global_registers, CodeBuffer& Buffer){
    Statement* statement_st1=((Statement*)exp1);

    /// Buffer.bpatch(statement_st1->next_list,Buffer.genLabel());

    Statement* new_statement=new Statement();
    return new_statement;
}


Node* StatmentHandler1(Node* st1,Registers&
global_registers, CodeBuffer& Buffer) {

    Statement *statement_st1 = ((Statement *) st1);

    Statement* new_statement=new Statement(statement_st1->next_list);
    return new_statement;
}

Node* StatmentHandler11(Node* sts,Node* st,Node* mr,Registers&
global_registers, CodeBuffer& Buffer) {
    Statement *statement_st = ((Statement *) st);
    Statement *statements = ((Statement *) sts);
    Marker* marker=((Marker*)mr);

    Buffer.bpatch(statements->next_list,marker->quad);
    /** NEED TO BPATCH */

    Statement* new_statement=new Statement(statement_st->next_list);
    return new_statement;
}

Node* StatmentHandler2(Node* st1,Registers&
    global_registers, CodeBuffer& Buffer){

    Statement* statement_st1=((Statement*)st1);
    /// Statement* new_statement=new Statement(statement_st1->next_list);
    return statement_st1;
}

Node* StatmentHandler3(Node* id,Node* num_node,Registers&
    global_registers, CodeBuffer& Buffer,scopeTables& symbol_table){
    Num* num=((Num*)num_node);
    assign(stoi(num->val),id,global_registers,Buffer,symbol_table);
    Statement* statement=new Statement();
    return statement;
}

Node* MIfEnd1(int indecator,Registers&
    global_registers, CodeBuffer& Buffer){

    if (indecator==1){ /// it`s just and if statment without else


    } else if (indecator==2){ /// first MEndIf in if else statment


    } else if (indecator==3){/// secound MEndIf in if else statment


    }
}

Node* MarkeToSkipElse(Registers&
global_registers, CodeBuffer& Buffer){
    vector<pair<int,BranchLabelIndex>> next_list;
    pair<int,BranchLabelIndex> p(Buffer.emit("goto @"),FIRST);
    next_list.push_back(p);
    Statement* statement=new Statement(next_list);
    return statement;
}

Node* IfElseStatment(Node* exp1,Node* node_n,Node* node_m2,Node*
    node_statment1,Node* node_statment2,Registers& global_registers, CodeBuffer&
                                                                Buffer){
    /** exp1 is pair of boolexp and marker for the if_statment */

    NodePair* nodePair=((NodePair*)exp1);
    Marker* m1=((Marker*)(nodePair->p.second));
    BoolTerm* boolTerm=((BoolTerm*)(nodePair->p.first));

    NodePair* nodePair1=((NodePair*)node_n);
    Marker* m2=((Marker*)(nodePair1->p.second));

    Buffer.bpatch(boolTerm->true_list,m1->quad);
    Buffer.bpatch(boolTerm->false_list,m2->quad);

    Statement* statement1=((Statement*)node_statment1);
    Statement* statement2=((Statement*)node_statment2);


    Statement* n=((Statement*)(nodePair1->p.first));

    Statement* statement= new Statement(statement1->next_list);
    statement->addToNextList(n->next_list);
   /// statement->addToNextList(statement2->next_list);
    return statement;
}

Node* ElseM(Registers& global_registers, CodeBuffer& Buffer){

           Statement* statement=((Statement*)MarkeToSkipElse(global_registers,
                   Buffer));
           Marker* m2=((Marker*)MarkerHandel(Buffer));

          return new NodePair(statement,m2);
}

void FunctionHandler1(Registers& global_registers, CodeBuffer& Buffer){

    string funcArray=global_registers.allocateRegister();
    Buffer.emit("%"+funcArray+" = alloca [50 x i32]");
    global_registers.funcs_params_array_pts.push_back(funcArray);

}


Node* DeclareID(Node* type,Node* id,Registers& global_registers, CodeBuffer&
    Buffer,scopeTables& symbol_table){
    symbol_table.insert_Entry(id->id, type->type, IS_VAR);

    assign(0,id,global_registers,Buffer,symbol_table);

    Statement* statement=new Statement();
    return statement;
}

void assign(int value,Node* id,Registers& global_registers, CodeBuffer&
    Buffer,scopeTables& symbol_table){

    int index=global_registers.funcs_params_array_pts.size();
    string para_array_ptr=global_registers.funcs_params_array_pts[index-1];
    scope_Entry* scopeEntry=symbol_table.return_entry_ptr(id->id);
    int offset=scopeEntry->offset;
    Buffer.emit("%"+id->id+" = getelementptr [10 x i32],[10 x i32]* "
                           "%"+global_registers.get_curr_func_params_array_ptr()
                +", i32"
                 " 0,i32 "+to_string(offset));

    Buffer.emit("store i32 "+to_string(value)+", i32* %"+id->id);

}

Node* AssignHandel1(){
    return new Statement();
}

void check(int i,CodeBuffer& Buffer){
    int k=i;
}

/**
void printBuffer(CodeBuffer& Buffer){
	std::cout << "************* BUFFER ***********"
    <<std::endl;
    Buffer.printCodeBuffer();
    std::cout << "********************************"
    <<std::endl;
}
*/