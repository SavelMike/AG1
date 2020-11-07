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

#define STATELESS 0
#define INFECTED 1
#define HASVACCINE 2

struct graphvertex
{
	int id; // identifier
	int status; // infected or has vaccine or stateless
	struct intlist* adjactedverteces; // neighboor verteces
};

// Insert new element to beginning of intlist
// Arguments:
//		value (data to be added to the list)
//		head (pointer to list head, updated to new first element of list)
// return nothing
void add_intlist_first(int value, struct intlist** head)
{
	struct intlist* newelement;
	newelement = malloc(sizeof(struct intlist));
	if (newelement == NULL) {
		perror("Malloc failed");
		exit(1);
	}
	newelement->value = value;
	newelement->next = *head;
	*head = newelement;
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

char* statenames[] =
{
	[STATELESS] = "stateless",[INFECTED] = "infected",
	[HASVACCINE] = "has vaccine"
};

void print_graph(struct graphvertex* graph, int nvertex)
{
	int i;
	
	for (i = 0; i < nvertex; i++) {
		printf("id = %d status = %s adjacent vertices: ", graph[i].id, 
				statenames[graph[i].status]);
		print_intlist(graph[i].adjactedverteces);
		printf("\n");
	}
}

void free_graph(struct graphvertex* graph, int nvertex)
{
	int i;

	for (i = 0; i < nvertex; i++) {
		free_intlist(graph[i].adjactedverteces);
	}
	free(graph);
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

	// Initilize all fields of the array
	for (i = 0; i < nplanets; i++) {
		graph[i].id = i;
		graph[i].status = STATELESS;
		graph[i].adjactedverteces = NULL;
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
		add_intlist_first(v1, &graph[v2].adjactedverteces);
		add_intlist_first(v2, &graph[v1].adjactedverteces);
	}

	printf("num of planets = %d, num of paths = %d, init planet = %d, " 
		   "term planet = %d, infected stops = %d\n", nplanets, mpaths, 
		    start, terminal, infectstops);

	print_graph(graph, nplanets);

	free(infplanetid);
	free(planetswithvaccineids);
	
	return 0;
}
