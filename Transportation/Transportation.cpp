// Transportation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

struct heap_cell{
    int count;
    int from;
};

class max_heap {
private:
    struct heap_cell *mheap;
    int size;
    bool is_leaf(int i) {
        if (size / 2 >= i) {
            return false;
        }
        else {
            return true;
        }
    }
    void bubble_down(int i) {
        int child_ind;
        while (!is_leaf(i)) {
            child_ind = 2 * i;
            if ((child_ind < size) && mheap[child_ind - 1].count < mheap[child_ind + 1 - 1].count) {
                // Right child exists and is bigger than left 
                child_ind++;
            }
            if (mheap[i - 1].count >= mheap[child_ind - 1].count) {
                break;
            }
            struct heap_cell tmp = mheap[i - 1];
            mheap[i - 1] = mheap[child_ind - 1];
            mheap[child_ind - 1] = tmp;
            i = child_ind;
        }
    }
public:
    // Build heap algorithm (lecture 4, p.38)
    max_heap(int *counts, int n) {
        size = n;
        mheap = new struct heap_cell[size];
        for (int i = 0; i < size; i++) {
            mheap[i].count = counts[i];
            mheap[i].from = i;
        }
        for (int i = size / 2; i >= 1; i--) {
            bubble_down(i);
        }
    }
    ~max_heap() {
        delete[] mheap;
    }
    
    int find_max() {
        cout << mheap[0].count << "\n";
        cout << "[" << mheap[0].from << "," << mheap[0].from + 1 << "]\n";
        return mheap[0].count;
    }
};

int main()
{
    int c; // number of inspectors
    int s; // number of stops
    int p; // number of passenger records
    

    cin >> c >> s >> p;
    cout << "c = " << c << "; s = " << s << "; p = " << p << "\n";
    int *segment = new int[s - 1];
    for (int i = 0; i < s - 1; i++) {
        segment[i] = 0;
    }
    for (int i = 0; i < p; i++) {
        int from; // station where ride begins
        int to; // station where ride ends
        int count; // number of passengers expected
        cin >> from >> to >> count;
        for (int j = from; j < to; j++) {
            segment[j] += count;
        }
    }

    max_heap mheap(segment, s - 1);
    mheap.find_max();

/*    
    for (int i = 0; i < s - 1; i++) {
        cout << "from " << i << " to " << i + 1 << ": " << segment[i] << "\n";
    }
*/
    return 0;
}
