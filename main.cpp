#include <iostream>
#include <vector>

struct NodeTree {
    NodeTree* parent = nullptr;
    NodeTree* left = nullptr;
    NodeTree* right = nullptr;
    int number = 0;
    int sum = 0;
};

static NodeTree* subset_sum_simple(const std::vector<int>& array, NodeTree* root, NodeTree* parent, int target, int index, int value, int& recursion_calls, NodeTree* &found) {

    recursion_calls++;

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
        if (found == nullptr) {
            root->left = subset_sum_simple(array, new NodeTree(), root, target, index, 0, recursion_calls, found);
        }

        if (found == nullptr) {
            root->right = subset_sum_simple(array, new NodeTree(), root, target, index, array[index], recursion_calls, found);
        }
    }

    return root;
}

static void print_result(const NodeTree* result) {
    if (result != nullptr) {
        std::cout << "[";
        while (result->parent != nullptr) {
            std::cout << result->number;

            result = result->parent;

            if (result->parent != nullptr) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    } else {
        std::cout << "False" << std::endl;
    }
}

int main() {
    int target = 30;
    std::vector<int> array{3, 7, 11, 15, 19};

    NodeTree *root = new NodeTree();
    NodeTree* found = nullptr;
    int recursion_calls = 0;

    subset_sum_simple(array, root, nullptr, target, 0, 0, recursion_calls, found);
    print_result(found);
    std::cout << recursion_calls << std::endl;

    return 0;
}