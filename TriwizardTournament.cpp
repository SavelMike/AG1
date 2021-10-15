// TriwizardTournament.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// Array of levers will be created while processing the input
struct Lever
{
    int vert_dist;
    bool change_vector[1000];
};

enum cell_state 
{
    undef, opened, closed
};

struct coord
{
    int x;
    int y;
};

struct maze_cell
{
    bool is_wall;
    int distance;
    struct coord predecessor;
    cell_state state;
};

// Labyrinth
struct maze_cell** maze;

void print_levers(struct Lever* levers, int n, int k) {
    for (int i = 0; i < k; i++) {
        std::cout << levers[i].vert_dist << ' ';

        for (int j = 0; j < n; j++) {
            std::cout << levers[i].change_vector[j];
        }
        std::cout << "\n";
    }
}

void print_maze(int n, coord* goblet) {
    std::cout << "Maze:\n";
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 0; j < n; j++) {
            if (i == goblet->x && j == goblet->y) {
                std::cout << " G ";
            }
            else {
                std::cout << ' ' << maze[i][j].is_wall << ' ';
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
    maze = new struct maze_cell* [n];
    for (int i = 0; i < n; i++) {
        maze[i] = new struct maze_cell[n];
        for (int j = 0; j < n; j++) {
            char c;
            std::cin >> c;
            if (c == '0') {
                maze[i][j].is_wall = false;
            }
            else {
                maze[i][j].is_wall = true;
            }
            // Initiliazation of other fileds is in BFS
        }
    }

    // Read Goblet coordinates
    coord goblet;
    
    std::cin >> goblet.x >> goblet.y;
 
    goblet.x--;
    goblet.y--;
    
    std::cout << "n = " << n << "; k = " << k << "\n";

    print_levers(levers, n, k);

    print_maze(n, &goblet);
    
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

