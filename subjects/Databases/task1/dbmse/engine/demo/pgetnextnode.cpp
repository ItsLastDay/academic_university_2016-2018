// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#include "pgetnextnode.h"

PGetNextNode::PGetNextNode(): PResultNode(NULL, NULL){
  Initialize();
}

void PGetNextNode::Initialize(){
  return;
}

void PGetNextNode::Reset(){
    return;
}

std::vector<int>* PGetNextNode::GetNext(){
  return NULL;
}
