#ifndef __PROGTEST__
#include "flib.h"
#endif //__PROGTEST__

struct merge_info {
    int* arr1;
    int* arr2;
    int* out;
    int arr_size;
};

// Pseudocode merge function
// Input parameters:
//      in1
//      in2
//      serie_size
//      struct merge_info*
//      out
void merge(int in1, int in2, int serie_size, struct m_info* m_info)
{    
    int index1 = m_info->arr_size;
    int index2 = m_info->arr_size;
    int index_out = 0;
    int rd1 = index1;
    int rd2 = index2;
    int total_rd1 = 0;
    int total_rd2 = 0;
    int to_read;

    while (rd1 != 0 || rd2 != 0) {
        if (index1 == rd1) {
            // Read next chunk of serie from file in1
            to_read = m_info->arr_size; 
            if (total_rd1 + to_read > serie_size) {
                to_read = total_rd1 + to_read - serie_size;
            }
            rd1 = read(in1, m_info->arr1, to_read);
            index1 = 0;
            total_rd1 += rd1;
        }
        if (index2 == rd2) {
            // Read next chunk of serie from file in2
            to_read = m_info->arr_size;
            if (total_rd2 + to_read > serie_size) {
                to_read = total_rd2 + to_read - serie_size;
            }
            rd2 = read(in2, m_info->arr2, to_read);
            index2 = 0;
            total_rd2 += rd2;
        }
        
        // Sorting merge of m_info->arr1 and m_info->arr2 to m_info->out
        for ( ; (index1 < rd1 && index2 < rd2) || (rd1 + rd2 != 0); index_out++) {
            if (rd1 == 0) {
                // Serie from file in1 is read and merged completely
                m_info->out[index_out] = m_info->arr2[index2];
                index2++;
                continue;
            }
            if (rd2 == 0) {
                // Serie from file in2 is read and merged completely
                m_info->out[index_out] = m_info->arr1[index1];
                index1++;
                continue;
            }
            // Merge using two chunks of series
            if (m_info->arr1[index1] > m_info->arr2[index2]) {
                m_info->out[index_out] = m_info->arr2[index2];
                index2++;
            }
            else {
                m_info->out[index_out] = m_info->arr2[index1];
                index1++;
            }
        }
    }
}

void tarant_allegra(int32_t in_file, int32_t out_file, int32_t bytes) {

    struct merge_info m_info;
    int serie_size;

    m_info.arr_size = bytes / 4 / sizeof(int) * sizeof(int);
    m_info.arr1 = malloc(m_info.arr_size);
    m_info.arr2 = malloc(m_info.arr_size);
    m_info.out = malloc(m_info.arr_size * 2);
    if (m_info.arr1 == NULL || m_info.arr2 == NULL || m_info.out == NULL) {
        perror("Malloc failed");
        exit(1);
    }
    // Pseudocode
    // Open aux file1
    // Open aux file2
    // open in_file

while(!eof in_file) {
    //      read bytes bytes
    //      sort read data
    //      write to aux1
    //      read bytes bytes
    //      sort read data
    //      write to aux2
    // }
    // close in_file
    // close aux1
    // close aux2
    // 
    // serie_size = bytes / sizeof(int)
    serie_size = m_info.arr_size / sizeof(int);
    while (1) {
    
    //      open in1
    //      open in2
    //      create out1
    //      create out2
        merge(in1, in2, serie_size, &m_info);
        write(out1, m_info.out, ?);
        merge(in1, in2, serie_size, &m_info);
        write(out2, m_info.out, ? );
    //      close in1
    //      close in2
    //      rm in1, in2
        serie_size *= 2;
    }
    free(m_info.arr1);
    free(m_info.arr2);
    free(m_info.out);
}

#ifndef __PROGTEST__

uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
    /* srand(time(NULL)); */
#define STEP 100ll
    int val[STEP];
    for(int i=0; i<size; i+=STEP){
        for(int j=0; j<STEP && i+j < size; ++j){
            val[j]=-1000 + (rand()%(2*1000+1));
            total_sum_mod += val[j];
        }
        flib_write(output, val, (STEP < size-i) ? STEP : size-i);
    }
    flib_close(output);
}

void tarant_allegra ( int in_file, int out_file, int bytes );

void check_result ( int out_file, int SIZE ){
    flib_open(out_file, READ);
    int q[30], loaded, last=-(1<<30), total=0;
    uint64_t current_sum_mod=0;
    while(loaded = flib_read(out_file, q, 30), loaded != 0){
        total += loaded;
        for(int i=0; i<loaded; ++i){
            if(last > q[i]){
                printf("the result file contains numbers %d and %d on position %d in the wrong order!\n", last, q[i], i-1);
                exit(1);
            }
            last=q[i];
            current_sum_mod += q[i];
        }
    }
    if(total != SIZE){
        printf("the output contains %d but the input had %d numbers\n", total, SIZE); exit(1);
    }
    if(current_sum_mod != total_sum_mod){
        printf("the output numbers are not the same as the input numbers\n");
        exit(1);
    }
    flib_close(out_file);
}

int main(int argc, char **argv){
    const uint16_t MAX_FILES = 65535;
    flib_init_files(MAX_FILES);
    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 140;

    create_random(INPUT, SIZE);
    tarant_allegra(INPUT, RESULT, 1000);
    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
