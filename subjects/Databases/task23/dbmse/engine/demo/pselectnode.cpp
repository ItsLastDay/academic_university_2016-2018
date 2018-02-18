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

#include <iostream>
#include <fstream>

#include <string>
#include <cassert>
#include <random>

#include "pselectnode.h"

PSelectNode::PSelectNode(){}

PSelectNode::PSelectNode(BaseTable* table, Predicate* predicate): PGetNextNode(){
  this->table = table;
  this->predicate = predicate;
  pos = 0;
  vals = NULL;
  left = NULL;
  right = NULL;
  data.clear();
  sorted = CS_UNKNOWN;
  Initialize();
  MakeHistogram();
}

PSelectNode::~PSelectNode(){
    input_file.close();
  delete predicate;
  delete table;
}


void PSelectNode::MakeHistogram(size_t sample_size)
{
    // Sampling is done via "reservoir sampling" algorithm
    // https://en.wikipedia.org/wiki/Reservoir_sampling.
    std::vector<int> sample_values;
    size_t all_size = 0;

    std::mt19937 generator(12345);
    assert(sample_size != 0);

    std::vector<int>* cur_values = GetNext();
    while (cur_values != nullptr)
    {
        for (auto const& val: *cur_values)
        {
            ++all_size;
            if (sample_values.size() < sample_size)
            {
                sample_values.push_back(val);
            }
            else
            {
                size_t idx = generator() % (all_size + 1);
                if (idx < sample_size)
                {
                    sample_values[idx] = val;
                }
            }
        }

        cur_values = GetNext();
    }

    //std::cout << "Sampled " << sample_values.size() << " values from select." << std::endl;
    histogram = Histogram(sample_values, all_size);
    Reset();
}


void PSelectNode::Initialize(){
  std::string line;
  input_file.open(table->relpath);
  if(input_file.is_open()){
    // skipping first 2 lines
    getline(input_file, line);
    getline(input_file, line);
    // and reading sort order of the row
    getline(input_file, line);
    if(line.compare("ASCENDING") == 0)
      sorted = CS_ASCENDING;
    else if(line.compare("DESCENDING") == 0)
      sorted = CS_DESCENDING;
    else if(line.compare("NO") == 0)
      sorted = CS_NO;
    else sorted = CS_UNKNOWN;
  }
}

void PSelectNode::Reset(){
    pos = 0;
    input_file.clear();
    input_file.seekg(0, input_file.beg);
    std::string line;
    getline(input_file, line);
    getline(input_file, line);
    getline(input_file, line);
    data.clear();
}

std::vector<int>* PSelectNode::GetNext(){
    data.clear();

    if(input_file.eof())
        return nullptr;

    int val;
    std::string line;
    while(getline(input_file, line)){
      // we assume only 1 int predicate
      val = std::stoi(line);
      bool has_pred = false;
      if(predicate != NULL && predicate->vtype == VT_INT){
        if (predicate->ptype == PT_EQUALS){
          if (predicate->vint == val) data.push_back(val);
        }else
          if (predicate->vint < val) data.push_back(val);
        has_pred = true;
      }
      if(not has_pred)
          data.push_back(val);

      if(data.size() * sizeof(int) >= MAX_BYTE_SIZE)
          return &data;
    }

    if(not data.empty())
        return &data;
    return nullptr;
}

ErrCode PSelectNode::GetRecord(){
  if (pos == data.size()) {
      auto nx = GetNext();
      if(nx == nullptr)
          return EC_FINISH;
      pos = 0;
      // Can happen e.g. when Select's predicate filtered remaining output 
      if(data.empty()) 
          return GetRecord();
  }
  for(int i = 0; i < GetAttrNum(); i++){
    vals[i] = Value(data[pos]);
  }
  pos++;
  return EC_OK;
}

Value* PSelectNode::GetValuePos(){
  if(vals == NULL){
    vals = new Value [GetAttrNum()];
    for(int i = 0; i < GetAttrNum(); i++) vals[i] = Value(0);
  }
  return vals;
}

void PSelectNode::Print(int indent){
  for (int i = 0; i < indent; i++){
    std::cout<<" ";
  }
  std::cout<<"SCAN "<<*table<<" with predicate ";
  if(predicate != NULL)
    std::cout<<*predicate;
  else
    std::cout<<" NULL";
  std::cout<<" on "<<sorted<<" data\n";
  histogram.Print(indent);
  if(left != NULL) left->Print(indent + 2);
  if(right != NULL) right->Print(indent + 2);
}

