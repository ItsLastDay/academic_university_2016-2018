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

#include "pgetnextnode.h"
#include <cassert>

COLUMN_SORT GetCombinedStatus(COLUMN_SORT left_status, COLUMN_SORT right_status)
{
    assert(left_status != CS_UNKNOWN);
    assert(right_status != CS_UNKNOWN);

    if (left_status == CS_NO or right_status == CS_NO)
    {
        return CS_NO;
    }

    if (left_status == right_status)
    {
        return left_status;
    }

    return CS_NO;
}

COLUMN_SORT PGetNextNode::GetSortednessStatus()
{
    assert(sorted != CS_UNKNOWN);
    return sorted;
}


void PGetNextNode::MakeHistogram(size_t sample_size)
{
    if (right == nullptr)
    {
        histogram = static_cast<PGetNextNode*>(left)->histogram;
    }
    else
    {
        Histogram hist_le = static_cast<PGetNextNode*>(left)->histogram;
        Histogram hist_rg = static_cast<PGetNextNode*>(right)->histogram;
        histogram = Histogram(hist_le, hist_rg);
    }
}


size_t PGetNextNode::QueryHistogram(Predicate* pred) const
{
    return histogram.EstimateSelectivity(pred);
}


PGetNextNode::PGetNextNode(): PResultNode(NULL, NULL){
  Initialize();
}

PGetNextNode::PGetNextNode(PResultNode* left, PResultNode* right): PResultNode(left, right){
  Initialize();
  MakeHistogram();
}

void PGetNextNode::Initialize(){
  return;
}

void PGetNextNode::Reset(){
    return;
}

std::vector<int>* PGetNextNode::GetNext(){
  return NULL;
}

int PGetNextNode::GetAttrNum(){
  return 1;
}

