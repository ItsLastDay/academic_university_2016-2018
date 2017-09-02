// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

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
    // returns number of attributes
    virtual int GetAttrNum();
  protected:
//    const size_t MAX_BYTE_SIZE = 1;
  private:
    BaseTable* table;
    Predicate* predicate;
    int pos;
    std::ifstream input_file;
};

#endif // PSELECTNODE_H
