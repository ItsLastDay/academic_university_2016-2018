#include "histogram.h"
#include "pgetnextnode.h"
#include "../interface/interface.h"
#include <vector>
#include <algorithm>
#include <iostream>

Histogram::Histogram()
    : num_steps(-1)
    , number_of_elems(0)
    , is_combined_histogram(false)
{
}


Histogram::Histogram(std::vector<int> &all_values, size_t total_size)
    : num_steps(0)
    , number_of_elems(total_size)
    , is_combined_histogram(false)
{
    std::sort(all_values.begin(), all_values.end());

    number_of_elems = total_size;

    int desired_num_steps = 10;
    size_t num_in_each_step = (all_values.size() + desired_num_steps - 1) / desired_num_steps;
    for (size_t i = 0; i + 1 < all_values.size(); i += num_in_each_step)
    {
        steps.push_back(all_values[i]);
    }

    if (all_values.size() > 1)
    {
        steps.push_back(all_values.back());
    }

    num_steps = steps.size() - 1;
}


Histogram::Histogram(Histogram const& le, Histogram const& rg)
    : num_steps(0)
    , number_of_elems(0)
    , is_combined_histogram(true)
{
    // Only allow estimating the total number of elements from this histogram.
    if (le.is_combined_histogram or rg.is_combined_histogram)
    {
        number_of_elems = le.number_of_elems * rg.number_of_elems / 2;
        return;
    }

    for (auto const &le_step: le.steps)
    {
        for (int i = 0; i < rg.num_steps; ++i)
        {
            if (rg.steps[i] <= le_step and le_step <= rg.steps[i + 1])
            {
                number_of_elems += rg.number_of_elems / rg.num_steps;
            }
        }
    }

    for (auto const &rg_step: rg.steps)
    {
        for (int i = 0; i < le.num_steps; ++i)
        {
            if (le.steps[i] <= rg_step and rg_step <= le.steps[i + 1])
            {
                number_of_elems += le.number_of_elems / le.num_steps;
            }
        }
    }
}


void Histogram::Print(int indent) const
{
    using std::cout;
    using std::endl;
    for (int i = 0; i < indent; i++){
      std::cout<<" ";
    }
    cout << "Histogram for relation with " << number_of_elems << " elements." << endl; 
    for (int i = 0; i < indent; i++){
      std::cout<<" ";
    }
    cout << num_steps << " distribution steps: ";
    for (auto const& step: steps)
    {
        cout << step << " ";
    }
    cout << endl;
}


size_t Histogram::EstimateSelectivity(Predicate *pred=nullptr) const
{
    if (num_steps == -1)
    {
        throw std::runtime_error("Using an unitialized Histogram");
    }

    if (pred == nullptr)
    {
        return number_of_elems;
    }

    if (is_combined_histogram)
    {
        throw std::runtime_error("Using a combined Histogram with non-null predicate");
    }

    if (pred->ptype == PT_EQUALS)
    {
        int count_eq = 0;
        for (int i = 0; i < num_steps; i++)
        {
            count_eq += steps[i + 1] >= pred->vint and pred->vint >= steps[i];
        }
        return number_of_elems * count_eq / num_steps;
    }
    else if (pred->ptype == PT_GREATERTHAN)
    {
        for (int i = 0; i < num_steps + 1; i++)
        {
            if (steps[i] > pred->vint)
            {
                return number_of_elems * (num_steps + 1 - i) / num_steps;
            } 
        }

        return 0;
    }
}
