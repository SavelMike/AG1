#include "SortedArray.h"
#include <iostream>

using namespace std;

class sorted_array
{
private:
	int* arr;
	int size;

public:
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

	void print_array() {
		for (int i = 0; i < size; i++) {
			cout << arr[i] << " ";
		}
		cout << "\n";
	}
};

int main() {
	int arr[6] = { 8, 4, 1, 3, 2, 3 };
	sorted_array new_arr = sorted_array(arr, 6);
	
	new_arr.print_array();
	new_arr.bubble_sort();
	new_arr.print_array();

	return 0;
}
