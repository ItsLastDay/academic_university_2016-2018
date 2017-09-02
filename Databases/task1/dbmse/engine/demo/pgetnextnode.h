// A toy one-columnar DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#ifndef PGETNEXTNODE_H
#define PGETNEXTNODE_H
#include <vector>
#include "../interface/interface.h"

class PGetNextNode : public PResultNode{
  public:
    PGetNextNode();
    // internal way to transfer data
    virtual std::vector<int>* GetNext();
    virtual void Initialize();
    virtual void Reset();
  protected:
    std::vector<int> data;
    const size_t MAX_BYTE_SIZE = 5;
};

#endif // PGETNEXTNODE_H
