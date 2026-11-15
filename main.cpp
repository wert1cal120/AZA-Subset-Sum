#include <iostream>
#include <numeric>
#include <vector>

struct SearchResult {
    std::vector<std::vector<int>> subsets;
    std::optional<std::vector<int>> found;
    int recursion_depth;
};

static SearchResult subset_sum_simple(const std::vector<int>& array, int target, int& recursion_depth) {

    ++recursion_depth;

    if (array.empty()) {
        return {{}, std::nullopt, recursion_depth};
    }

    if (array[0] == target) {
        return {{}, std::vector<int>({array[0]}), recursion_depth};
    }

    if (array.size() == 1) {
        return {{{array[0]}}, std::nullopt, recursion_depth};
    }

    std::vector<int> tail(array.begin() + 1, array.end());
    SearchResult next = subset_sum_simple(tail, target, recursion_depth);

    if (next.found.has_value()) {
        return next;
    }

    std::vector<std::vector<int>> subsets{{array[0]}};

    for (const std::vector<int>& subset: next.subsets) {
        std::vector<std::vector<int>> temp_subsets;

        temp_subsets.push_back(subset);

        std::vector<int> with_first{array[0]};
        with_first.insert(with_first.end(), subset.begin(), subset.end());
        temp_subsets.push_back(with_first);

        for (const std::vector<int>& temp_subset: temp_subsets) {
            int sum = std::accumulate(
                temp_subset.begin(),
                temp_subset.end(),
                0
            );

            if (sum == target) {
                return {{}, temp_subset, recursion_depth};
            }

            subsets.push_back(temp_subset);
        }
    }

    return {subsets, std::nullopt, recursion_depth};
}

static void print_result(const SearchResult& result) {
    if (!result.found.has_value()) {
        std::cout << "false\n";
        return;
    }

    std::cout << "[";
    for (std::size_t i = 0; i < result.found->size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << (*result.found)[i];
    }
    std::cout << "] ";
    std::cout << "depth: " << result.recursion_depth << std::endl;
}

int main() {
    int target = 30;
    std::vector<int> array{3, 7, 11, 15, 19};
    int recursion_depth = 0;

    print_result(subset_sum_simple(array, target, recursion_depth));

    return 0;
}