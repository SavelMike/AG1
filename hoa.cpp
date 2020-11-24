#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define TOMBSTONE -1

int double_hash(int k, int i)
{
	return (7 * k + i * (3 * k % 7 + 1)) % 17;
}

// Return value: index of key in hash table (if found)
// - 1 if not found
int open_find(int* arr, int nr_slots, int key)
{
	int i;
	int h;

	for (i = 0; i < nr_slots; i++) {
		h = double_hash(key, i);
		if (arr[h] == 0) {
			return -1;
		}
		if (arr[h] == key) {
			return h;
		}
		// Is a tombstone or arr[h] != key
	}
	return -1;
}

void open_insert(int* arr, int nr_slots, int key)
{
	int i;
	int h;
	int found;

	if (open_find(arr, nr_slots, key) != -1) {
		return;
	} 
	for (i = 0; i < nr_slots; i++) {
		h = double_hash(key, i);
		if (arr[h] == 0 || arr[h] == TOMBSTONE) {
			arr[h] = key;
			return;
		}
	}
	printf("Table is full\n");
	exit(1);
}

void open_delete(int* arr, int nr_slots, int key)
{
	int found;

	found = open_find(arr, nr_slots, key);
	if (found == -1) {
		return;
	}
	arr[found] = TOMBSTONE;
}
	

void print_array(int* arr, int nr_slots)
{
	int i;

	for (i = 0; i < nr_slots; i++) {
		if (arr[i] == TOMBSTONE) {
			printf("t");
		} else if (arr[i] != 0) {
			printf("%d", arr[i]);
		} else {
			printf("-");
		}
		if (i != nr_slots - 1) {
			printf(",");
		}
	}
	printf("\n");
}

int main(void)
{
	int* ht;
	int nr_slots;
	int i;
	int key;

	scanf("%d", &nr_slots);
	ht = (int*)malloc(sizeof(int) * nr_slots);
	if (ht == NULL) {
		perror("Malloc failed\n");
		exit(1);
	}

	for (i = 0; i < nr_slots; i++) {
		ht[i] = 0; // slot is yet empty
	}
	while (scanf("%d", &key) == 1) {
		if (key < 0) {
			open_delete(ht, nr_slots, key * (-1));
		} else {
			open_insert(ht, nr_slots, key);
		}
	}

	print_array(ht, nr_slots);
	free(ht);

	return 0;
}
