#include "SortedArray.h"
#include <iostream>
#include <assert.h>

using namespace std;

 int range_rand(int min, int max) {
	int rc = 0;
	assert(min <= max);
	rc = (rand() % (max - min + 1)) + min;
	assert(rc >= min && rc <= max);

	return rc;
}

class sorted_array
{
private:
	int* arr;
	int size;

public:
	sorted_array(int sizex) {
		this->size = sizex;
		this->arr = new int[sizex];
	}

	sorted_array(int arrx[], int sizex) {
		this->size = sizex;
		this->arr = new int[sizex];
		for (int i = 0; i < sizex; i++) {
			this->arr[i] = arrx[i];
		}
	}

	~sorted_array() {
		delete[] this->arr;
	}

	// Properties:
	//	time complexity O(x^2)
	//	stable, in-place, data sensitive
	void bubble_sort() {
		int end = this->size - 1;
		int changed = 1;
		while (changed == 1) {
			changed = 0;
			for (int i = 0; i < end; i++) {
				if (arr[i] > arr[i + 1]) {
					int tmp = arr[i + 1];
					arr[i + 1] = arr[i];
					arr[i] = tmp;
					changed = 1;
				}
			}
			end--;
		}
	}

	// Properties:
	//	time complexity O(n^2)
	//	in-place, not data sensitive, not stable
	void select_sort() {
		for (int i = 0; i < this->size; i++) {
			int ind = i;
			for (int j = i + 1; j < this->size; j++) {
				if (arr[j] < arr[ind]) {
					ind = j;
				}
			}
			int tmp = arr[i];
			arr[i] = arr[ind];
			arr[ind] = tmp;
		}
	}

	// Properties:
	//	time complexity O(n^2)
	//	in-place, data sensitive, stable
	void insert_sort() {
		for (int i = 1; i < this->size; i++) {
			int key = arr[i];
			int j = i - 1;
			while ((j >= 0) && (arr[j] > key)) {
				arr[j + 1] = arr[j];
				j = j - 1;
			}
			arr[j + 1] = key;
		}
	}

	void merge_sort() {

	}

	// Design an algorithm which finds out the number 
	// of inverses in the input sequence.
	// Ex 2. Tutorial 5
	int num_of_inverses() {
		int count = 0;
		for (int i = 1; i <= this->size; i++) {
			for (int j = i + 1; j <= this->size; j++) {
				if (arr[i] > arr[j]) {
					count++;
				}
			}
		}

		return count;
	}

	// Array elements are {0 <= this->arr[i] <= this->size^3 - 1}
	void radix_sort() {
		int *count = new int[this->size];
		int* output = new int[this->size];
		for (int k = 0; k < 3; k++) {
			int div = pow(this->size, k);
			for (int i = 0; i < this->size; i++) {
				count[i] = 0;
			}
			for (int i = 0; i < this->size; i++) {
				int rem = ((this->arr[i] / div) % this->size);
				count[rem]++;
			}
			for (int i = 1; i < this->size; i++) {
				count[i] += count[i - 1];
			}
			// Build output array
			for (int i = this->size - 1; i >= 0; i--) {
				int rem = ((this->arr[i] / div) % this->size);
				int pos = count[rem] - 1;
				output[pos] = this->arr[i];
				count[rem]--;
			}
			// Copy output array to P
			for (int i = 0; i < this->size; i++) {
				this->arr[i] = output[i];
			}
			this->print_array();
		}
	}

	void randomizer(int min, int max) {
		for (int i = 0; i < this->size; i++) {
			this->arr[i] = range_rand(min, max);
		}
	}

	void print_array() {
		for (int i = 0; i < size; i++) {
			cout << arr[i] << " ";
		}
		cout << "\n";
	}
};

int main() {
	sorted_array new_arr = sorted_array(10);
	new_arr.randomizer(0, pow(10, 3) - 1);
	new_arr.print_array();
	new_arr.radix_sort();
	new_arr.print_array();

	return 0;
}
