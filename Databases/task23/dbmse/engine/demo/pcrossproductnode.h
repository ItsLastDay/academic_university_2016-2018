#ifndef __P_CROSSPRODUCTNODE_H__
#define __P_CROSSPRODUCTNODE_H__

#include "pgetnextnode.h"
#include <vector>

class PCrossProductNode: public PGetNextNode 
{
public:
    PCrossProductNode(PGetNextNode *, PGetNextNode *);
    ~PCrossProductNode();
    virtual std::vector<int>* GetNext();
    virtual void Initialize();
    virtual void Reset();
    // returns error status and writes data to pre-specified area
    virtual ErrCode GetRecord();
    // returns pointer of pre-allocated memory area, where results of GetRecord would be stored
    virtual Value* GetValuePos();
    // returns number of attributes
    virtual int GetAttrNum();
    virtual void Print(int indent);
protected:
//    const size_t MAX_BYTE_SIZE = 50;
private:
    int pos;
    std::vector<int>* left_fragment;
    std::vector<int>* right_fragment;
    size_t left_pos, right_pos;

    void upd_right();
    void upd_left();
    void reset_right();
};

#endif
