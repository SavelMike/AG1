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
	int bfs_mark; // required by BFS algorithm
//	int bfs_distance; // required by BFS algorithm D(v)
//	int bfs_pred; // List of predecessors required by BFS algorithm P(v)
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
		head->tail->next = newelement;
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

void delete_intlist_last(struct intlist_head* list, struct intlist* old_last)
{
	free(list->tail);
	list->tail = old_last;
	if (old_last == NULL) {
		list->head = NULL;
	}
	else {
		list->tail->next = NULL;
	}
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
	[STATELESS] = "stateless",
	[INFECTED] = "infected",
	[HASVACCINE] = "has vaccine"
};

char* marknames[] =
{
	[BFSUNMARKED] = "unmarked", 
	[BFSOPENED] = "opened",
	[BFSCLOSED] = "closed"
};

#if 0
void print_graph(struct graphvertex* graph, int nvertex)
{
	int i;
	
	for (i = 0; i < nvertex; i++) {
		printf("id = %d status = %s pred = %d distance = %d mark = %s adjacent vertices: ", graph[i].id, 
				statenames[graph[i].status], graph[i].bfs_pred, graph[i].bfs_distance, marknames[graph[i].bfs_mark]);
		print_intlist(graph[i].adjacency_list.head);
		printf("\n");
	}
}
#endif

void free_graph(struct graphvertex* graph, int nvertex)
{
	int i;

	for (i = 0; i < nvertex; i++) {
		free_intlist(graph[i].adjacency_list.head);
	}
	free(graph);
}

#if 0
void print_path(struct graphvertex* graph, int vertex)
{
	if (graph[vertex].bfs_pred == BFSPUNDEF) {
		printf("%d ", vertex);
		return;
	}
	print_path(graph, graph[vertex].bfs_pred);
	printf("%d ", vertex);
}
#endif

int infectstops; // Stops after crew was infected

int planet_walk_safe(struct intlist_head* path, struct graphvertex* graph)
{
	struct intlist* cur;
	int is_infected = 0;
	int jumps_left = -1;
	
	cur = path->head;
	while (cur != NULL) {
		if (jumps_left != -1) {
			jumps_left--;
			if (jumps_left < 0) {
				return 0;
			}
		}
		if (!is_infected) {
			if (graph[cur->value].status == INFECTED) {
				jumps_left = infectstops;
				is_infected = 1;
			}
			cur = cur->next;
			continue;
		}
		// We are infected
		if (graph[cur->value].status == HASVACCINE) {
			is_infected = 0;
			jumps_left = -1;
		}
		cur = cur->next;
	}
	return 1;
}

void DFS_planet_walk(int curvertex, struct graphvertex* graph, int nvertex, int terminal, struct intlist_head* path)
{
	struct intlist* w;
	struct intlist* old_last = path->tail;
	
	add_intlist_last(curvertex, path);
	if (curvertex == terminal) {
		if (planet_walk_safe(path, graph)) {
			print_intlist(path->head);
			exit(0);
		}
		printf("Found planet %d\n", terminal);
		print_intlist(path->head);
		printf("\n");
		// Remove last element
		delete_intlist_last(path, old_last);
		return;
	}
	if (graph[curvertex].bfs_mark == BFSOPENED) {
		delete_intlist_last(path, old_last);		
		return;
	}
	graph[curvertex].bfs_mark = BFSOPENED;
	for (w = graph[curvertex].adjacency_list.head; w != NULL; w = w->next) {
		DFS_planet_walk(w->value, graph, nvertex, terminal, path);
	}
	graph[curvertex].bfs_mark = BFSUNMARKED;
	delete_intlist_last(path, old_last);
}

int main(void)
{
	int nplanets; // Num of planets
	int mpaths; // Num of paths
	int start; // Initial planet
	int terminal; // Terminal planet
	int numofinfectedplanets; // Amount of infected planets 
	int* infplanetid; // Array of inf planets ids
	int i;
	int planetswithvaccine; // Amount of planets with vaccine
	int* planetswithvaccineids; // Array of planets with vaccine ids
	struct graphvertex* graph; // Array of verteces or the graph itself
	struct intlist_head path = { .head = NULL, .tail = NULL }; // Path for DFS 

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
//		graph[i].bfs_distance = BFSDUNDEF;
//		graph[i].bfs_pred = BFSPUNDEF;
	}

	// Set status of vertex
	for (i = 0; i < numofinfectedplanets; i++) {
		graph[infplanetid[i]].status = INFECTED;
	}

	for (i = 0; i < planetswithvaccine; i++) {
		graph[planetswithvaccineids[i]].status = HASVACCINE;
	}

	// Read edges
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

	DFS_planet_walk(start, graph, nplanets, terminal, &path);
	printf("-1"); // No safe planet walk

//	print_graph(graph, nplanets);

	free(infplanetid);
	free(planetswithvaccineids);
	
	return 0;
}
