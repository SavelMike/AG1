#ifndef __PROGTEST__
#include "flib.h"
#include <sys/resource.h>
#endif //__PROGTEST__

struct m_info {
    int* arr1;
    int* arr2;
    int* out;
    int arr_size;
};

#if 0
void print_array(const char* str, int* p, int64_t amnt)
{
    int i;
    
    printf("*******%s : %lld\n", str, amnt);
    for (i = 0; i < amnt; i++) {
        printf("%d ", p[i]);
    }
    printf("\n");
}
#endif

// Input parameters:
//      in1
//      in2
//      serie_size (in bytes)
//      struct merge_info*
//      out
// return value:
//      1 if nothing was read from in1 and from in2
//      0 otherwise
int merge(int in1, int in2, int serie_size, struct m_info* m_info, int out)
{    
    int index1 = 0;
    int index2 = 0;
    int index_out = 0;
    int64_t rd1 = 0;
    int64_t rd2 = 0;
    int64_t total_rd1 = 0;
    int64_t total_rd2 = 0;
    int64_t to_read;
    int nf = serie_size / m_info->arr_size;
    int rem = serie_size % m_info->arr_size;
    int cnt1 = 0;
    int cnt2 = 0;
    int in1_over = 0;
    int in2_over = 0;

    while (!in1_over || !in2_over) {
        if (rd1 == 0 && !in1_over) {
            // Read next chunk of serie from file in1
            if (cnt1 == nf) {
                to_read = rem / sizeof(int);
                in1_over = 1;
            }
            else {
                to_read = m_info->arr_size / sizeof(int);
                cnt1++;
            }
            rd1 = flib_read(in1, m_info->arr1, to_read);
            if (rd1 != to_read || to_read == 0) {
                in1_over = 1;
            }
            index1 = 0;
            total_rd1 += rd1 * sizeof(int);
            if (total_rd1 == serie_size) {
                in1_over = 1;
            }
            if (total_rd1 > serie_size) {
                printf("Something is wrong.\n");
                exit(1);
            }
        }
        if (rd2 == 0 && !in2_over) {
            if (cnt2 == nf) {
                to_read = rem / sizeof(int);
                in2_over = 1;
            }
            else {
                to_read = m_info->arr_size / sizeof(int);
                cnt2++;
            }
            // Read next chunk of serie from file in2
            rd2 = flib_read(in2, m_info->arr2, to_read);
            if (rd2 != to_read || to_read == 0) {
                in2_over = 1;
            }
            index2 = 0;
            total_rd2 += rd2 * sizeof(int);
            if (total_rd2 == serie_size) {
                in2_over = 1;
            }
            if (total_rd2 > serie_size) {
                printf("Something is wrong.\n");
                exit(1);
            }
        }
        
        // Sorting merge of m_info->arr1 and m_info->arr2 to m_info->out
        for ( ; (rd1 > 0 && rd2 > 0) || (rd1 == 0 && rd2 > 0 && in1_over) 
             || (rd2 == 0 && rd1 > 0 && in2_over); index_out++) {
            if ((index_out * sizeof(int)) == (2U * m_info->arr_size)) {
                flib_write(out, m_info->out, index_out);
                index_out = 0;
            }
            if (rd1 == 0) {
                // Serie from file in1 is read and merged completely
                m_info->out[index_out] = m_info->arr2[index2];
                index2++;
                rd2--;
                continue;
            }
            if (rd2 == 0) {
                // Serie from file in2 is read and merged completely
                m_info->out[index_out] = m_info->arr1[index1];
                index1++;
                rd1--;
                continue;
            }
            // Merge using two chunks of series
            if (m_info->arr1[index1] > m_info->arr2[index2]) {
                m_info->out[index_out] = m_info->arr2[index2];
                index2++;
                rd2--;
            }
            else {
                m_info->out[index_out] = m_info->arr1[index1];
                index1++;
                rd1--;
            }
        }
    }

    flib_write(out, m_info->out, index_out);
    
    return (total_rd1 == 0 && total_rd2 == 0);
}

int cmp(const void* a, const void* b)
{
    int a1 = *(const int*)a;
    int b1 = *(const int*)b;

    if (a1 < b1) {
        return -1;
    }
    return (a1 == b1) ? 0 : 1;
}

void copy(int32_t in_file, int32_t out_file, struct m_info *m_info)
{
    int64_t intloaded;
    
    flib_open(out_file, WRITE);
    flib_open(in_file, READ);

    while (1) {
        intloaded = flib_read(in_file, m_info->out, m_info->arr_size * 2 / sizeof(int));
        if (intloaded == 0) {
            break;
        }
        flib_write(out_file, m_info->out, intloaded);
    }

    flib_close(out_file);
    flib_close(in_file);
}

void tarant_allegra(int32_t in_file, int32_t out_file, int32_t bytes) {

    struct m_info m_info;
    int serie_size;
    // Auxillary files for merging
    uint16_t f1 = in_file + out_file + 1;
    uint16_t f2 = f1 + 1;
    uint16_t f3 = f2 + 1;
    uint16_t f4 = f3 + 1;
    int64_t intloaded;
    int tmp;
    int* buff;
    uint16_t removed1 = 65535;
    uint16_t removed2 = 65535;

    // Reserve memory for 4 simultaneously opened files, 16 bytes per each
    bytes -= 4 * 16;
    if ((unsigned)bytes < 4 * sizeof(int)) {
        printf("Too small memory heap\n");
        exit(1);
    }
    m_info.arr_size = bytes / 4 / sizeof(int) * sizeof(int);

    buff = (int*)malloc(bytes / sizeof(int) * sizeof(int));
    if (buff == NULL) {
        perror("Malloc failed");
        exit(1);
    }
    m_info.arr1 = buff;
    m_info.arr2 = buff + m_info.arr_size / sizeof(int);
    m_info.out = m_info.arr2 + (m_info.arr_size / sizeof(int));
    
    // Initial split
    flib_open(in_file, READ);
    flib_open(f1, WRITE);
    flib_open(f2, WRITE);

    // Serie must be multiple of size of int
    serie_size = bytes / sizeof(int) * sizeof(int);

    while(1) {
        intloaded = flib_read(in_file, buff, serie_size / sizeof(int));
        if (intloaded == 0) {
            break;
        }
        qsort(buff, (size_t)intloaded, sizeof(int), cmp);
        flib_write(f1, buff, intloaded);
        
        intloaded = flib_read(in_file, buff, serie_size / sizeof(int));
        if (intloaded == 0) {
            break;
        }
        qsort(buff, (size_t)intloaded, sizeof(int), cmp);
        flib_write(f2, buff, intloaded);
    }
    flib_close(in_file);
    flib_close(f1);
    flib_close(f2);

    // Main merge sorting loop 
    while (1) {
        int rc;
        int merges_nr;
        flib_open(f1, READ);
        flib_open(f2, READ);
	flib_open(f3, WRITE);
	flib_open(f4, WRITE);

	if (f3 == removed1) {
		removed1 = 65535;
	}
	if (f4 == removed2) {
		removed2 = 65535;x
	}
	
        merges_nr = 0;

        while (1) {
            rc = merge(f1, f2, serie_size, &m_info, f3);
            if (rc == 0) {
                merges_nr++;
            } else {
                break;
            }
            rc = merge(f1, f2, serie_size, &m_info, f4);
            if (rc == 0) {
                merges_nr++;
            } else {
                break;
            }
        }
        
        flib_close(f1);
        flib_close(f2);
        flib_close(f3);
	flib_close(f4);

	printf("merges_nr = %d\n", merges_nr);
        if (merges_nr == 1) {
            if (f3 != out_file) {
                copy(f3, out_file, &m_info);
            }
            printf("Sorting is over\n");
            break;
        }
        if (merges_nr == 0) {
            printf("Something is wrong\n");
            exit(1);
        }

        if (merges_nr == 2) {
            // We are about to do last merge, merge directly to out file
            removed1 = f1;
            removed2 = f2;
            flib_remove(f1);
            flib_remove(f2);
            f1 = f3;
            f3 = out_file;
        }
        else {
            tmp = f1;
            f1 = f3;
            f3 = tmp;
        }
        tmp = f2;
        f2 = f4;
        f4 = tmp;
        
        serie_size *= 2;
    }
    
    free(buff);
    
    for (tmp = 2; tmp <= 5; tmp++) {
        if (removed1 != tmp && removed2 != tmp) {
            flib_remove(tmp);
        }
    }
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
    struct rlimit rl;
    // Set stack and data limits for the program
    rl.rlim_cur = 4000;
    rl.rlim_max = 4000;
    if (setrlimit(RLIMIT_STACK, &rl) != 0) {
	    perror("Seterimit fail for stack\n");
	    exit(1);
    }
    
    rl.rlim_cur = 20000000;
    rl.rlim_max = 20000000;
    if (setrlimit(RLIMIT_DATA, &rl) != 0) {
	    perror("Setrlimit fail for data\n");
	    exit(1);
    }
    
    if (getrlimit(RLIMIT_STACK, &rl) != 0) {
	    perror("Geterimit fail for stack\n");
	    exit(1);
    }
    printf("Stack limit: rlim_cur = %ld, rlim_max = %ld\n", rl.rlim_cur, rl.rlim_max);
    
    if (getrlimit(RLIMIT_DATA, &rl) != 0) {
	    perror("Getrlimit fail for data\n");
	    exit(1);
    }
    printf("Data limit: rlim_cur = %ld, rlim_max = %ld\n", rl.rlim_cur, rl.rlim_max);

    flib_init_files(MAX_FILES);

    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 30000000;

    create_random(INPUT, SIZE);
    tarant_allegra(INPUT, RESULT, 20000000);
    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
