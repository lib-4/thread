#include "-4_thread.h"

#include <stdbool.h>
#include <stdlib.h>

#include "-0_utils.h"

#include "internal.h"

g_condition_variable_t *g_condition_variable_new(g_allocator_t *allocator) {
  g_condition_variable_t *const result =
      g_alloc(allocator, sizeof(g_condition_variable_t));
  if (!result) {
    return NULL;
  }
  result->allocator = allocator;
#ifdef _WIN32
  InitializeConditionVariable(&result->cond);
#else
  if (pthread_cond_init(&result->cond, NULL) != 0) {
    g_dealloc(allocator, result);
    return NULL;
  }
#endif
  return result;
}

void g_condition_variable_wait(g_condition_variable_t *self, g_mutex_t *mutex) {
#ifdef _WIN32
  CRITICAL_SECTION *const actual_mutex = &mutex->handle;
  g_assert(SleepConditionVariableCS(&self->cond, actual_mutex, INFINITE));
#else
  pthread_mutex_t *const actual_mutex = &mutex->handle;
  g_assert(pthread_cond_wait(&self->cond, actual_mutex) == 0);
#endif
}

void g_condition_variable_wait_with_timeout(g_condition_variable_t *self,
                                            g_mutex_t *mutex,
                                            unsigned int timeout_millis,
                                            bool *out_timeout_occurred) {
#ifdef _WIN32
  CRITICAL_SECTION *const actual_mutex = &mutex->handle;
  DWORD result =
      SleepConditionVariableCS(&self->cond, actual_mutex, timeout_millis);
  if (result == 0) {
    DWORD error = GetLastError();
    if (error == ERROR_TIMEOUT) {
      if (out_timeout_occurred) {
        *out_timeout_occurred = true;
      }
      return;
    }
    abort();
  }
#else
  pthread_mutex_t *const actual_mutex = &mutex->handle;
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  ts.tv_sec += timeout_millis / 1000;
  ts.tv_nsec += (timeout_millis % 1000) * 1000000;
  if (ts.tv_nsec >= 1000000000) {
    ts.tv_sec += 1;
    ts.tv_nsec -= 1000000000;
  }

  int result = pthread_cond_timedwait(&self->cond, actual_mutex, &ts);
  if (result == ETIMEDOUT) {
    if (out_timeout_occurred) {
      *out_timeout_occurred = true;
    }
    return;
  }
  g_assert(result == 0);
#endif
  if (out_timeout_occurred) {
    *out_timeout_occurred = false;
  }
}

void g_condition_variable_signal(g_condition_variable_t *self) {
#ifdef _WIN32
  WakeConditionVariable(&self->cond);
#else
  g_assert(pthread_cond_signal(&self->cond) == 0);
#endif
}

void g_condition_variable_broadcast(g_condition_variable_t *self) {
#ifdef _WIN32
  WakeAllConditionVariable(&self->cond);
#else
  g_assert(pthread_cond_broadcast(&self->cond) == 0);
#endif
}

void g_condition_variable_dispose(g_condition_variable_t *self) {
#ifdef _WIN32
  // No explicit destroy needed for CONDITION_VARIABLE in Windows
#else
  pthread_cond_destroy(&self->cond);
#endif
  g_dealloc(self->allocator, self);
}
