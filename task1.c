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

    // Reads number of planes and number of paths
	scanf("%d %d", &nplanets, &mpaths);

    // Reads number of initial planet, terminal planet 
	// and stops after getting infected
	scanf("%d %d %d", &start, &terminal, &infectstops);
	
	// Read amount of infected planets and their ids
	infplanetid = readintarray(&numofinfectedplanets);

	// Read amount of planets with vaccine and their ids
	planetswithvaccineids = readintarray(&planetswithvaccine);

	printf("num of planets = %d, num of paths = %d, init planet = %d, " 
		   "temr planet = %d, infected stops = %d\n", nplanets, mpaths, 
		    start, terminal, infectstops);

	printf("infected planets: \n");
	for (i = 0; i < numofinfectedplanets; i++) {
		printf("%d ", infplanetid[i]);
	}
	printf("\n");

	// Print planets with vaccines
	printf("Planets with vaccine: \n");
	for (i = 0; i < planetswithvaccine; i++) {
		printf("%d ", planetswithvaccineids[i]);
	}
	printf("\n");

	free(infplanetid);
	free(planetswithvaccineids);
	
	return 0;
}
