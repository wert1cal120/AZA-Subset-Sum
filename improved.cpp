#include <vector>
#include <chrono>
#include "subset_sum.hpp"


NodeTree* subset_sum_improved(const std::vector<int>& array, const std::vector<int>& array_sums, NodeTree* root, NodeTree* parent, int target, int index, int value, int& recursion_calls, NodeTree* &found,  bool& time_out, std::chrono::steady_clock::time_point deadline) {

    recursion_calls++;

    if (time_out) {
        return root;
    }

    if (std::chrono::steady_clock::now() > deadline) {
        time_out = true;
        return root;
    }

    if (parent != nullptr) {
        root->parent = parent;
        root->sum = root->parent->sum + value;
        root->number = value;
        index++;
    }

    if (root->sum == target) {
        found = root;
        return root;
    }

    if (root->sum > target) {
        return root;
    }

    if (index < array.size() && root->sum + array_sums[index] >= target) {
        if (found == nullptr) {
            root->left = subset_sum_improved(array, array_sums, new NodeTree(), root, target, index, 0, recursion_calls, found, time_out, deadline);
        }

        if (found == nullptr) {
            root->right = subset_sum_improved(array, array_sums, new NodeTree(), root, target, index, array[index], recursion_calls, found, time_out, deadline);
        }
    }

    return root;
}