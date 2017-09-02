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

#ifndef PJOINNODE_H
#define PJOINNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PJoinNode : public PGetNextNode{
  public:
    PJoinNode(PGetNextNode* left, PGetNextNode* right, COLUMN_SORT cs);
    PJoinNode(PGetNextNode* left, PGetNextNode* right);
    ~PJoinNode();
    virtual std::vector<int>* GetNext();
    virtual void Initialize();
    virtual void Reset();
    // returns error status and writes data to pre-specified area
    virtual ErrCode GetRecord();
    // returns pointer of pre-allocated memory area, where results of GetRecord would be stored
    virtual Value* GetValuePos();
    // print node
    virtual void Print(int indent);
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
