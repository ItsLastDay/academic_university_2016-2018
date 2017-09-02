// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#include "pjoinnode.h"

PJoinNode::PJoinNode(PGetNextNode* left, PGetNextNode* right){
  this->left = left;
  this->right = right;
  pos = 0;
  vals = NULL;
  Initialize();
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

int PJoinNode::GetAttrNum(){
  return 1;
}
