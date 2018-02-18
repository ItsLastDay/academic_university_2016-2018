// A simple interface for a toy DBMS, v0.3
// George Chernishev (c) 2016, chernishev@gmail.com
// CHANGELOG:
// 0.3: added:
//      1) support for restricting phisycal join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

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
                     int offset1, int offset2, int memorylimit):LAbstractNode(left, right){
  this->offset1 = offset1;
  this->offset2 = offset2;
  this->memorylimit = memorylimit;
}

size_t LJoinNode::GetMemoryLimit() const
{
    return memorylimit;
}

LJoinNode::~LJoinNode(){
  delete left;
  delete rigth;
}

LCrossProductNode::LCrossProductNode(LAbstractNode *left, LAbstractNode *right): LAbstractNode(left, right)
{
}

LCrossProductNode::~LCrossProductNode()
{
    delete left;
    delete rigth;
}

LProjectNode::LProjectNode(LAbstractNode* child, int* offsets,
                           int nboffset):LAbstractNode(child, NULL){
  this->offsets = offsets;
  this->nboffset = nboffset;
}

LProjectNode::~LProjectNode(){
  delete[] offsets;
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

LUniqueNode::LUniqueNode(LAbstractNode* child):LAbstractNode(child, NULL){
}

LUniqueNode::~LUniqueNode(){
}


/* Physical nodes*/

PResultNode::PResultNode(PResultNode* left, PResultNode* right){
  this->left = left;
  this->right = right;
}

PResultNode::~PResultNode(){
  delete[] vals;
}

