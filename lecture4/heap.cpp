/*
 * Minimun binary heap
 */

#include <iostream>
#include <vector>
#include <cstdlib>

class MinBinHeap {
private:
	std::vector<int> array;

public:
	MinBinHeap();
	MinBinHeap(int n);
	MinBinHeap(const char *file);
		
	void HeapInsert(int n);
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
}

/*
 * Round up a number to power of 2
 *	example: n = 10, rc = 16
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
MinBinHeap::MinBinHeap(int n) : array(n)
{
	for (int i = 0; i < n; i++)
		this->array[i] = rand();
}

void MinBinHeap::HeapPrint(void)
{
	std::cout << "Array size: " << this->array.size() << "\n";
	for (unsigned i = 0; i < this->array.size(); i++) {
		std::cout << " " << this->array[i];
	}
	std::cout << "\n";
}

/*
 *
 */
void MinBinHeap::HeapInsert(int n)
{
	this->array.push_back(n);
}

int main(void)
{
	MinBinHeap heap(10);

	heap.HeapPrint();

	return 0;
}
