#include "ema-with-cache.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void test_merge(){
    int numbers_to_sort[] = {0, 1, 0, 1, 2, 5, 4, 3, 0, 1, 2};
    int numbers_sorted[] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5};

    size_t numbers_size = sizeof(numbers_to_sort) / sizeof(int);

    char *name = "./data/test_merge.bin";

    FILE *f = fopen(name, "wb");
    size_t written = fwrite(numbers_to_sort, sizeof(int), numbers_size, f);
    if (written != numbers_size) {
        perror("Error writing to file");
    }
    fclose(f);

    simple_merging_sort(name);

    f = fopen(name, "rb");
    fread(numbers_to_sort, sizeof(int), numbers_size, f);
    fclose(f);

    for (int i = 0; i < numbers_size; i++) {
        if (numbers_to_sort[i] != numbers_sorted[i]) {
            printf("* Ema-sort test failed\n");
            return;
        }
    }

    printf("* Ema-sort test passed\n");

}

void test_merge_2(){
    int numbers_to_sort[] = {0, 0, 1, 2, 1000, -1, 0, 1, 2, 5, 3, 2, 3, -1, 0, 0};
    int numbers_sorted[] = {-1, -1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 5, 1000};

    size_t numbers_size = sizeof(numbers_to_sort) / sizeof(int);

    char *name = "./data/test_merge_2.bin";

    FILE *f = fopen(name, "wb");
    size_t written = fwrite(numbers_to_sort, sizeof(int), numbers_size, f);
    if (written != numbers_size) { 
        perror("Error writing to file");
    }
    fclose(f);

    simple_merging_sort(name);

    f = fopen(name, "rb");
    fread(numbers_to_sort, sizeof(int), numbers_size, f);
    fclose(f);

    for (int i = 0; i < numbers_size; i++) {
        if (numbers_to_sort[i] != numbers_sorted[i]) {
            printf("* Ema-sort test 2 failed\n");
            return;
        }
    }

    printf("* Ema-sort test 2 passed\n");

}

int main() {
    test_merge();
    test_merge_2();
    return 0;
}