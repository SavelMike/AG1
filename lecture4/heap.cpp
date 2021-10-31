/*
 * Minimun binary heap
 */

#include <iostream>
#include <random>
#include <vector>
#include <cstdlib>

class MinBinHeap {
private:
	std::vector<int> array;

public:
	MinBinHeap();
	MinBinHeap(unsigned n);
	MinBinHeap(const char *file);
		
	void HeapInsert(int val);
	int HeapFindMin(void);
	int HeapExtractMin();
	void HeapPrint(void);
	bool IsLeaf(unsigned idx);
	void BubbleDown(unsigned idx);
	void BubbleUp(unsigned index);
	void HeapBuild(void);
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
MinBinHeap::MinBinHeap(unsigned n) : array(n)
{
	for (unsigned i = 0; i < n; i++)
		this->array[i] = rand() % (n * 2) + 1;

	this->HeapBuild();
}

/*
 * if there are n vertices in heap, first n/2 are not leaves
 */
bool MinBinHeap::IsLeaf(unsigned n)
{
	return n > this->array.size() / 2;
}

/*
 * AG1, lecture 4, page 32
 * Note: index is from 1 to this->array.size(). Use -1 when accessing array elements
 */
void MinBinHeap::BubbleDown(unsigned index)
{
	unsigned childidx;

	while (!this->IsLeaf(index)) {
		/* child with smaller key */
		childidx = index * 2;
		if (this->array.size() > childidx &&
		    this->array[childidx + 1 - 1] < this->array[childidx - 1])
			/* there is right child and it is smaller */
			childidx++;

		if (this->array[index - 1] <= this->array[childidx - 1])
			return;

		/* swap index-th vertex with the child */
		int tmp = this->array[index - 1];
		this->array[index - 1] = this->array[childidx - 1];
		this->array[childidx - 1] = tmp;

		index = childidx;
	}
}

/*
 * AG1, lecture 4, page 32
 * Note: index is from 1 to this->array.size(). Use -1 when accessing array elements
 */
void MinBinHeap::BubbleUp(unsigned index)
{
	unsigned parent;

	while (index != 1) {
		parent = index / 2;

		if (this->array[parent - 1] <= this->array[index - 1])
			return;

		/* swap index-th vertex with its parent */
		int tmp = this->array[index - 1];
		this->array[index - 1] = this->array[parent - 1];
		this->array[parent - 1] = tmp;

		index = parent;
	}
}

/*
 *
 */
void MinBinHeap::HeapBuild(void)
{
	for (int i = this->array.size() / 2; i >= 1; i--)
		this->BubbleDown(i);
}

void MinBinHeap::HeapPrint(void)
{
	std::cout << "Array size: " << this->array.size() << "\n";
	int levelsize = 1;
	int levelcnt = 0;
	for (unsigned i = 0; i < this->array.size(); i++) {
		if (levelsize == levelcnt) {
			std::cout << "\n";
			levelsize *= 2;
			levelcnt = 0;
		}
		std::cout << " " << this->array[i];
		levelcnt++;
	}
	std::cout << "\n";
}

/*
 *
 */
void MinBinHeap::HeapInsert(int n)
{
	this->array.push_back(n);
	this->BubbleUp(this->array.size());
}

int main(void)
{
	MinBinHeap heap(10);

	heap.HeapPrint();

	heap.HeapInsert(-20);

	heap.HeapPrint();

	return 0;
}
