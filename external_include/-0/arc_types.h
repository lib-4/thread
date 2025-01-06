#ifndef G_0_ARC_TYPES_H
#define G_0_ARC_TYPES_H

#include <stdatomic.h>

typedef struct g_0_arc {
  atomic_size_t rc;
} g_0_arc_t;

#endif
