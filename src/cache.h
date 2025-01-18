#ifndef CACHE_H
#define CACHE_H

#include <stddef.h>
#include <unistd.h>

#define CACHE_SIZE 512 // Number of pages in the cache
#define PAGE_SIZE 4096 // Size of a single page
#define ALIGNMENT 512 // For `O_DIRECT`

typedef struct CacheEntry {
    int fd;              
    off_t offset;        
    char* data;
    int frequency;       
    int dirty;           // dirty flag (1 if modified)
    struct CacheEntry *next;
} CacheEntry;

typedef struct Cache {
    CacheEntry *entries; // linked list of cache entries
    int size;            // current number of cached pages
} Cache;

extern Cache cache;


CacheEntry *find_cache_entry(int fd, off_t offset); 
void evict_lfu();
void add_to_cache(int fd, off_t offset, const char *data, int dirty);

#endif

