#ifndef G_4_THREAD_FUNCTIONS_H
#define G_4_THREAD_FUNCTIONS_H

#include "thread_types.h"

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "-0_allocator.h"
#include "-0_common.h"

g_4_thread_t *g_4_thread_new(g_0_allocator_t *allocator,
                             g_err_t (*routine)(void *context), void *context);
void g_4_thread_join(g_4_thread_t *self);
void g_4_thread_detach(g_4_thread_t *self);
void g_4_thread_exit(void);

g_4_thread_mutex_t *g_4_thread_mutex_new(g_0_allocator_t *allocator);
void g_4_thread_mutex_lock(g_4_thread_mutex_t *self);
void g_4_thread_mutex_unlock(g_4_thread_mutex_t *self);
bool g_4_thread_mutex_try_lock(g_4_thread_mutex_t *self);
void g_4_thread_mutex_dispose(g_4_thread_mutex_t *self);

g_4_thread_condition_variable_t *
g_4_thread_condition_variable_new(g_0_allocator_t *allocator);
void g_4_thread_condition_variable_wait(g_4_thread_condition_variable_t *self,
                                        g_4_thread_mutex_t *mutex);
void g_4_thread_condition_variable_wait_with_timeout(
    g_4_thread_condition_variable_t *self, g_4_thread_mutex_t *mutex,
    unsigned int timeout_millis, bool *out_timeout_occurred);
void g_4_thread_condition_variable_signal(
    g_4_thread_condition_variable_t *self);
void g_4_thread_condition_variable_broadcast(
    g_4_thread_condition_variable_t *self);
void g_4_thread_condition_variable_dispose(
    g_4_thread_condition_variable_t *self);

#endif
