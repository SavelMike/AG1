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
        std::cout << levers[i].vert_dist << ' ';

        for (int j = 0; j < n; j++) {
            std::cout << levers[i].change_vector[j];
        }
        std::cout << "\n";
    }
}

void print_maze(bool** maze, int n, int goblet_i, int goblet_j) {
    std::cout << "Maze:\n";
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < n; j++) {
            if (i == goblet_i && j == goblet_j) {
                std::cout << " G ";
            }
            else {
                std::cout << ' ' << maze[i][j] << ' ';
            }
        }
        std::cout << "\n";
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
            char c;
            std::cin >> c;
            if (c == '0') {
                levers[i].change_vector[j] = false;
            }
            else {
                levers[i].change_vector[j] = true;
            }
        }
    }

 
    // Read maze configuration
    // Define 2D array of booleans
    bool** maze = new bool* [n];
    for (int i = 0; i < n; i++) {
        maze[i] = new bool[n];
        for (int j = 0; j < n; j++) {
            char c;
            std::cin >> c;
            if (c == '0') {
                maze[i][j] = false;
            }
            else {
                maze[i][j] = true;
            }
        }
    }

    // Read Goblet coordinates
    int goblet_i;
    int goblet_j;
    std::cin >> goblet_i >> goblet_j;
 
    goblet_i--;
    goblet_j--;
    
    std::cout << "n = " << n << "; k = " << k << "\n";

    print_levers(levers, n, k);

    print_maze(maze, n, goblet_i, goblet_j);
    
    // Free memory
    delete[] levers;
    for (int i = 0; i < n; i++) {
        delete[] maze[i];
    }

    delete[] maze;
    
    return 0;
}


/*
dfs() {
    ;
}
*/

