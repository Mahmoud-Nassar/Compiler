//
// Created by Basel on 12/16/2019.
//

#ifndef HW3_SCOPING_HPP
#define HW3_SCOPING_HPP

#include <vector>
#include <iostream>
#include "hw3_output.hpp"
#include <assert.h>
#include <stdio.h>
#include "semantics.hpp"

enum Entry_Type{
    IS_FUNC,
    IS_PARAM,
    IS_VAR,
    IS_ENUM,
    IS_ENUM_VAL,
    IS_ENUM_VAR,
    IS_ENUM_PARAM,
};

using namespace std;

class scope_Entry;

typedef vector<scope_Entry> scope;

class scope_Entry {
public:
    string id;
    string type;
    int offset;
    Entry_Type et;
  string ret_Type;
  vector<string> args_Types;
  vector<string> enum_values;
  string enum_type;
    scope_Entry(string nid, string ntype ,Entry_Type net) : id(nid) , offset(0), type(ntype) ,ret_Type(ntype),enum_type(""), et(net) {}

    scope_Entry(const scope_Entry& entry) : id(entry.id),  type(entry.type), offset(entry.offset) , et(entry.et) , ret_Type(entry.ret_Type)
    , args_Types(entry.args_Types) , enum_values(entry.enum_values) , enum_type(entry.enum_type){}

  void addArgType(string type){

    args_Types.push_back(type);
  }
  bool checkLegalVal(string v) {

    for (int i = 0; i < enum_values.size(); i++) {
      if (v == enum_values[i])
        return true;
    }
    return false;
  }
  void addEnumVal(string val){
    enum_values.push_back(val);
  }
  string getEnumType(){
    return "enum "+ enum_type;
  }

};

class scopeTables {
public:
    vector<vector<scope_Entry>> scopeStack;
    vector<int> offsets;
  int get_next_param_offset(){
    assert(scopeStack.size() != 0);
    assert(scopeStack[0].size() > 2);
    return (-1*((scopeStack[0]).back().args_Types.size()))-1;
  }
  int get_next_offset(){
    return offsets.empty() ? 0 : offsets.back();
  }

    void openScope() {
        scope new_scope;
        scopeStack.push_back(new_scope);
        int curr_off = get_next_offset();
        offsets.push_back(curr_off);
    }

    void close_Scope() {
        for (int i = 0; i < scopeStack.back().size(); i++) {
            if ((scopeStack.back())[i].et != IS_ENUM && (scopeStack.back())[i].et != IS_ENUM_VAR && (scopeStack.back())[i].et != IS_ENUM_VAL) {
                if ((scopeStack.back())[i].et == IS_FUNC) {
                    output::printID((scopeStack.back())[i].id, 0,
                                    output::makeFunctionType(
                                            ((scopeStack.back())[i]).ret_Type,
                                            (((scopeStack.back())[i]).args_Types)));
                } else {
                    output::printID((scopeStack.back())[i].id, (scopeStack.back())[i].offset,
                                    (scopeStack.back())[i].type);
                }
            } else {
                if((scopeStack.back())[i].et == IS_ENUM_VAR){
                    output::printID((scopeStack.back())[i].id, (scopeStack.back())[i].offset,
                                    ((scopeStack.back())[i]).getEnumType());
                }

            }
        }
      for (int i = 0; i < scopeStack.back().size(); i++)
      if ((scopeStack.back())[i].et == IS_ENUM) {
        output::printEnumType(scopeStack.back()[i].id,
                              ((scopeStack.back()[i]).enum_values));
      }
        scopeStack.pop_back();
        offsets.pop_back();

    }


  scope_Entry* find_func_entry_by_ID(string id){
    vector<scope>::iterator it;
    scope::iterator i;
    for (it = scopeStack.begin(); it != scopeStack.end(); it++) {
      for (i = (*it).begin(); i != (*it).end(); i++) {
        if((*i).et == IS_FUNC && (*i).id == id){
          scope_Entry* ret = (&(*i));
          return ret;
        }
      }
    }
    return NULL;
  }
    void insert_Entry(string id, string type, Entry_Type et) {
        if (et == IS_VAR) {
            scope_Entry new_var(id, type, IS_VAR);
            new_var.offset = get_next_offset();
            scopeStack.back().push_back(new_var);
            offsets.back()++;
        } else if (et == IS_FUNC) {
            scope_Entry new_func(id, type, IS_FUNC);
            new_func.offset = 0;
            scopeStack.back().push_back(new_func);
        } else if (et == IS_PARAM) {
            scope_Entry new_param(id, type, IS_PARAM);
            new_param.offset = get_next_param_offset();
            scopeStack.back().push_back(new_param);
            // add to the args_types of the last inserted func
          (scopeStack[0]).back().args_Types.push_back(type);

        }
        else if(et == IS_ENUM_VAL){
          scope_Entry new_val(id, type, IS_ENUM_VAL);
          scopeStack.back().push_back(new_val);
        }
    }

    void insert_enum_entry(string id,string type,EnumValList* elist){
            scope_Entry new_enum(id,type,IS_ENUM);
            vector<string> vs;
            for(int i=0 ; i<elist->enumval_Nodes.size();i++){
              vs.push_back(elist->enumval_Nodes[i]->id);
            }
            new_enum.enum_values=vector<string>(vs);
            new_enum.offset = get_next_offset();
            scopeStack.back().push_back(new_enum);

    }
    void insert_enumVar_Entry(string id,string type,string enumtype){
        scope_Entry new_evar(id,type,IS_ENUM_VAR);
        new_evar.offset=get_next_offset();
        new_evar.enum_type = enumtype;
        scopeStack.back().push_back(new_evar);
        offsets.back()++;
    }
  void insert_enumPar_Entry(string id,string type,string enumtype){
      string s = "enum "+enumtype;
    scope_Entry new_param(id,s,IS_ENUM_PARAM);
    new_param.offset = get_next_param_offset();
    new_param.enum_type=enumtype;
    scopeStack.back().push_back(new_param);
    (scopeStack[0]).back().args_Types.push_back(s);
  }

  scope_Entry* find_entry_by_ID(string id, int entry_Def){
    vector<scope>::iterator it;
    scope::iterator i;
    for (it = scopeStack.begin(); it != scopeStack.end(); it++) {
      for (i = (*it).begin(); i != (*it).end(); i++) {
        if((*i).et == entry_Def && (*i).id == id){

          return &(*i);
        }
      }
    }
    return NULL;
  }

    string getEntryType(string id) {
      vector<scope>::iterator it;
      scope::iterator i;
      string s = "";
      for (it = scopeStack.begin(); it != scopeStack.end(); it++) {
        for (i = (*it).begin(); i != (*it).end(); i++) {
          if ((*i).id == id) {
            switch((*i).et){
            case IS_ENUM: { s+="ENUM"; break;}
            case IS_ENUM_VAR: {s+=(*i).getEnumType(); break;}
            case IS_ENUM_VAL: {s+="ENUM_VAL"; break;}
            default: {s+=(*i).type;}
            }
          }
        }
      }
      return s;
    }
  string getEnumAndIdType(string id) {
    vector<scope>::iterator it;
    scope::iterator i;
    string s = "";
    for (it = scopeStack.begin(); it != scopeStack.end(); it++) {
      for (i = (*it).begin(); i != (*it).end(); i++) {
        if ((*i).id == id && (*i).et == IS_ENUM) {
          s += "enum ";
          s += (*i).id;
        }
      }
    }
    return s;
    }

    scope_Entry* return_entry_ptr(string id){
        vector<scope>::iterator it;
        scope::iterator i;
        for (it = scopeStack.begin(); it != scopeStack.end(); it++) {
            for (i = (*it).begin(); i != (*it).end(); i++) {
                if((*i).id == id){
                    return &(*i);
                }
            }
        }
        return NULL;
    }

      bool check_entry_exists(string id){
        return (return_entry_ptr(id)!=NULL);
      }
    scope_Entry* find_enum_entry_by_ID(string id){
        vector<scope>::iterator it;
        scope::iterator i;
        for (it = scopeStack.begin(); it != scopeStack.end(); it++) {
            for (i = (*it).begin(); i != (*it).end(); i++) {
                if((*i).et == IS_ENUM && (*i).id == id){
                    return &(((*i)));
                }
            }
        }
        return NULL;
    }
    vector<string>* get_func_args_types_by_id(string id){
        scope_Entry* rc = this->find_func_entry_by_ID(id);
        if(rc == NULL){
            return NULL;
        }
        return &(rc->args_Types);
    }
    vector<string>* get_enum_vals_by_id(string id){
       scope_Entry* rc = this->find_enum_entry_by_ID(id);
        if(rc == NULL){
            return NULL;
        }
        return &(rc->enum_values);
    }

    string get_last_inserted_func_retType(){
        assert(scopeStack.size() != 0);
        assert(scopeStack[0].size() > 2);
        return ((scopeStack[0]).back()).ret_Type;
    }



};


#endif //HW3_SCOPING_HPP
