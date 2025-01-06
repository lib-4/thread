#ifndef G_4_THREAD_SHORTCUT_H
#define G_4_THREAD_SHORTCUT_H

#include "thread_functions.h"

typedef g_4_thread_t g_thread_t;
typedef g_4_thread_mutex_t g_mutex_t;
typedef g_4_thread_condition_variable_t g_condition_variable_t;

#define g_thread_new g_4_thread_new
#define g_thread_join g_4_thread_join
#define g_thread_detach g_4_thread_detach
#define g_thread_exit g_4_thread_exit
#define g_mutex_new g_4_thread_mutex_new
#define g_mutex_lock g_4_thread_mutex_lock
#define g_mutex_unlock g_4_thread_mutex_unlock
#define g_mutex_try_lock g_4_thread_mutex_try_lock
#define g_mutex_dispose g_4_thread_mutex_dispose
#define g_condition_variable_new g_4_thread_condition_variable_new
#define g_condition_variable_wait g_4_thread_condition_variable_wait
#define g_condition_variable_wait_with_timeout                                 \
  g_4_thread_condition_variable_wait_with_timeout
#define g_condition_variable_signal g_4_thread_condition_variable_signal
#define g_condition_variable_broadcast g_4_thread_condition_variable_broadcast
#define g_condition_variable_dispose g_4_thread_condition_variable_dispose

#endif
