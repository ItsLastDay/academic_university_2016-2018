#ifndef PMERGEJOIN_H
#define PMERGEJOIN_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"



class PMergeJoin : public PGetNextNode{
  public:
    PMergeJoin(PGetNextNode* left, PGetNextNode* right, COLUMN_SORT cs);
    PMergeJoin(PGetNextNode* left, PGetNextNode* right);
    ~PMergeJoin();
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
    enum MERGEJOIN_AUTOMATA_STATE
    {
        AUTOM_INITIAL,
        AUTOM_EXHAUSTED,
        AUTOM_WORK_NORMAL,
        AUTOM_WRITE_PREV,
        AUTOM_NEED_ADVANCE_LEFT,
        AUTOM_NEED_ADVANCE_RIGHT,
    };

    int pos;

    std::vector<int>* left_fragment;
    std::vector<int>* right_fragment;
    size_t left_pos, right_pos;

    size_t prev_left_cnt;
    size_t cur_left_cnt;

    void UpdRight();
    void UpdLeft();
    void ResetAutomataState();
    MERGEJOIN_AUTOMATA_STATE CurState;
};

#endif // PMERGEJOIN_H
