// Transportation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

struct pass_record {
    int from;
    int to;
    long long count;
}; 

// Max number of passengers which can be checked
long long max_num = 0;
// Array of path segments where inspectors should be at 
int* stops_to_check;
// Array of input passenger records
struct pass_record* record;
int nr_records;

long long calculate_passengers(int* segments, int k) {
    long long passcnt = 0;
    for (int i = 0; i < nr_records; i++) {
        for (int j = 0; j < k; j++) {
            if (segments[j] >= record[i].from && segments[j] < record[i].to) {
                passcnt += record[i].count;
                break;
            }
        }
    }

    return passcnt;
}

// Combinatorics combination
void combination(int *arrin, int n, int *arrout, int ind, int k) {
    if (ind == k) {
        long long num = calculate_passengers(arrout, k);
        if (num > max_num) {
            max_num = num;
            for (int i = 0; i < ind; i++) {               
                stops_to_check[i] = arrout[i];
            }
        }
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
    
    cin >> c >> s >> nr_records;
    // Elements of this array become true if segments passed passengers. 
    bool *active_segments = new bool[s - 1];
    for (int i = 0; i < s - 1; i++) {
        active_segments[i] = false;
    }

    // Read passenger records
    record = new struct pass_record[nr_records];
    for (int i = 0; i < nr_records; i++) {
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
    stops_to_check = new int[c];
    combination(act_segments, cnt, comb, 0, c);

    cout << max_num << "\n";
    for (int i = 0; i < c; i++) {
        cout << "[" << stops_to_check[i] << ";" << stops_to_check[i] + 1 << "]";
        if (i != c - 1) {
            cout << ",";
        }
    }
    cout << "\n";
    delete[] active_segments;
    delete[] act_segments;
    delete[] stops_to_check;
    delete[] comb;
    delete[] record;

    return 0;
}
