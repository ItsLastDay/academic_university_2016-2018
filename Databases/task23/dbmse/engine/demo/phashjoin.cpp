#include "phashjoin.h"
#include <cassert>
#include <iostream>
using namespace std;

PHashJoin::PHashJoin(PGetNextNode* left, PGetNextNode* right,
                     COLUMN_SORT cs): PGetNextNode(left, right){

  this->left = left;
  this->right = right;
  pos = 0;
  vals = NULL;
  sorted = cs;
  Initialize();
}

PHashJoin::PHashJoin(PGetNextNode* left, PGetNextNode* right): PHashJoin(left, right, CS_UNKNOWN){
}


PHashJoin::~PHashJoin(){
  delete left;
  delete right;
}

void PHashJoin::ResetAutomataState()
{
    CurState = AUTOM_INITIAL;
    left_pos = 0;
    right_pos = 0;
    left_fragment = right_fragment = nullptr;
    count_elems.clear();
    data.clear();
    pos = 0;
}

void PHashJoin::Reset(){
    static_cast<PGetNextNode*>(left)->Reset();
    static_cast<PGetNextNode*>(right)->Reset();
    ResetAutomataState();
}

void PHashJoin::UpdRight()
{
    if (right_fragment == nullptr or right_pos >= right_fragment->size()) 
    {
        right_fragment = static_cast<PGetNextNode*>(right)->GetNext();
        right_pos = 0;
    }
}

void PHashJoin::UpdLeft()
{
    if (left_fragment == nullptr or left_pos >= left_fragment->size()) 
    {
        left_fragment = static_cast<PGetNextNode*>(left)->GetNext();
        left_pos = 0;
    }
}

void PHashJoin::AdvanceRight()
{
    right_pos++;
    UpdRight();
    if (right_fragment == nullptr)
    {
        CurState = AUTOM_EXHAUSTED;
    }
}

std::vector<int>* PHashJoin::GetNext(){
    data.clear();

    while (CurState != AUTOM_EXHAUSTED)
    {
        switch (CurState)
        {
            case AUTOM_INITIAL:
            {
                UpdLeft();
                UpdRight();
                CurState = AUTOM_HASHING;
                if (left_fragment == nullptr or right_fragment == nullptr)
                {
                    CurState = AUTOM_EXHAUSTED;
                }
                break;
            }

            case AUTOM_HASHING:
            {
                while (left_fragment != nullptr)
                {
                    count_elems[left_fragment->at(left_pos)]++;
                    left_pos++;
                    UpdLeft();
                }

                CurState = AUTOM_PROBING;
                break;
            }

            case AUTOM_PROBING:
            {
                CurVal = right_fragment->at(right_pos);

                if (count_elems[CurVal] > 0)
                {
                    CurCount = 0;
                    CurState = AUTOM_PROBING_PREV;
                } 
                else
                {
                    AdvanceRight();
                }

                break;
            }

            case AUTOM_PROBING_PREV:
            {
                if (CurCount < count_elems[CurVal])
                {
                    CurCount++;
                    data.push_back(CurVal);
                    if (data.size() * sizeof(int) >= MAX_BYTE_SIZE)
                    {
                        return &data;
                    }
                } 
                else
                {
                    CurState = AUTOM_PROBING;
                    AdvanceRight();
                }

                break;
            }

            default:
                assert(false);
                break;
        }
    }

    if (data.empty())
    {
        return nullptr;
    }
    return &data;
}

void PHashJoin::Initialize(){
    ResetAutomataState();
    // We hash the left side, and emit from the right side -> order
    // preserves from the right.
    sorted = static_cast<PGetNextNode*>(right)->GetSortednessStatus();
}

ErrCode PHashJoin::GetRecord(){
  if (pos == data.size()) {
      auto nx = GetNext();
      if(nx == nullptr)
          return EC_FINISH;
      pos = 0;
  }
  for(int i = 0; i < GetAttrNum(); i++){
    vals[i] = Value(data[pos]);
  }
  pos++;
  return EC_OK;
}

Value* PHashJoin::GetValuePos(){
  if(vals == NULL){
    vals = new Value [GetAttrNum()];
    for(int i = 0; i < GetAttrNum(); i++) vals[i] = Value(0);
  }
  return vals;
}

void PHashJoin::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout<<" ";
  }
  std::cout<<"HASH JOIN";
  std::cout<<" on "<<sorted<<" data\n";
  histogram.Print(indent);
  left->Print(indent + 2);
  right->Print(indent + 2);
}
