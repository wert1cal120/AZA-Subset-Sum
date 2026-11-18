#ifndef AZA_SUBSET_SUM_SUBSET_SUM_HPP
#define AZA_SUBSET_SUM_SUBSET_SUM_HPP

#include <vector>

using namespace std::chrono;

struct NodeTree {
    NodeTree* parent = nullptr;
    NodeTree* left = nullptr;
    NodeTree* right = nullptr;
    int number = 0;
    int sum = 0;
};

NodeTree* subset_sum_baseline(const std::vector<int>& array, NodeTree* root, NodeTree* parent, int target, int index, int value, int& recursion_calls, NodeTree* &found,  bool& time_out, steady_clock::time_point deadline);
NodeTree* subset_sum_improved(const std::vector<int>& array, const std::vector<int>& array_sums, NodeTree* root, NodeTree* parent, int target, int index, int value, int& recursion_calls, NodeTree* &found,  bool& time_out, steady_clock::time_point deadline);

#endif //AZA_SUBSET_SUM_SUBSET_SUM_HPP


