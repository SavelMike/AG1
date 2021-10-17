// TriwizardTournament.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

struct coord
{
    int x;
    int y;
};

// Element of a queue
struct queue_element 
{
    struct coord cell;
    struct queue_element* next;
};

// Queue: holds pointer to head and tail of the queue
struct 
{
    struct queue_element* head;
    struct queue_element* tail;
} queue = {NULL, NULL};

void print_queue(void);

// Remove first element from the queue
// return value:
//      struct coord
struct coord dequeue(void) {
    if (queue.tail == NULL) {
        std::cerr << "Empty queue\n";
        exit(1);
    }
    struct coord rc = queue.head->cell;
    struct queue_element* phead = queue.head;
    
    queue.head = queue.head->next;
    
    if (queue.head == NULL) {
        // Queue became empty
        queue.tail = NULL;
    }
    delete phead;
    
 //   print_queue();

    return rc;
}

// Add new element to end of the queue
void enqueue(struct coord* cell) {
    struct queue_element* qe = new struct queue_element;
    qe->cell = *cell;
    qe->next = NULL;
    // Add as last element to queue
    if (queue.tail == NULL) {
        queue.head = qe;
    } else {
        queue.tail->next = qe;
    }
    queue.tail = qe;
 //   print_queue();
}

void print_queue(void) {
    if (queue.head == NULL) {
        std::cout << "Empty queue : tail = " << queue.tail << '\n';
        return;
    }
    struct queue_element* qe = queue.head;
    while (qe != NULL) {
        std::cout << " [" << qe->cell.x << ';' << qe->cell.y << "]";
        qe = qe->next;
    }
    std::cout << "\n";
}

// Array of levers will be created while processing the input
struct lever
{
    int vert_dist;
    bool change_vector[1000];
};

enum cell_state 
{
    unmarked, opened, closed
};

// Cell of 2D labyrinth
struct maze_cell
{
    bool is_wall;
    int distance;
    struct coord predecessor;
    cell_state state;
};

// Labyrinth
struct {
    struct maze_cell** map;
    int n;
} maze;


// Coordinates of goblet
struct coord goblet;

void print_levers(struct lever* levers, int k) {
    for (int i = 0; i < k; i++) {
        std::cout << levers[i].vert_dist << ' ';

        for (int j = 0; j < maze.n; j++) {
            std::cout << levers[i].change_vector[j];
        }
        std::cout << "\n";
    }
}

void print_maze(struct coord* goblet) {
    std::cout << "Maze:\n";
    for (int i = maze.n - 1; i >= 0; i--) {
        for (int j = 0; j < maze.n; j++) {
            if (i == goblet->x && j == goblet->y) {
                std::cout << " G ";
            }
            else {
                std::cout << ' ' << maze.map[i][j].is_wall << ',' << maze.map[i][j].state << ' ';
            }
        }
        std::cout << "\n";
    }
}

void bfs(struct coord* start);

int main(void)
{
    int k; // Num of levers
    
    // read size of labyrint and numnber of levers
    // example: 5 2
    std::cin >> maze.n >> k;

    // Read levers configuration: k lines of format
    // example:
    // 2 00110
    // 3 10100
    struct lever* levers = new lever[k];
    for (int i = 0; i < k; i++) {
        // Expect 6 001100 (n = 6)
        std::cin >> levers[i].vert_dist;
        for (int j = 0; j < maze.n; j++) {
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
    maze.map = new struct maze_cell* [maze.n];
    for (int i = 0; i < maze.n; i++) {
        maze.map[i] = new struct maze_cell[maze.n];
        for (int j = 0; j < maze.n; j++) {
            char c;
            std::cin >> c;
            if (c == '0') {
                maze.map[i][j].is_wall = false;
            }
            else {
                maze.map[i][j].is_wall = true;
            }
            // Initialization of other fileds is in BFS
        }
    }

    // Read Goblet coordinates
    std::cin >> goblet.x >> goblet.y;
    goblet.x--;
    goblet.y--;
    
    std::cout << "n = " << maze.n << "; k = " << k << "\n";

    print_levers(levers, k);

    coord start = { 0, 0 };

    bfs(&start);
    
    // Free memory
    delete[] levers;
    for (int i = 0; i < maze.n; i++) {
        delete[] maze.map[i];
    }

    delete[] maze.map;
    
    return 0;
}

// Return value: 
//      true if goblet is found
//      false otherwise
bool check_pair(struct coord* centr, struct coord* neighbour) {
    struct maze_cell* p = &maze.map[neighbour->x][neighbour->y];
    if (p->is_wall != true && p->state == unmarked) {
        p->state = opened;
        p->distance = maze.map[centr->x][centr->y].distance + 1;
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

// Auxillary function for bfs to process neighbours of cell
bool check_adjacents(struct coord* centr) {

    struct coord neighbour;

    // Check top
    if (centr->y != 0) {
        neighbour = *centr;
        neighbour.y--;
        if (check_pair(centr, &neighbour)) {
            return true;
        }
    }
    // Check right
    if (centr->x != maze.n - 1) {
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
    if (centr->y != maze.n - 1) {
        neighbour = *centr;
        neighbour.y++;
        if (check_pair(centr, &neighbour)) {
            return true;
        }
    }

    return false;
}

void print_path(struct coord* cur) {
    if (maze.map[cur->x][cur->y].predecessor.x != -1) {
        print_path(&maze.map[cur->x][cur->y].predecessor);
    }
    std::cout << "[" << cur->x + 1 << ";" << cur->y + 1 << "]";
    if (cur->x == goblet.x && cur->y == goblet.y) {
        std::cout << "\n";
    }
    else {
        std::cout << ",";
    }
}

// Breadth-first search
void bfs(struct coord* start) {
    bool found = false;
    // Initialization of state, distance and predecessor
    for (int i = 0; i < maze.n; i++) {
        for (int j = 0; j < maze.n; j++) {
            maze.map[i][j].state = unmarked;
            maze.map[i][j].distance = -1;
            maze.map[i][j].predecessor.x = -1;
            maze.map[i][j].predecessor.y = -1;
        }    
    }
    print_maze(&goblet);
    enqueue(start);
    maze.map[start->x][start->y].state = opened;
    maze.map[start->x][start->y].distance = 0;
    while (queue.head != NULL) {
        coord first_el = dequeue();
        if (check_adjacents(&first_el)) {
            found = true;
            break;
        }
        maze.map[first_el.x][first_el.y].state = closed;
    }

    if (found) {
        std::cout << "found; path length = " << maze.map[goblet.x][goblet.y].distance << "\n";
        print_path(&goblet);
    }
    else {
        std::cout << "Goblet not found\n";
    }
}