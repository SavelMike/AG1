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

    int heap_extract_max() {
        if (size == 0) {
            return -1;
        }
        
//        cout << mheap[0].count << "\n";
//        cout << "[" << mheap[0].from << "," << mheap[0].from + 1 << "]\n";
        int from = mheap[0].from;
        size--;
        if (size > 0) {
            mheap[0] = mheap[size - 1];
            bubble_down(1);
        }

        return from;
    }
};

struct pass_record {
    int from;
    int to;
    int count;
};

int main()
{
    int c; // number of inspectors
    int s; // number of stops
    int p; // number of passenger records
    
    cin >> c >> s >> p;
    int *segment = new int[s - 1];
    for (int i = 0; i < s - 1; i++) {
        segment[i] = 0;
    }

    struct pass_record *record = new struct pass_record[p];
    for (int i = 0; i < p; i++) {
        cin >> record[i].from >> record[i].to >> record[i].count;
        for (int j = record[i].from; j < record[i].to; j++) {
            segment[j] += record[i].count;
        }
    }
    int* froms = new int[c];

    max_heap mheap(segment, s - 1);
    for (int i = 0; i < c; i++) {
        froms[i] = mheap.heap_extract_max();
    }
    // Calculate number of passengers which travelled from froms[0]-froms[0] + 1, from[1]-from[1] + 1, etc
    int npass = 0;
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < c; j++) {
            if (record[i].from <= froms[j] && froms[j] < record[i].to) {
                npass += record[i].count;
                break;
            }
        }
    }
    cout << npass << "\n";
    for (int i = 0; i < c; i++) {
        cout << "[" << froms[i] << ";" << froms[i] + 1 << "]";
        if (i != c - 1) {
            cout << ",";
        }
    }
    cout << "\n";

    delete[] segment;
    delete[] froms;
    delete[] record;
    
    return 0;
}
