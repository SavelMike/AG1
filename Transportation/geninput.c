#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

long int range_rand(long int min, long int max) {
	long int rc = 0;
	assert(min <= max);
	rc = (rand() % (max - min + 1)) + min;
	assert(rc >= min && rc <= max);
	
	return rc;
}

int main() {
	long int p;
	long int c;
	long int s;
	long int i;
	long int from;
	long int to;

	srand(time(NULL));
	printf("%ld %ld %ld\n", c = range_rand(1, 5), s = range_rand(2, 15), p = range_rand(0, 100));
	
	for (i = 0; i < p; i++) {
		from = range_rand(0, s - 2);
		to = range_rand(from + 1, s - 1);
		printf("%ld %ld %ld\n", from, to, range_rand(0, 20000)); 
	}
	
	return 0;
}
