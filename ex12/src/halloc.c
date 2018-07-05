#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#define NALLOC 1024

// This header is stored at the beginning of memory segments in the list
union header {
  struct {
    union header *next;
    unsigned len;
  } meta;
  long x; // Presence forces alignment of headers in memory
};

static union header list;
static union header *first = NULL;

void free(void *ptr) {
  if (ptr == NULL) return;

  union header *iter, *block;
  iter = first;
  block = (union header *)ptr - 1;
  // Traverse to the spot in the list to insert the freed fragment,
  // such that the list is ordered by memory address (for coalescing)
  while (block <= iter || block >= iter->meta.next) {
    if ((block > iter || block < iter->meta.next) &&
        iter >= iter->meta.next) {
      break;
    }
    iter = iter->meta.next;
  }
  // If the new fragment is adjacent in memory to any others, merge
  // them (we only have to check the adjacent elements because the
  // order semantics are enforced)
  if (block + block->meta.len == iter->meta.next) {
    block->meta.len += iter->meta.next->meta.len;
    block->meta.next = iter->meta.next->meta.next;
  } else
    block->meta.next = iter->meta.next;

  if (iter + iter->meta.len == block) {
    iter->meta.len += block->meta.len;
    iter->meta.next = block->meta.next;
  } else
    iter->meta.next = block;

  first = iter;
}
unsigned check_true_size(unsigned true_size) {
  // We have to request memory of at least a certain size
  if (true_size < NALLOC)
    true_size = NALLOC;
  return true_size;
}

union header *split_fragment(union header *p, union header *prev, unsigned true_size) {
  if (p->meta.len == true_size)
    prev->meta.next = p->meta.next; // If the fragment is exactly the right size, we do not have to split it
  else {
    // Otherwise, split the fragment, returning the first half and
    // storing the back half as another element in the list
    p->meta.len -= true_size;
    p += p->meta.len;
    p->meta.len = true_size;
  }
  first = prev;
  return p;
}

void create_fragment(unsigned true_size, union header *p, char *page) {
  // Create a fragment from this new memory and add it to the list
  // so the above logic can handle breaking it if necessary
  union header *block = (union header *)page;
  block->meta.len = true_size;
  free((void *)(block + 1));
  p = first;
}

void *halloc(size_t size) {
  union header *p, *prev;
  prev = first;
  unsigned true_size = (size + sizeof(union header) - 1) / sizeof(union header) + 1;
  if (size <= 0)
    return NULL;
  //If the list of previously allocated fragments is empty, initialize it.
  if (first == NULL) {
    prev = &list;
    first = prev;
    list.meta.next = first;
    list.meta.len = 0;
  }
  p = prev->meta.next;
  // Traverse the list of previously allocated fragments, searching for one sufficiently large to allocate.
  while (1) {
    if (p->meta.len >= true_size) {
      p = split_fragment(p, prev, true_size);
      return (void *)(p + 1);
    }
    //If we reach the beginning of the list, no satisfactory fragment was found, so we have to request a new one
    if (p == first) {
      true_size = check_true_size(true_size);
      char *page = sbrk((intptr_t) (true_size * sizeof(union header)));
      if (page == (char *)-1) {
        errno = ENOMEM;// There was no memory left to allocate
        return NULL;
      }
      create_fragment(true_size, p, page);
    }
    prev = p;
    p = p->meta.next;
  }
  return NULL;
}

