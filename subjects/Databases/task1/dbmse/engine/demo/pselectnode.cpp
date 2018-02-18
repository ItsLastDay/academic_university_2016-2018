// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#include <iostream>
#include <fstream>
#include <string>
#include "pselectnode.h"

PSelectNode::PSelectNode(){}

PSelectNode::PSelectNode(BaseTable* table, Predicate* predicate){
  this->table = table;
  this->predicate = predicate;
  pos = 0;
  left = NULL;
  right = NULL;
  vals = NULL;
  data.clear();
  Initialize();
}

PSelectNode::~PSelectNode(){
    input_file.close();
  delete predicate;
  delete table;
}

void PSelectNode::Initialize(){
  std::string line;
  input_file.open(table->relpath);
  if(input_file.is_open()){
    // skipping input_fileirst 2 lines
    getline(input_file, line);
    getline(input_file, line);
  }
}

void PSelectNode::Reset(){
    pos = 0;
    input_file.clear();
    input_file.seekg(0, input_file.beg);
    std::string line;
    getline(input_file, line);
    getline(input_file, line);
    data.clear();
}

std::vector<int>* PSelectNode::GetNext(){
    data.clear();

    if(input_file.eof())
        return nullptr;

    int val;
    std::string line;
    while(getline(input_file, line)){
      // we assume only 1 int predicate
      val = std::stoi(line);
      bool has_pred = false;
      if(predicate != NULL && predicate->vtype == VT_INT){
        if (predicate->ptype == PT_EQUALS){
          if (predicate->vint == val) data.push_back(val);
        }else
          if (predicate->vint < val) data.push_back(val);
        has_pred = true;
      }
      if(not has_pred)
          data.push_back(val);

      if(data.size() * sizeof(int) >= MAX_BYTE_SIZE)
          return &data;
    }

    if(not data.empty())
        return &data;
    return nullptr;
}

ErrCode PSelectNode::GetRecord(){
  if (pos == data.size()) {
      auto nx = GetNext();
      if(nx == nullptr)
          return EC_FINISH;
      pos = 0;
      // Can happen e.g. when Select's predicate filtered remaining output 
      if(data.empty()) 
          return GetRecord();
  }
  for(int i = 0; i < GetAttrNum(); i++){
    vals[i] = Value(data[pos]);
  }
  pos++;
  return EC_OK;
}

Value* PSelectNode::GetValuePos(){
  if(vals == NULL){
    vals = new Value [GetAttrNum()];
    for(int i = 0; i < GetAttrNum(); i++) vals[i] = Value(0);
  }
  return vals;
}

int PSelectNode::GetAttrNum(){
  return 1;
}

