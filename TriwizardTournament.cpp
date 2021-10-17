// TriwizardTournament.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

struct coord
{
    int x;
    int y;
};

struct queue_element 
{
    coord cell;
    struct queue_element* next;
};

struct 
{
    queue_element* head;
    queue_element* tail;
} queue = {NULL, NULL};

coord dequeue() {
    if (queue.tail == NULL) {
        std::cerr << "Empty queue\n";
        exit(1);
    }
    coord rc = queue.head->cell;
    queue_element* phead = queue.head;
    queue.head = queue.head->next;
    
    if (queue.head == NULL) {
        // Queue is empty
        queue.tail = NULL;
    }
    delete phead;
    
    return rc;
}

void enqueue(coord* cell) {
    queue_element* qe = new struct queue_element;
    qe->cell = *cell;
    qe->next = NULL;
    // Add as last element to queue
    if (queue.tail == NULL) {
        queue.head = qe;
        queue.tail = qe;
    } else {
        queue.tail->next = qe;
    }
    
}

// Array of levers will be created while processing the input
struct Lever
{
    int vert_dist;
    bool change_vector[1000];
};

enum cell_state 
{
    unmarked, opened, closed
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

// Coordinates of goblet
coord goblet;

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

void bfs(coord* start, int n);

int main(void)
{
    int n; // Size of labyrinth of one side
    int k; // Num of levers
    
    // read size of labyrint and numnber of levers
    // example: 5 2
    std::cin >> n >> k;

    // Read levers configuration: k lines of format
    // example:
    // 2 00110
    // 3 10100
    struct Lever* levers = new Lever[k];
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
    std::cin >> goblet.x >> goblet.y;
 
    goblet.x--;
    goblet.y--;
    
    std::cout << "n = " << n << "; k = " << k << "\n";

    print_levers(levers, n, k);

    print_maze(n, &goblet);

    coord start = { 0, 0 };

    bfs(&start, n);
    
    // Free memory
    delete[] levers;
    for (int i = 0; i < n; i++) {
        delete[] maze[i];
    }

    delete[] maze;
    
    return 0;
}

// Return value: 
//      true if goblet is found
//      false otherwise
bool check_pair(coord* centr, coord* neighbour) {
    maze_cell* p = &maze[neighbour->x][neighbour->y];
    if (p->is_wall != true && p->state == unmarked) {
        p->state = opened;
        p->distance = maze[centr->x][centr->y].distance + 1;
        p->predecessor = *centr;
        // Check if we reached the target
        if (neighbour->x == goblet.x && neighbour->y == goblet.y) {
            // Reached the target
            return true;
        }
        enqueue(neighbour);
    }
    return false;
}

bool check_adjacents(coord* centr, int n) {

    coord neighbour;
    // Check top
    if (centr->y != 0) {
        neighbour = *centr;
        neighbour.y--;
        if (check_pair(centr, &neighbour)) {
            return true;
        }
    }
    // Check right
    if (centr->x != n - 1) {
        neighbour = *centr;
        neighbour.x++;
        if (check_pair(centr, &neighbour)) {
            return true;
        }
    }

    // Check left
    if (centr->x != 0) {
        neighbour = *centr;
        neighbour.x--;
        if (check_pair(centr, &neighbour)) {
            return true;
        }
    }

    // Check bottom
    if (centr->y != n - 1) {
        neighbour = *centr;
        neighbour.y++;
        if (check_pair(centr, &neighbour)) {
            return true;
        }
    }

    return false;
}


void bfs(coord* start, int n) {
    bool found = false;
    // Initialization of state, distance and predecessor
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            maze[i][j].state = unmarked;
            maze[i][j].distance = -1;
            maze[i][j].predecessor.x = -1;
            maze[i][j].predecessor.y = -1;
        }    
    }
    enqueue(start);
    maze[start->x][start->y].state = opened;
    maze[start->x][start->y].distance = 0;
    while (queue.head != NULL) {
        coord first_el = dequeue();
        if (check_adjacents(&first_el, n)) {
            found = true;
            break;
        }
        maze[first_el.x][first_el.y].state = closed;
    }

    if (found) {
        std::cout << "found; path length = " << maze[goblet.x][goblet.y].distance << "\n";
    }
    else {
        std::cout << "Goblet not found\n";
    }
}


