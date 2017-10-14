#include "my_allocator.h"
#include <sys/mman.h>


static void **ptr = nullptr;

static void *alloc() {
    void *mem = mmap(nullptr, 4096, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ptr = (void **) mem;
    for (auto i = 0; i < 4096; i += 128) {
        auto cur = (char *) mem + i;
        *(void **)cur = 0;
        if (i != 0) *(void **)(cur - 128) = cur;
    }
    return ptr;
}

void* get_next() {
    if (ptr == nullptr) {
        alloc();
    }
    void *ans = ptr;
    ptr = (void**)*ptr;
    return ans;
}

void free_ptr(void* old_ptr) {
    *(void **) old_ptr = ptr;
    ptr = (void **) old_ptr;    
}
