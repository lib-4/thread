#

#include "-4_thread.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "-0_allocator.h"
#include "-0_arc.h"

struct g_4_thread {
  g_allocator_t *allocator;
#ifdef _WIN32
  HANDLE handle;
#else
  pthread_t handle;
#endif
  void *actual_context;
  g_err_t (*actual_routine)(void *context);
  g_arc_t context_wrapper_arc;
};

struct g_4_thread_mutex {
  g_allocator_t *allocator;
#ifdef _WIN32
  CRITICAL_SECTION handle;
#else
  pthread_mutex_t handle;
#endif
};

struct g_4_thread_condition_variable {
  g_allocator_t *allocator;
#ifdef _WIN32
  CONDITION_VARIABLE cond;
#else
  pthread_cond_t cond;
#endif
};
