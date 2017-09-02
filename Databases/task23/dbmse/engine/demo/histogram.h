#pragma once
#include "../interface/interface.h"
#include <vector>

// Implementation based on page 7 from
// "Accurate estimation of the number of tuples satisfying a condition"
// G Piatetsky-Shapiro, C Connell - ACM SIGMOD Record, 1984
struct Histogram
{
    Histogram();
    Histogram(std::vector<int>& all_values, size_t total_size);
    Histogram(Histogram const& le, Histogram const& rg);

    void Print(int indent) const;
    
    size_t EstimateSelectivity(Predicate *pred) const;
private:
    int num_steps;
    size_t number_of_elems;
    std::vector<int> steps;
    bool is_combined_histogram;
};
