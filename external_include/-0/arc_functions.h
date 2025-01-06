#ifndef G_0_ARC_FUNCTIONS_H
#define G_0_ARC_FUNCTIONS_H

#include "arc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void g_0_arc_increase(g_0_arc_t *arc);
void g_0_arc_decrease(g_0_arc_t *arc, void (*dispose)(void *context),
                      void *context);

#ifdef __cplusplus
}
#endif

#endif
