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

#ifndef PGETNEXTNODE_H
#define PGETNEXTNODE_H
#include <vector>

#include "../interface/interface.h"
#include "histogram.h"

enum COLUMN_SORT{
    CS_ASCENDING,
    CS_DESCENDING,
    CS_NO,
    CS_UNKNOWN
};

COLUMN_SORT GetCombinedStatus(COLUMN_SORT left_status, COLUMN_SORT right_status);

class PGetNextNode : public PResultNode{
  public:
    PGetNextNode();
    PGetNextNode(PResultNode* left, PResultNode* right);
    // internal way to transfer data
    virtual std::vector<int>* GetNext();
    // getting access to data
    virtual void Initialize();
    // get number of attributes, since its a toy "columnar" DB its always 1
    virtual int GetAttrNum();
    virtual void Reset();
    virtual COLUMN_SORT GetSortednessStatus();
    virtual void MakeHistogram(size_t sample_size=0);
    size_t QueryHistogram(Predicate* pred=nullptr) const;
  protected:
    Histogram histogram;
    std::vector<int> data;  
    const size_t MAX_BYTE_SIZE = 100;
    // sorted status
    COLUMN_SORT sorted;
};

#endif // PGETNEXTNODE_H
