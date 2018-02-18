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

#ifndef PSELECTNODE_H
#define PSELECTNODE_H

#include <vector>
#include "../interface/interface.h"
#include "pgetnextnode.h"
#include <fstream>

class PSelectNode : public PGetNextNode{
  public:
    PSelectNode();
    PSelectNode(BaseTable* table, Predicate* predicate);
    ~PSelectNode();
    virtual std::vector<int>* GetNext();
    virtual void Initialize();
    virtual void Reset();
    // returns error status and writes data to pre-specified area
    virtual ErrCode GetRecord();
    // returns pointer of pre-allocated memory area, where results of GetRecord would be stored
    virtual Value* GetValuePos();
    // print node
    virtual void Print(int indent);

    void MakeHistogram(size_t sample_size=1064) override;
  private:
    BaseTable* table;
    Predicate* predicate;
    int pos;
    std::ifstream input_file;
};

#endif // PSELECTNODE_H
