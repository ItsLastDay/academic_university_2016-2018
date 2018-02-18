#include "pmergejoin.h"
#include <cassert>
#include <iostream>
using namespace std;

PMergeJoin::PMergeJoin(PGetNextNode* left, PGetNextNode* right,
                     COLUMN_SORT cs): PGetNextNode(left, right){

  this->left = left;
  this->right = right;
  pos = 0;
  vals = NULL;
  sorted = cs;
  Initialize();
}

PMergeJoin::PMergeJoin(PGetNextNode* left, PGetNextNode* right): PMergeJoin(left, right, CS_UNKNOWN){
}


PMergeJoin::~PMergeJoin(){
  delete left;
  delete right;
}

void PMergeJoin::ResetAutomataState()
{
    CurState = AUTOM_INITIAL;
    left_pos = 0;
    right_pos = 0;
    prev_left_cnt = 0;
    cur_left_cnt = 0;
    left_fragment = right_fragment = nullptr;
    data.clear();
    pos = 0;
}

void PMergeJoin::Reset(){
    static_cast<PGetNextNode*>(left)->Reset();
    static_cast<PGetNextNode*>(right)->Reset();
    ResetAutomataState();
}

void PMergeJoin::UpdRight()
{
    if (right_fragment == nullptr or right_pos >= right_fragment->size()) 
    {
        right_fragment = static_cast<PGetNextNode*>(right)->GetNext();
        right_pos = 0;
    }
}

void PMergeJoin::UpdLeft()
{
    if (left_fragment == nullptr or left_pos >= left_fragment->size()) 
    {
        left_fragment = static_cast<PGetNextNode*>(left)->GetNext();
        left_pos = 0;
    }
}

std::vector<int>* PMergeJoin::GetNext(){
    data.clear();

    while (CurState != AUTOM_EXHAUSTED)
    {
        switch (CurState)
        {
            case AUTOM_INITIAL:
            {
                UpdLeft();
                UpdRight();
                CurState = AUTOM_WORK_NORMAL;
                if (left_fragment == nullptr or right_fragment == nullptr)
                {
                    CurState = AUTOM_EXHAUSTED;
                }
                break;
            }

            case AUTOM_WORK_NORMAL:
            {
                int left_val = left_fragment->at(left_pos);
                int right_val = right_fragment->at(right_pos);

                CurState = AUTOM_NEED_ADVANCE_RIGHT;
                if ((sorted == CS_ASCENDING and right_val > left_val)
                        or (sorted == CS_DESCENDING and right_val < left_val))
                {
                    CurState = AUTOM_NEED_ADVANCE_LEFT;
                }

                if (left_val == right_val)
                {
                    cur_left_cnt++;
                    data.push_back(left_val);
                    if (data.size() * sizeof(int) >= MAX_BYTE_SIZE)
                    {
                        return &data;
                    }
                }

                break;
            }

            case AUTOM_WRITE_PREV:
            {
                for (; cur_left_cnt < prev_left_cnt; )
                {
                    cur_left_cnt++;
                    data.push_back(left_fragment->at(left_pos));
                    if (data.size() * sizeof(int) >= MAX_BYTE_SIZE)
                    {
                        return &data;
                    }
                }

                CurState = AUTOM_NEED_ADVANCE_LEFT;

                break;
            }

            case AUTOM_NEED_ADVANCE_LEFT:
            {
                prev_left_cnt = cur_left_cnt;
                int prev_left_val = left_fragment->at(left_pos);
                cur_left_cnt = 0;

                left_pos++;
                UpdLeft();
                CurState = AUTOM_WORK_NORMAL;
                if (left_fragment == nullptr)
                {
                    CurState = AUTOM_EXHAUSTED;
                } else if (left_fragment->at(left_pos) == prev_left_val)
                {
                    CurState = AUTOM_WRITE_PREV;
                } else if (right_fragment == nullptr)
                {
                    CurState = AUTOM_EXHAUSTED;
                }

                break;
            }    

            case AUTOM_NEED_ADVANCE_RIGHT:
            {
                right_pos++;
                UpdRight();
                CurState = AUTOM_WORK_NORMAL;
                if (right_fragment == nullptr)
                {
                    // There may be some equal elements in the left.
                    CurState = AUTOM_NEED_ADVANCE_LEFT;
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

void PMergeJoin::Initialize(){
    ResetAutomataState();
    // Both left and right relation must be sorted in the same way.
    sorted = static_cast<PGetNextNode*>(left)->GetSortednessStatus();
    assert(sorted != CS_NO);
    assert(sorted == static_cast<PGetNextNode*>(right)->GetSortednessStatus());
}

ErrCode PMergeJoin::GetRecord(){
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

Value* PMergeJoin::GetValuePos(){
  if(vals == NULL){
    vals = new Value [GetAttrNum()];
    for(int i = 0; i < GetAttrNum(); i++) vals[i] = Value(0);
  }
  return vals;
}

void PMergeJoin::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout<<" ";
  }
  std::cout<<"MERGE JOIN";
  std::cout<<" on "<<sorted<<" data\n";
  histogram.Print(indent);
  left->Print(indent + 2);
  right->Print(indent + 2);
}
