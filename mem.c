#include "mem.h"

int main(void)
{

    heap_init(10000);
    struct mem *heap = (struct mem *)HEAP_START;
    heap->capacity = 0;
    heap->is_free = true;
    heap->next = NULL;

    int *test = _malloc(sizeof(int));
    *test = 5;
    float *test2 = _malloc(sizeof(double));
    *test2 = 10;
    float *test3 = _malloc(sizeof(double));
    *test3 = 10;

    mmealloc_debug_heap(stdout, (struct mem *)HEAP_START);

    return 0;
}

void *_malloc(size_t query)
{
    struct mem *curr = (struct mem *)HEAP_START;

    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    if (curr->is_free)
    {
        curr->is_free = false;
        curr->capacity = query;

        return (void *)curr + sizeof(struct mem);
    }

    struct mem *next = (void *)curr + sizeof(struct mem) + curr->capacity;
    next->capacity = query;
    next->is_free = false;
    next->next = NULL;

    curr->next = next;

    return (void *)next + sizeof(struct mem);
}

void *heap_init(size_t initial_size)
{
    return mmap(HEAP_START, initial_size, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_SHARED | 0X20, -1, 0);
}

void memalloc_debug_struct_info(FILE *f, struct mem const *const address)
{

    size_t i;
    fprintf(f, "start: %p\nsize: %lu\nis_free: %d\n", (void *)address, address->capacity, address->is_free);
    for (i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i)
        fprintf(f, "%hhX", ((char *)address)[sizeof(struct mem) + i]);

    putc('\n', f);
}

void mmealloc_debug_heap(FILE *f, struct mem const *ptr)
{
    for (; ptr; ptr = ptr->next)
    {
        memalloc_debug_struct_info(f, ptr);
    }
}