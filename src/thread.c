#include "-4_thread.h"

#include "-0_utils.h"

#include "internal.h"

static void dispose(void *context) {
  g_thread_t *const self = (g_thread_t *)context;
  g_dealloc(self->allocator, self);
}

static void g_thread_dispose(g_thread_t *self) {
  g_arc_decrease(&self->context_wrapper_arc, dispose, self);
}

#ifdef _WIN32
static DWORD routine_wrapper(void *context) {
  g_thread_t *const actual = (g_thread_t *)context;
  g_err_t (*const actual_routine)(void *) = actual->actual_routine;
  void *const actual_context = actual->actual_context;
  g_thread_dispose(actual);
  return (DWORD)actual_routine(actual_context);
}
#else
static void *routine_wrapper(void *context) {
  g_thread_t *const actual = (g_thread_t *)context;
  g_err_t (*const actual_routine)(void *) = actual->actual_routine;
  void *const actual_context = actual->actual_context;
  g_thread_dispose(actual);
  return (void *)(uintptr_t)actual_routine(actual_context);
}
#endif

g_thread_t *g_thread_new(g_0_allocator_t *allocator,
                         g_err_t (*routine)(void *context), void *context) {
  g_thread_t *const result = g_alloc(allocator, sizeof(g_thread_t));
  if (!result) {
    return NULL;
  }
  result->allocator = allocator;
  result->actual_context = context;
  result->actual_routine = routine;
  result->context_wrapper_arc.rc = 2;
#ifdef _WIN32
  result->handle = CreateThread(NULL, 0, routine_wrapper, result, 0, NULL);
  if (result->handle == NULL) {
    g_dealloc(allocator, result);
    return NULL;
  }
#else
  if (pthread_create(&result->handle, NULL, routine_wrapper, result) != 0) {
    g_dealloc(allocator, result);
    return NULL;
  }
#endif
  return result;
}

void g_thread_exit(void) {
#ifdef _WIN32
  ExitThread(0);
#else
  pthread_exit(NULL);
#endif
}

void g_thread_join(g_thread_t *self) {
#ifdef _WIN32
  DWORD waitResult = WaitForSingleObject(self->handle, INFINITE);
  g_assert(waitResult == WAIT_OBJECT_0);
  g_assert(CloseHandle(self->handle));
#else
  g_assert(pthread_join(self->handle, NULL) == 0);
#endif
  g_thread_dispose(self);
}

void g_thread_detach(g_thread_t *self) {
#ifdef _WIN32
  g_assert(CloseHandle(self->handle));
#else
  g_assert(pthread_detach(self->handle) == 0);
#endif
  g_thread_dispose(self);
}
