// A simple interface for a toy DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com


#ifndef INTERFACE_H
#define INTERFACE_H
#include <string.h>
#include <vector>
#include <string>
#include <iostream>

class PGetNextNode;

class LJoinNode;
class LSelectNode;
class LCrossProductNode;

class BaseQueryFactory
{
public:
    virtual PGetNextNode* dispatch(LJoinNode&) = 0;
    virtual PGetNextNode* dispatch(LSelectNode&) = 0;
    virtual PGetNextNode* dispatch(LCrossProductNode&) = 0;
};

enum ValueType{
  VT_INT,
  VT_STRING
};

struct Value{
  ValueType vtype;
  int vint;
  char* vstr;
  Value& operator=(const Value& other){
    if(&other == this){
      return *this;
    }
    vtype = other.vtype;
    vint = other.vint;
    delete[] vstr;
    vstr = new char[strlen(other.vstr)+1];
    memcpy(vstr, other.vstr, strlen(other.vstr)+1);
    return *this;
  }
  Value(int v){
    vtype = VT_INT;
    vint = v;
    vstr = new char[1];
    vstr[0] = '\0';
  }
  Value(char* v){
    vtype = VT_STRING;
    vint = 0;
    vstr = v;
  }
  Value(){
    vtype = VT_INT;
    vint = 0;
    vstr = new char[1];
    vstr[0] = '\0';
  }
  ~Value(){
    delete[] vstr;
  }
};

enum PredicateType{
  PT_EQUALS,
  PT_GREATERTHAN,
};

struct Predicate{
  PredicateType ptype;
  ValueType vtype;
  int attribute;
  int vint;
  char* vstr;
  Predicate& operator=(const Predicate& other){
    if(&other == this){
      return *this;
    }
    ptype = other.ptype;
    vtype = other.vtype;
    attribute = other.attribute;
    vint = other.vint;
    delete[] vstr;
    vstr = new char[strlen(other.vstr)+1];
    memcpy(vstr, other.vstr, strlen(other.vstr)+1);
    return *this;
  }

  Predicate(const Predicate& p){
    ptype = p.ptype;
    vtype = p.vtype;
    attribute = p.attribute;
    vint = p.vint;
    vstr = new char[strlen(p.vstr)+1];
    memcpy(vstr, p.vstr, strlen(p.vstr)+1);
  }

  Predicate(PredicateType ptype, ValueType vtype, int attribute, int vint, char* vstr){
    this->ptype = ptype;
    this->vtype = vtype;
    this->attribute = attribute;
    this->vint = vint;
    this->vstr = vstr;
  }

  ~Predicate(){delete[] vstr;}
};

struct BaseTable{
  std::string relpath;
  ValueType* vtypes;
  int nbAttr;
  BaseTable():relpath(""), vtypes(NULL), nbAttr(0){}
  BaseTable(const BaseTable& t): relpath(t.relpath), nbAttr(t.nbAttr){
    vtypes = new ValueType[t.nbAttr];
    memcpy(vtypes, t.vtypes, sizeof(int)*nbAttr);
  }
  BaseTable(BaseTable* t): relpath(t->relpath), nbAttr(t->nbAttr){
    vtypes = new ValueType[t->nbAttr];
    memcpy(vtypes, t->vtypes, sizeof(int)*nbAttr);
  }
  ~BaseTable(){delete[] vtypes;}
};

enum ErrCode{
  EC_OK,
  EC_FINISH,
  EC_ERROR
};

/* Logical nodes (query) */

class LAbstractNode{
  public:
    LAbstractNode(LAbstractNode* left, LAbstractNode* right);
    virtual ~LAbstractNode();
    LAbstractNode* GetLeft();
    LAbstractNode* GetRight();
    virtual PGetNextNode* accept(BaseQueryFactory &vis) = 0;
  protected:
    LAbstractNode* left;
    LAbstractNode* rigth;
};

class LCrossProductNode : public LAbstractNode{
  public:
    LCrossProductNode(LAbstractNode* left, LAbstractNode* right);
    ~LCrossProductNode();
    PGetNextNode* accept(BaseQueryFactory &vis);
};

class LJoinNode : public LAbstractNode{
  public:
    LJoinNode(LAbstractNode* left, LAbstractNode* right, int offset1, int offset2);
    ~LJoinNode();
    PGetNextNode* accept(BaseQueryFactory &vis);
  private:
    int offset1, offset2;
};

class LProjectNode : public LAbstractNode{
  public:
    // offsets to keep
    LProjectNode(LAbstractNode* child, int* offsets, int nboffset);
    ~LProjectNode();
};

class LSelectNode : public LAbstractNode{
  public:
    PGetNextNode* accept(BaseQueryFactory &vis);
    LSelectNode(BaseTable* table, std::vector<Predicate>* predicates);
    // gets new instance of BaseTable, its caller's goal to free
    BaseTable* GetBaseTable();
    // returns next predicate and end status
    int GetNextPredicate(Predicate**);
    // resets predicate iterator
    void ResetIterator();
    ~LSelectNode();
  private:
    int iteratorpos;
    BaseTable* table;
    std::vector<Predicate>* predicates;
};

/* Physical node interface (result) */

class PResultNode{
  public:
    PResultNode(PResultNode* left, PResultNode* right);
    virtual ~PResultNode();
    // returns error status and writes data to pre-specified area (vals)
    virtual ErrCode GetRecord() = 0;
    // returns pointer of pre-allocated memory area, where results of GetRecord would be stored
    virtual Value* GetValuePos() = 0;
    // returns number of attributes
    virtual int GetAttrNum() = 0;
  protected:
    PResultNode* left;
    PResultNode* right;
    Value* vals;
};


#endif // INTERFACE_H
