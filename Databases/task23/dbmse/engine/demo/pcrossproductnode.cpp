#include "pcrossproductnode.h"
#include <vector>
#include <cstdlib>

PCrossProductNode::PCrossProductNode(PGetNextNode *left, PGetNextNode *right)
    : PGetNextNode(left, right)
{
    pos = 0;
    this->left = left;
    this->right = right;
    vals = NULL;
    Initialize();
} 

PCrossProductNode::~PCrossProductNode()
{
    delete left;
    delete right;
}

static int length_base_10(int x)
{
    if(x == 0)
        return 1;
    int ret = 0;
    while(x) {
        x /= 10;
        ret++;
    }
    return ret;
}

void PCrossProductNode::Initialize(){
    pos = 0;
    left_pos = 0;
    right_pos = 0;
    left_fragment = right_fragment = nullptr;
    sorted = GetCombinedStatus(
            static_cast<PGetNextNode*>(left)->GetSortednessStatus(), 
            static_cast<PGetNextNode*>(right)->GetSortednessStatus());
}

void PCrossProductNode::Reset(){
    static_cast<PGetNextNode*>(left)->Reset();
    static_cast<PGetNextNode*>(right)->Reset();
    data.clear();
    pos = 0;
    left_pos = 0;
    right_pos = 0;
    left_fragment = right_fragment = nullptr;
}

void PCrossProductNode::upd_right()
{
    if (right_fragment == nullptr or right_pos >= right_fragment->size()) 
    {
        right_fragment = static_cast<PGetNextNode*>(right)->GetNext();
        right_pos = 0;
    }
}

void PCrossProductNode::upd_left()
{
    if (left_fragment == nullptr or left_pos >= left_fragment->size()) 
    {
        left_fragment = static_cast<PGetNextNode*>(left)->GetNext();
        left_pos = 0;
    }
}

void PCrossProductNode::reset_right()
{
    static_cast<PGetNextNode*>(right)->Reset();
    right_fragment = nullptr;
    right_pos = 0;
}

std::vector<int>* PCrossProductNode::GetNext(){
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
                int le_num = left_fragment->at(left_pos);
                int rg_num = right_fragment->at(right_pos);

                int rg_sz = length_base_10(rg_num);

                int concatted_number = le_num;
                for(int k = 0; k < rg_sz; k++) {
                    concatted_number *= 10;
                }
                concatted_number += rg_num;
                data.push_back(concatted_number);
                if(data.size() * sizeof(int) >= MAX_BYTE_SIZE)
                {
                    right_pos++;
                    return &data;
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


ErrCode PCrossProductNode::GetRecord(){
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

Value* PCrossProductNode::GetValuePos(){
  if(vals == NULL){
    vals = new Value [GetAttrNum()];
    for(int i = 0; i < GetAttrNum(); i++) vals[i] = Value(0);
  }
  return vals;
}

int PCrossProductNode::GetAttrNum(){
  return 1;
}


void PCrossProductNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout<<" ";
  }
  std::cout<<"CrossProduct";
  std::cout<<" on "<<sorted<<" data\n";
  histogram.Print(indent);
  left->Print(indent + 2);
  right->Print(indent + 2);
}
