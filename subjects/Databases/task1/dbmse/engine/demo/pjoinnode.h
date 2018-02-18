// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#ifndef PJOINNODE_H
#define PJOINNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PJoinNode : public PGetNextNode{
  public:
    PJoinNode(PGetNextNode* left, PGetNextNode* right);
    ~PJoinNode();
    virtual std::vector<int>* GetNext();
    virtual void Initialize();
    virtual void Reset();
    // returns error status and writes data to pre-specified area
    virtual ErrCode GetRecord();
    // returns pointer of pre-allocated memory area, where results of GetRecord would be stored
    virtual Value* GetValuePos();
    // returns number of attributes
    virtual int GetAttrNum();
  private:
    int pos;
    std::vector<int>* left_fragment;
    std::vector<int>* right_fragment;
    size_t left_pos, right_pos;

    void upd_right();
    void upd_left();
    void reset_right();
};

#endif // PJOINNODE_H
