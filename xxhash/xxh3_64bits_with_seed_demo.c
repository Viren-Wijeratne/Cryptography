//Simple program using the xxhash library - Everytime the program runs the hash will be different, not suitable to be used with a hashtable

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <xxhash.h>

int main(void) {
    const char *key = "user:12345";
    size_t key_len = strlen(key);

    //Entropy created by the time and the seed pointer casted to uint64_t and XORed together
    uint64_t seed = (uint64_t)time(NULL) ^ (uint64_t)(uintptr_t)&seed;
    printf("Seed: %u\n", seed);

    uint64_t hash1 = XXH3_64bits_withSeed(key, key_len, seed);

    uint64_t hash = XXH3_64bits_withSeed(key, key_len, hash1);

    printf("hash 1:   0x%016llx\n", (unsigned long long)hash1);
    printf("hash 2:   0x%016llx\n", (unsigned long long)hash);
    
    return 0;
}
/*
- Compile using: gcc -O2 hash_demo.c -o hash_demo -lxxhash
- Structure if XXH64_hash_t is an unsigned 64-bit integer.
- Function of XXH3_64bits_withSeed(): 
XXH64_hash_t XXH3_64bits_withSeed(const void *input, size_t length, XXH64_hash_t seed);

- When considering the specific hash function when the number of times it is invoked increases the probabilty for collisions increases as well
For the XXH3_64bits_withSeed function
+-------------+---------------------------+
|  Enteries   | Expected 64-bit collisions|
+-------------+---------------------------+
| 1 million   | Essentially zero          |
| 100 million | About 0.00027             |
| 1 billion   | About 0.027               |
| 100 billion | About 270                 | 
+-------------+---------------------------+

- Suppose we want to limit the range of the generated hashes to say 1 million to be used as an index for an array of size 1 million 
- Normally we would do something like; hash % (capacity - 1) But doing it this way takes alot more CPU cycles compared too something like hash & (capacity - 1) which takes less 
- The & operation keeps only the lowest 20 bits of the 64-bit hash and sets every higher bit to zero. The result is therefore guaranteed to be in the range 0 through 1,048,575—exactly the valid bucket/index range
    size_t capacity = 1u << 20; //This mask is used to limit the entries to 1,048,576 indexes/buckets (2^20) 
    size_t bucket = (size_t)(hash & (capacity - 1));
- So the full hash is still used for collision handling and key comparison; the mask merely selects which bucket it starts in.
- Above reference: https://theruntimetheory.com/interviews/dsa/why-hash-tables-use-power-of-two-capacity
    */