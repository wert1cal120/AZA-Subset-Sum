#include <vector>
#include <chrono>
#include "subset_sum.hpp"


NodeTree* subset_sum_baseline(const std::vector<int>& array, NodeTree* root, NodeTree* parent, int target, int index, int value, int& recursion_calls, NodeTree* &found,  bool& time_out, std::chrono::steady_clock::time_point deadline) {

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

    if (index < array.size()) {
        if (found == nullptr && !time_out) {
            root->left = subset_sum_baseline(array, new NodeTree(), root, target, index, 0, recursion_calls, found, time_out, deadline);
        }

        if (found == nullptr && !time_out) {
            root->right = subset_sum_baseline(array, new NodeTree(), root, target, index, array[index], recursion_calls, found, time_out, deadline);
        }
    }

    return root;
}

