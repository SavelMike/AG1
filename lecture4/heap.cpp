/*
 * Minimun binary heap
 */

#include <iostream>
#include <cstdlib>

class MinBinHeap {
private:
	int *arr;
	int size;
	int memsize;
public:
	MinBinHeap();
	MinBinHeap(int n);
	MinBinHeap(const char *file);
		
	void HeapInsert();
	int HeapFindMin(void);
	int HeapExtractMin();
	void HeapPrint(void);
};

/*
 * Default constructor
 *	creare heap with 0 elements
 */
MinBinHeap::MinBinHeap()
{
	this->arr = nullptr;
	this->size = 0;
}

/*
 * Round up a number to power of 2
 *	n = 10, rc = 16
 */
int ceil2(int n)
{
	int rc = 1;

	while (rc < n)
		rc <<= 1;
	return rc;
}

/*
 * Random data filled heap of size n constructor
 *	create heap of n element, fill with random data
 */
MinBinHeap::MinBinHeap(int n)
{
	this->memsize = ceil2(n);
	this->arr = new int[this->memsize];

	for (int i = 0; i < n; i++)
		this->arr[i] = rand();
	this->size = n;
}

void MinBinHeap::HeapPrint(void)
{
	for (int i = 0; i < this->size; i++) {
		std::cout << " " << this->arr[i];
	}
	std::cout << "\n";
}

int main(void)
{
	MinBinHeap heap(10);

	heap.HeapPrint();

	return 0;
}
