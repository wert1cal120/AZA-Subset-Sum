#include <iostream>
#include <vector>
#include <chrono>
#include "subset_sum.hpp"

using namespace std::chrono;

//TODO: reverse result sequence
static void print_result(const NodeTree* result) {
    if (result != nullptr) {
        std::cout << "[";
        while (result->parent != nullptr) {
            if (result->number == 0) {
                result = result->parent;
                continue;
            }

            if (result->left != nullptr || result->right != nullptr) {
                std::cout << ", " << result->number;
            } else {
                std::cout << result->number;
            }

            result = result->parent;
        }
        std::cout << "] ";
    } else {
        std::cout << "no solution ";
    }
}

void delete_tree(NodeTree* root) {
    if (root == nullptr) {
        return;
    }

    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

void run_both(int target, std::vector<int> array) {
    for (int j = 0; j < 2; j++) {
        NodeTree* root = new NodeTree();
        NodeTree* found = nullptr;
        int recursion_calls = 0;
        bool time_out = false;

        auto start = steady_clock::now();
        if (j == 0) {
            subset_sum_baseline(array, root, nullptr, target, 0, 0, recursion_calls, found, time_out, start + seconds(10));
        } else {
            std::vector<int> array_sums(array.size(), 0);
            int sum = 0;

            for (int x = array.size(); x > 0; x--) {
                sum += array[x-1];
                array_sums[x-1] = sum;
            }

            subset_sum_improved(array, array_sums, root, nullptr, target, 0, 0, recursion_calls, found, time_out, start + seconds(10));
        }
        auto elapsed = std::chrono::duration<double, std::milli>(steady_clock::now() - start);

        if (!time_out) {
            print_result(found);
            std::cout << "Recursion calls: "<< recursion_calls << std::endl;
            std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;
        } else {
            std::cout << "TIMEOUT" << std::endl;
        }
        delete_tree(root);
    }
    std::cout << "\n";
}


int main() {
    std::vector<int> n{10, 15, 20};
    std::srand(time(nullptr));

    std::cout << "Experiment for n: ";
    for (int i = 0; i < n.size(); i++) {std::cout << n[i] << " ";}
    std::cout << "\n------------------------------\n";

    for (int i = 0; i < n.size(); i++) {
        std::cout << "N=" << n[i] << std::endl;
        int target;
        std::vector<int> array;

        switch (n[i]) {
            case 10:

                //Fixed test
                std::cout << "Fixed test:\n";
                target = 81;
                array = {3, 7, 11, 15, 19, 9, 21, 17, 13, 31};
                run_both(target, array);

                //Random test
                std::cout << "Random test:\n";
                target = std::rand() % (100 - 1 + 1) + 1;
                std::cout << "S=" <<  target << " Array: ";

                array = {};
                for (int j = 0; j < n[i]; j++) {
                    array.push_back(std::rand() % (100 - 1 + 1) + 1);
                    std::cout << array[j] << " ";
                }
                std::cout << std::endl;
                run_both(target, array);

                //Fixed fail test
                std::cout << "Fixed impossible test:\n";
                target = 33;
                array = {2, 6, 4, 12, 18, 8, 26, 10, 30, 22};
                run_both(target, array);
                break;

            case 15:
                //Fixed test
                std::cout << "Fixed test:\n";
                target = 68;
                array = {3, 7, 11, 15, 19, 9, 21, 17, 13, 31, 23, 27, 29, 33, 35};
                run_both(target, array);

                //Random test
                std::cout << "Random test:\n";
                target = std::rand() % (100 - 1 + 1) + 1;
                std::cout << "S=" <<  target << " Array: ";

                array = {};
                for (int j = 0; j < n[i]; j++) {
                    array.push_back(std::rand() % (100 - 1 + 1) + 1);
                    std::cout << array[j] << " ";
                }
                std::cout << std::endl;
                run_both(target, array);

                //Fixed fail test
                std::cout << "Fixed impossible test:\n";
                target = 242;
                array = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
                run_both(target, array);
                break;

            case 20:
                //Fixed test
                std::cout << "Fixed test:\n";
                target = 90;
                array = {3, 7, 11, 15, 19, 9, 21, 17, 13, 31, 23, 27, 29, 33, 35, 37, 39, 41, 43, 47};
               run_both(target, array);

                //Random test
                std::cout << "Random test:\n";
                target = std::rand() % (100 - 1 + 1) + 1;
                std::cout << "S=" <<  target << " Array: ";

                array = {};
                for (int j = 0; j < n[i]; j++) {
                    array.push_back(std::rand() % (100 - 1 + 1) + 1);
                    std::cout << array[j] << " ";
                }
                std::cout << std::endl;
                run_both(target, array);

                //Fixed fail test
                std::cout << "Fixed impossible test:\n";
                target = 402;
                array = {4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80};
                run_both(target, array);
                break;
            default:
                break;
        }
        std::cout << "------------------------------\n";
    }

    return 0;
}