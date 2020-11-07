#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

// Read array
int* readintarray(int* nr)
{
	int* arr;
	int arrsize;
	int i;
	
	scanf("%d", &arrsize);

	arr = malloc(sizeof(int) * arrsize);
	if (arr == NULL) {
		perror("Malloc failed");
		exit(1);
	}
	for (i = 0; i < arrsize; i++) {
		scanf("%d", &arr[i]);
	}

	*nr = arrsize;
	
	return arr;
}

struct intlist 
{
	int value;
	struct intlist* next;
};

struct intlist_head {
	struct intlist* head; // Head of intlist 
	struct intlist* tail; // End of intlist 
};

#define STATELESS 0
#define INFECTED 1
#define HASVACCINE 2
#define BFSUNMARKED 0
#define BFSOPENED 1
#define BFSCLOSED 2
#define BFSPUNDEF -1
#define BFSDUNDEF -1

struct graphvertex
{
	int id; // identifier
	int status; // infected or has vaccine or stateless
	int bfs_distance; // required by BFS algorithm D(v)
	int bfs_mark; // required by BFS algorithm
	int bfs_pred; // Number of predecessor required by BFS algorithm P(v)
	struct intlist_head adjacency_list; // neighboor verteces
};

// Insert new element to beginning of intlist
// Arguments:
//		value (data to be added to the list)
//		head (pointer to list head, updated to new first element of list)
// return nothing
void add_intlist_first(int value, struct intlist_head* head)
{
	struct intlist* newelement;
	newelement = malloc(sizeof(struct intlist));
	if (newelement == NULL) {
		perror("Malloc failed");
		exit(1);
	}
	newelement->value = value;
	newelement->next = head->head;
	head->head = newelement;
	if (head->tail == NULL) {
		head->tail = newelement;
	}
}

void add_intlist_last(int value, struct intlist_head* head)
{
	struct intlist* newelement;
	newelement = malloc(sizeof(struct intlist));
	if (newelement == NULL) {
		perror("Malloc failed");
		exit(1);
	}
	newelement->value = value;
	newelement->next = NULL;
	if (head->tail != NULL) {
		head->tail->next = newelement->next;
	}
	else {
		head->head = newelement;
	}
	head->tail = newelement;
}

// Remove first element from the List
// Return value of returned element
// Arguments:
//		head (pointer to pointer to list head)
int delete_intlist_first(struct intlist_head* head)
{
	int rc;
	struct intlist* next;
	
	rc = head->head->value;
	next = head->head->next;
	free(head->head);
	head->head = next;
	if (next == NULL) {
		head->tail = NULL;
	}

	return rc;
}

void print_intlist(struct intlist* head)
{
	while (head != NULL) {
		printf("%d ", head->value);
		head = head->next;
	}
}

void free_intlist(struct intlist* head)
{
	while (head != NULL) {
		struct intlist* temp;
		temp = head->next;
		free(head);
		head = temp;
	}
}

int is_empty_intlist(const struct intlist_head* head)
{
	return head->head == NULL;
}

char* statenames[] =
{
	[STATELESS] = "stateless",[INFECTED] = "infected",
	[HASVACCINE] = "has vaccine"
};

void print_graph(struct graphvertex* graph, int nvertex)
{
	int i;
	
	for (i = 0; i < nvertex; i++) {
		printf("id = %d status = %s pred = %d distance = %d adjacent vertices: ", graph[i].id, 
				statenames[graph[i].status], graph[i].bfs_pred, graph[i].bfs_distance);
		print_intlist(graph[i].adjacency_list.head);
		printf("\n");
	}
}

void free_graph(struct graphvertex* graph, int nvertex)
{
	int i;

	for (i = 0; i < nvertex; i++) {
		free_intlist(graph[i].adjacency_list.head);
	}
	free(graph);
}

void print_path(struct graphvertex* graph, int vertex)
{
	if (graph[vertex].bfs_pred = BFSPUNDEF) {
		printf("%d ", vertex);
		return;
	}
	print_path(graph, graph[vertex].bfs_pred);
	printf("%d ", vertex);
}

void BFS_planet_walk(struct graphvertex* graph, int nvertex, int start, int terminal)
{
	struct intlist_head Q; // Serves as queue for BFS
	
	Q.head = NULL;
	Q.tail = NULL;
	add_intlist_first(start, &Q);
	graph[start].bfs_mark = BFSOPENED;
	graph[start].bfs_distance = 0;
	
	while (!is_empty_intlist(&Q)) {
		int v;
		struct intlist* w;
		v = delete_intlist_first(&Q);
		for (w = graph[v].adjacency_list.head; w != NULL; w = w->next) {
			if (graph[w->value].bfs_mark == BFSUNMARKED) {
				graph[w->value].bfs_mark = BFSOPENED;
				graph[w->value].bfs_distance = graph[v].bfs_distance + 1;
				graph[w->value].bfs_pred = graph[v].id; // or just v
				if (graph[w->value].id == terminal) {
					// Reached the terminal planet
					printf("*******************\n");
					print_path(graph, terminal);
					printf("\n");
				}
				add_intlist_last(graph[w->value].id, &Q);
			}
		}
		graph[v].bfs_mark = BFSCLOSED;
	}
}

int main(void)
{
	int nplanets; // Num of planets
	int mpaths; // Num of paths
	int start; // Initial planet
	int terminal; // Terminal planet
	int infectstops; // Stops after crew was infected
	int numofinfectedplanets; // Amount of infected planets 
	int* infplanetid; // Array of inf planets ids
	int i;
	int planetswithvaccine; // Amount of planets with vaccine
	int* planetswithvaccineids; // Array of planets with vaccine ids
	struct graphvertex* graph; // Array of verteces or the graph itself

    // Reads number of planes and number of paths
	scanf("%d %d", &nplanets, &mpaths);

    // Reads number of initial planet, terminal planet 
	// and stops after getting infected
	scanf("%d %d %d", &start, &terminal, &infectstops);
	
	// Read amount of infected planets and their ids
	infplanetid = readintarray(&numofinfectedplanets);

	// Read amount of planets with vaccine and their ids
	planetswithvaccineids = readintarray(&planetswithvaccine);

	// Constuct a graph in adjustency vertices form
	graph = malloc(sizeof(struct graphvertex) * nplanets);
	if (graph == NULL) {
		perror("Malloc failed");
		exit(1);
	}

	// Initilize all vertices of the graph
	for (i = 0; i < nplanets; i++) {
		graph[i].id = i;
		graph[i].status = STATELESS;
		graph[i].adjacency_list.head = NULL;
		graph[i].adjacency_list.tail = NULL;
		graph[i].bfs_mark = BFSUNMARKED;
		graph[i].bfs_distance = BFSDUNDEF;
		graph[i].bfs_pred = BFSPUNDEF;
	}

	// Set status of vertex
	for (i = 0; i < numofinfectedplanets; i++) {
		graph[infplanetid[i]].status = INFECTED;
	}

	for (i = 0; i < planetswithvaccine; i++) {
		graph[planetswithvaccineids[i]].status = HASVACCINE;
	}

	// Read edges inf
	for (i = 0; i < mpaths; i++) {
		int v1;
		int v2;
		scanf("%d %d", &v1, &v2);
		add_intlist_first(v1, &graph[v2].adjacency_list);
		add_intlist_first(v2, &graph[v1].adjacency_list);
	}

	printf("num of planets = %d, num of paths = %d, init planet = %d, " 
		   "term planet = %d, infected stops = %d\n", nplanets, mpaths, 
		    start, terminal, infectstops);

	BFS_planet_walk(graph, nplanets, start, terminal);

	print_graph(graph, nplanets);

	free(infplanetid);
	free(planetswithvaccineids);
	
	return 0;
}
