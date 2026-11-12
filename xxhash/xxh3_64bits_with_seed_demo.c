#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <xxhash.h>
#include <sys/random.h>

int main(void) {
    const char *key = "user:12345";
    size_t key_len = strlen(key);

    /* Generate one random 64-bit seed at process startup. */
    uint64_t seed;
    if (getrandom(&seed, sizeof (seed), 0) != sizeof (seed)) {
        /* Fallback for systems without getrandom(2). */
        seed = (uint64_t)time(NULL) ^ (uint64_t)(uintptr_t)&seed;
        seed *= 0x9E3779B97F4A7C15ULL;
    }

    uint64_t hash = XXH3_64bits_withSeed(key, key_len, seed);

    /* For a power-of-two table capacity. */
    size_t capacity = 1u << 20; /* 1,048,576 buckets */
    size_t bucket = (size_t)(hash & (capacity - 1));

    printf("hash:   0x%016llx\n", (unsigned long long)hash);
    printf("bucket: %zu\n", bucket);
    return 0;
}
/*
- Compile using: gcc -O2 hash_demo.c -o hash_demo -lxxhash
- Structure if XXH64_hash_t is an unsigned 64-bit integer.
- Function of XXH3_64bits_withSeed(): 
    XXH64_hash_t XXH3_64bits_withSeed(const void *input, size_t length, XXH64_hash_t seed);
*/