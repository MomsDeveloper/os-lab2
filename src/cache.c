#include "cache.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h> 

Cache cache = {NULL, 0};

CacheEntry *find_cache_entry(int fd, off_t offset) {
    CacheEntry *entry = cache.entries;
    while (entry) {
        if (entry->fd == fd && entry->offset == offset) {
            entry->frequency++;
            return entry;
        }
        entry = entry->next;
    }
    return NULL; // cache miss
}

void evict_lfu() {
    if (!cache.entries) return;

    CacheEntry *prev = NULL, *entry = cache.entries;
    CacheEntry *lfu_prev = NULL, *lfu_entry = cache.entries;

    while (entry) {
        if (entry->frequency < lfu_entry->frequency) {
            lfu_prev = prev;
            lfu_entry = entry;
        }
        prev = entry;
        entry = entry->next;
    }

    // if LFU page is dirty write it back to disk
    if (lfu_entry->dirty) {
        lseek(lfu_entry->fd, lfu_entry->offset, SEEK_SET);

        if (write(lfu_entry->fd, lfu_entry->data, PAGE_SIZE) < 0){
            perror("Error writing to file");
            exit(1);
        }
    }

    if (lfu_prev) {
        lfu_prev->next = lfu_entry->next;
    } else {
        cache.entries = lfu_entry->next;
    }
    free(lfu_entry);
    cache.size--;
}

void add_to_cache(int fd, off_t offset, const char *data, int dirty) {
    if (cache.size >= CACHE_SIZE) {
        evict_lfu();
    }

    CacheEntry *new_entry = malloc(sizeof(CacheEntry));
    if (posix_memalign((void **)&new_entry->data, 512, PAGE_SIZE) != 0) {
        perror("posix_memalign failed for cache entry");
        free(new_entry);
        return;
    }
    
    new_entry->fd = fd;
    new_entry->offset = offset;
    memcpy(new_entry->data, data, PAGE_SIZE);
    new_entry->frequency = 1;
    new_entry->dirty = dirty;
    new_entry->next = cache.entries;
    cache.entries = new_entry;
    cache.size++;

}
