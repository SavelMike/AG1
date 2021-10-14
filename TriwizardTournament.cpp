// TriwizardTournament.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// Array of levers will be created while processing the input
struct Lever
{
    int vert_dist;
    bool change_vector[1000];
};

void print_levers(struct Lever* levers, int n, int k) {
    for (int i = 0; i < k; i++) {
        std::cout << levers[i].vert_dist;
        for (int j = 0; j < n; j++) {
            std::cout << levers[i].change_vector[j];
        }
    }
}

int main()
{
    int n; // Size of labyrinth of one side
    int k; // Num of levers
    
    std::cin >> n >> k;
    struct Lever* levers = new Lever[k];
    // Read levers configuration
    for (int i = 0; i < k; i++) {
        // Expect 6 001100 (n = 6)
        std::cin >> levers[i].vert_dist;
        for (int j = 0; j < n; j++) {
            std::cin >> levers[i].change_vector[j];
        }
    }
    
    std::cout << "n = " << n << "; k = " << k << "\n";

    print_levers(levers, n, k);
    return 0;
}



/*
dfs() {
    ;
}
*/

