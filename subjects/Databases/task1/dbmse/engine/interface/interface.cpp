// A simple interface for a toy DBMS, v0.2
// George Chernishev (c) 2016, chernishev@gmail.com

#include <string.h>
#include "interface.h"

LAbstractNode::LAbstractNode(LAbstractNode* left, LAbstractNode* right){
  this->left = left;
  this->rigth = right;
}

LAbstractNode::~LAbstractNode(){
}

LAbstractNode* LAbstractNode::GetLeft(){
  return left;
}

LAbstractNode* LAbstractNode::GetRight(){
  return rigth;
}

PGetNextNode* LSelectNode::accept(BaseQueryFactory &vis)
{
    return vis.dispatch(*this);
}
PGetNextNode* LCrossProductNode::accept(BaseQueryFactory &vis)
{
    return vis.dispatch(*this);
}

PGetNextNode* LJoinNode::accept(BaseQueryFactory &vis)
{
    return vis.dispatch(*this);
}

LJoinNode::LJoinNode(LAbstractNode* left, LAbstractNode* right,
                     int offset1, int offset2):LAbstractNode(left, right){
  this->offset1 = offset1;
  this->offset2 = offset2;
}

LJoinNode::~LJoinNode(){
  delete left;
  delete rigth;
}

LSelectNode::LSelectNode(BaseTable* table,
                         std::vector<Predicate>* predicates): LAbstractNode(NULL, NULL){
  this->table = table;
  this->predicates = predicates;
  iteratorpos = 0;
}

BaseTable* LSelectNode::GetBaseTable(){
  return new BaseTable(table);
}

LCrossProductNode::LCrossProductNode(LAbstractNode *left, LAbstractNode *right): LAbstractNode(left, right)
{
}

LCrossProductNode::~LCrossProductNode()
{
    delete left;
    delete rigth;
}

int LSelectNode::GetNextPredicate(Predicate** p){
  if(predicates == NULL || iteratorpos > predicates->size()){
      *p = NULL;
      return 1;
  }
  *p = new Predicate((*predicates)[iteratorpos]);
  iteratorpos++;
  return 0;
}

void LSelectNode::ResetIterator(){
  iteratorpos = 0;
}


LSelectNode::~LSelectNode(){
  delete predicates;
  delete table;
}

/* Physical nodes*/

PResultNode::PResultNode(PResultNode* left, PResultNode* right){
  this->left = left;
  this->right = right;
}

PResultNode::~PResultNode(){
  delete[] vals;
}



