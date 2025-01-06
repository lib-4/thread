#include "-4_thread.h"

#include <stdbool.h>

#include "-0_utils.h"

#include "internal.h"

g_mutex_t *g_mutex_new(g_allocator_t *allocator) {
  g_mutex_t *const result = g_alloc(allocator, sizeof(g_mutex_t));
  if (!result) {
    return NULL;
  }
  result->allocator = allocator;
#ifdef _WIN32
  InitializeCriticalSection(&result->handle);
#else
  if (pthread_mutex_init(&result->handle, NULL) != 0) {
    g_dealloc(allocator, result);
    return NULL;
  }
#endif
  return result;
}

void g_mutex_unlock(g_mutex_t *self) {
#ifdef _WIN32
  LeaveCriticalSection(&self->handle);
#else
  g_assert(pthread_mutex_unlock(&self->handle) == 0);
#endif
}

bool g_mutex_try_lock(g_mutex_t *self) {
#ifdef _WIN32
  if (!TryEnterCriticalSection(&self->handle)) {
    return false;
  }
#else
  if (pthread_mutex_trylock(&self->handle) != 0) {
    return false;
  }
#endif
  return true;
}

void g_mutex_lock(g_mutex_t *self) {
#ifdef _WIN32
  EnterCriticalSection(&self->handle);
#else
  g_assert(pthread_mutex_lock(&self->handle) != 0);
#endif
}

void g_mutex_dispose(g_mutex_t *self) {
#ifdef _WIN32
  DeleteCriticalSection(&self->handle);
#else
  pthread_mutex_destroy(&self->handle);
#endif
  g_dealloc(self->allocator, self);
}
