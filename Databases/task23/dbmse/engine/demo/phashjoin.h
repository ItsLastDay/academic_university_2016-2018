#ifndef PHASHJOIN_H
#define PHASHJOIN_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"
#include <unordered_map>



class PHashJoin : public PGetNextNode{
  public:
    PHashJoin(PGetNextNode* left, PGetNextNode* right, COLUMN_SORT cs);
    PHashJoin(PGetNextNode* left, PGetNextNode* right);
    ~PHashJoin();
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
    enum HASHJOIN_AUTOMATA_STATE
    {
        AUTOM_INITIAL,
        AUTOM_EXHAUSTED,
        AUTOM_HASHING,
        AUTOM_PROBING,
        AUTOM_PROBING_PREV,
    };

    size_t pos;

    std::vector<int>* left_fragment;
    std::vector<int>* right_fragment;
    size_t left_pos, right_pos;
    size_t CurCount;
    int CurVal;

    std::unordered_map<int, size_t> count_elems;

    void UpdRight();
    void UpdLeft();
    void ResetAutomataState();
    void AdvanceRight();
    HASHJOIN_AUTOMATA_STATE CurState;
};

#endif // PHASHJOIN_H
