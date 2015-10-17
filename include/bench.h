#ifndef _BENCH_H_
#define _BENCH_H_

typedef unsigned long long u64;
typedef unsigned int uint;
typedef unsigned long long ull;

#define PARENT_FILE "parent.txt"
#define CHILD_FILE "child.txt"

#define RDTSC(result) \
        asm volatile("rdtsc" : "=a" (low), "=d" (high)); \
        result = ((ull)low) | ((ull)high << 32);

#endif
