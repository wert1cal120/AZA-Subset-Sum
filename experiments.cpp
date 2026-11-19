#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include "subset_sum.hpp"


static std::vector<int> get_solution(const NodeTree* result) {
    std::vector<int> solution;
    if (result != nullptr) {
        while (result->parent != nullptr) {
            if (result->number == 0) {
                result = result->parent;
                continue;
            }

            solution.push_back(result->number);

            result = result->parent;
        }
    }
    std::reverse(solution.begin(), solution.end());
    return solution;
}

void delete_tree(NodeTree* root) {
    if (root == nullptr) {
        return;
    }

    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

void run_both_algorithms(int target, std::vector<int> array) {
    auto time_limit = std::chrono::seconds(10);
    std::vector<int> found_solution_for_report;

    for (int algorithm = 0; algorithm < 2; algorithm++) {
        std::vector<double> times;
        int recursion_calls_for_report = 0;
        bool any_time_out = false;

        for (int repeat = 0; repeat < 3; repeat++) {
            NodeTree* root = new NodeTree();
            NodeTree* found = nullptr;
            int recursion_calls = 0;
            bool time_out = false;

            auto start = std::chrono::steady_clock::now();
            if (algorithm == 0) {
                subset_sum_baseline(array, root, nullptr, target, 0, 0,
                    recursion_calls, found, time_out, start + time_limit);
            } else {
                std::vector<int> array_sums(array.size(), 0);
                int sum = 0;

                for (std::size_t x = array.size(); x > 0; x--) {
                    sum += array[x-1];
                    array_sums[x-1] = sum;
                }
                subset_sum_improved(array, array_sums, root, nullptr, target, 0, 0,
                    recursion_calls, found, time_out, start + time_limit);
            }

            auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start);
            times.push_back(elapsed.count());

            if (repeat == 0) {
                recursion_calls_for_report = recursion_calls;
                found_solution_for_report = get_solution(found);
            }

            if (time_out) {
                any_time_out = true;
            }
            delete_tree(root);
        }

        if (algorithm == 0) {
            std::cout << "Baseline alg: ";
        } else {
            std::cout << "Improved alg: ";
        }

        std::sort(times.begin(), times.end());
        if (!any_time_out) {
            std::cout << "Recursive calls: " << recursion_calls_for_report;
            std::cout << " | Runtime median: " << times[1];
            std::cout << " | Solution: ";
            for (std::size_t x = 0; x < found_solution_for_report.size(); x++) {
                std::cout << found_solution_for_report[x] << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "Recursive calls: " << recursion_calls_for_report;
            std::cout << " | Runtime median: " << times[1];
            std::cout << " - TIMEOUT" << std::endl;
        }
    }

    std::cout << "S=" << target << " Array: ";
    std::cout << "[";
    for (std::size_t x = 0; x < array.size(); x++) {
        if (x < array.size() - 1) {
            std::cout << array[x] << ", ";
        } else {
            std::cout << array[x];
        }
    }
    std::cout << "]" << std::endl;

    std::cout << "\n";
}


int main() {
    std::vector<int> n{10, 15, 20};
    constexpr int seed = 58111158;
    std::srand(seed);

    std::cout << "SEED: "<< seed << std::endl;

    std::cout << "Experiment for n: ";
    for (const std::size_t i : n) {std::cout << i << " ";}
    std::cout << "\n------------------------------\n";

    for (const std::size_t i : n) {
        std::cout << "N=" << i << std::endl;
        int target;
        std::vector<int> array;

        switch (i) {
            case 10:

                //Fixed test
                std::cout << "Fixed test:\n";
                target = 81;
                array = {3, 7, 11, 15, 19, 9, 21, 17, 13, 31};
                run_both_algorithms(target, array);

                //Random test
                std::cout << "Random test:\n";
                target = std::rand() % (100 - 1 + 1) + 1;

                array = {};
                for (int j = 0; j < i; j++) {
                    array.push_back(std::rand() % (100 - 1 + 1) + 1);
                }
                run_both_algorithms(target, array);

                //Fixed fail test
                std::cout << "Fixed impossible test:\n";
                target = 33;
                array = {2, 6, 4, 12, 18, 8, 26, 10, 30, 22};
                run_both_algorithms(target, array);
                break;

            case 15:
                //Fixed test
                std::cout << "Fixed test:\n";
                target = 68;
                array = {3, 7, 11, 15, 19, 9, 21, 17, 13, 31, 23, 27, 29, 33, 35};
                run_both_algorithms(target, array);

                //Random test
                std::cout << "Random test:\n";
                target = std::rand() % (100 - 1 + 1) + 1;

                array = {};
                for (int j = 0; j < i; j++) {
                    array.push_back(std::rand() % (100 - 1 + 1) + 1);
                }
                run_both_algorithms(target, array);

                //Fixed fail test
                std::cout << "Fixed impossible test:\n";
                target = 242;
                array = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
                run_both_algorithms(target, array);
                break;

            case 20:
                //Fixed test
                std::cout << "Fixed test:\n";
                target = 90;
                array = {3, 7, 11, 15, 19, 9, 21, 17, 13, 31, 23, 27, 29, 33, 35, 37, 39, 41, 43, 47};
                run_both_algorithms(target, array);

                //Random test
                std::cout << "Random test:\n";
                target = std::rand() % (100 - 1 + 1) + 1;

                array = {};
                for (int j = 0; j < i; j++) {
                    array.push_back(std::rand() % (100 - 1 + 1) + 1);
                }
                run_both_algorithms(target, array);

                //Fixed fail test
                std::cout << "Fixed impossible test:\n";
                target = 402;
                array = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80};
                run_both_algorithms(target, array);
                break;
            default:
                break;
        }
        std::cout << "------------------------------\n";
    }

    return 0;
}