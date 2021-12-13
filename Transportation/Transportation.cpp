// Transportation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

struct pass_record {
    int from;
    int to;
    long long count;
};

void combination(int *arrin, int n, int *arrout, int ind, int k) {
    if (ind == k) {
        for (int i = 0; i < ind; i++) {
            cout << arrout[i] << ",";
        }
        cout << "\n";
        return;
    }
    for (int i = 0; i < n; i++) {
        arrout[ind] = arrin[i];
        combination(arrin + i + 1, n - i - 1, arrout, ind + 1, k);
    }
}

int main()
{
    int c; // number of inspectors
    int s; // number of stops
    int p; // number of passenger records
    
    cin >> c >> s >> p;
    // Elements of this array become true if segments passed passengers. 
    bool *active_segments = new bool[s - 1];
    for (int i = 0; i < s - 1; i++) {
        active_segments[i] = false;
    }

    // Read passenger records
    struct pass_record *record = new struct pass_record[p];
    for (int i = 0; i < p; i++) {
        cin >> record[i].from >> record[i].to >> record[i].count;
        for (int j = record[i].from; j < record[i].to; j++) {
            active_segments[j] = true;
        }
    }

    //Calculate number of active segments
    int cnt = 0;
    for (int i = 0; i < s - 1; i++) {
        if (active_segments[i] == true) {
            cnt++;
        }
    }
    if (c > cnt) {
        c = cnt;
    }
    // Array of active segments
    int* act_segments = new int[cnt];
    for (int i = 0, j = 0; i < s - 1; i++) {
        if (active_segments[i] == true) {
            act_segments[j++] = i;
        }
    }
    int* comb = new int[c];
    combination(act_segments, cnt, comb, 0, c);

    return 0;
}
