#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "cache.h"
#include "file_operations.h"

int lab2_open(const char *path) {
    int fd = open(path, O_RDWR | O_CREAT | O_DIRECT);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }
    return fd;

}


int lab2_close(int fd) {
    lab2_fsync(fd);
    return close(fd);

}

ssize_t lab2_read(int fd, void *buf, size_t count) {
    off_t offset = lseek(fd, 0, SEEK_CUR);
    size_t total_read = 0;

    if (offset % PAGE_SIZE != 0 || count % PAGE_SIZE != 0) {
        fprintf(stderr, "Offset or size is not aligned to PAGE_SIZE\n");
        return -1;
    }

    while (count > 0) {
        CacheEntry *entry = find_cache_entry(fd, offset);
        if (!entry) {
            // Cache miss: Read from disk
            char *page;
            if (posix_memalign((void **)&page, ALIGNMENT, PAGE_SIZE) != 0) {
                perror("posix_memalign failed");
                return -1;
            }

            if (pread(fd, page, PAGE_SIZE, offset) < 0) {
                perror("pread failed");
                free(page);
                return -1;
            }

            add_to_cache(fd, offset, page, 0);
            entry = find_cache_entry(fd, offset);
            free(page);
        }

        memcpy(buf + total_read, entry->data, PAGE_SIZE);

        total_read += PAGE_SIZE;
        count -= PAGE_SIZE;
        offset += PAGE_SIZE;
    }
    lseek(fd, offset, SEEK_SET);
    return total_read;
}

ssize_t lab2_write(int fd, const void *buf, size_t count) {
    off_t offset = lseek(fd, 0, SEEK_CUR);
    size_t total_written = 0;

    if (offset % PAGE_SIZE != 0 || count % PAGE_SIZE != 0) {
        fprintf(stderr, "Offset or size is not aligned to PAGE_SIZE\n");
        return -1;
    }

    while (count > 0) {
        CacheEntry *entry = find_cache_entry(fd, offset);
        if (!entry) {
            // Cache miss: Load page from disk
            char *page;
            if (posix_memalign((void **)&page, ALIGNMENT, PAGE_SIZE) != 0) {
                perror("posix_memalign failed");
                return -1;
            }

            if (pread(fd, page, PAGE_SIZE, offset) < 0) {
                perror("pread failed");
                free(page);
                return -1;
            }

            add_to_cache(fd, offset, page, 0);
            entry = find_cache_entry(fd, offset);
            free(page);
        }

        memcpy(entry->data, buf + total_written, PAGE_SIZE);
        entry->dirty = 1;

        total_written += PAGE_SIZE;
        count -= PAGE_SIZE;
        offset += PAGE_SIZE;

    }
    lseek(fd, offset, SEEK_SET);
    return total_written;
}

off_t lab2_lseek(int fd, off_t offset, int whence) {
    return lseek(fd, offset, whence);
}

int lab2_fsync(int fd) {
    CacheEntry *entry = cache.entries;
    while (entry) {
        if (entry->fd == fd && entry->dirty) {
            lseek(fd, entry->offset, SEEK_SET);
            if (write(fd, entry->data, PAGE_SIZE) < 0)
            {
                perror("Error writing");
                exit(1);
            }
            entry->dirty = 0;
        }
        entry = entry->next;
    }
    return fsync(fd);
}
