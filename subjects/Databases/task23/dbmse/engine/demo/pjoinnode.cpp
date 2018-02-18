// A toy one-column DBMS, v0.3
// George Chernishev (c) 2016, chernishev@gmail.com
// CHANGELOG:
// 0.3: added:
//      1) implemented print features for physical trees
//      updated:
//      1) new format for data files: third line is sort status now
//      2) improved makefile targets
//      3) some code clean-up
// 0.2: first public release

#include "pjoinnode.h"
#include <cassert>

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right,
                     COLUMN_SORT cs): PGetNextNode(left, right){

  this->left = left;
  this->right = right;
  pos = 0;
  vals = NULL;
  sorted = cs;
  Initialize();
}

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right): PJoinNode(left, right, CS_UNKNOWN){
}


PJoinNode::~PJoinNode(){
  delete left;
  delete right;
}

void PJoinNode::Reset(){
    static_cast<PGetNextNode*>(left)->Reset();
    static_cast<PGetNextNode*>(right)->Reset();
    data.clear();
    pos = 0;
    left_pos = 0;
    right_pos = 0;
    left_fragment = right_fragment = nullptr;
}

void PJoinNode::upd_right()
{
    if (right_fragment == nullptr or right_pos >= right_fragment->size()) 
    {
        right_fragment = static_cast<PGetNextNode*>(right)->GetNext();
        right_pos = 0;
    }
}

void PJoinNode::upd_left()
{
    if (left_fragment == nullptr or left_pos >= left_fragment->size()) 
    {
        left_fragment = static_cast<PGetNextNode*>(left)->GetNext();
        left_pos = 0;
    }
}

void PJoinNode::reset_right()
{
    static_cast<PGetNextNode*>(right)->Reset();
    right_fragment = nullptr;
    right_pos = 0;
}

std::vector<int>* PJoinNode::GetNext(){
    data.clear();

    upd_left();

    if (left_fragment == nullptr)
        return nullptr;


    while(true) 
    {
        for(; left_pos < left_fragment->size(); left_pos++) 
        {
            upd_right();
            if (right_fragment == nullptr)
            {
                reset_right();
                upd_right();
                continue;
            }

            while(true) 
            {
                if(left_fragment->at(left_pos) == 
                        right_fragment->at(right_pos))
                {
                    data.push_back(left_fragment->at(left_pos));
                    if(data.size() * sizeof(int) >= MAX_BYTE_SIZE)
                    {
                        right_pos++;
                        return &data;
                    }
                }

                right_pos++;
                upd_right();
                if (right_fragment == nullptr)
                {
                    reset_right();
                    break;
                }
            }

        }

        upd_left();
        if (left_fragment == nullptr)
            break;
    }

    if(not data.empty())
        return &data;
    return nullptr;
}

void PJoinNode::Initialize(){
    pos = 0;
    left_pos = 0;
    right_pos = 0;
    left_fragment = right_fragment = nullptr;

    // For each element from the left son, we find the apropriate 
    // element in the right one, and the output. This preserves
    // order of the left relation.
    sorted = static_cast<PGetNextNode*>(left)->GetSortednessStatus();
}

ErrCode PJoinNode::GetRecord(){
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

Value* PJoinNode::GetValuePos(){
  if(vals == NULL){
    vals = new Value [GetAttrNum()];
    for(int i = 0; i < GetAttrNum(); i++) vals[i] = Value(0);
  }
  return vals;
}

void PJoinNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout<<" ";
  }
  std::cout<<"NESTED LOOP JOIN";
  std::cout<<" on "<<sorted<<" data\n";
  left->Print(indent + 2);
  right->Print(indent + 2);
}
