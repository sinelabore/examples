#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "mydefs.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include "oven.h"
#include "oven_cb.h"

/*
 * Example how to add a callback feature to the generated oven state machine
 */

/*
 * Register a callback to the oven state machine.
 * Whenever the state machine is in the given state the callback is called.
 * An ID is returned to later be able to remove the callback again.
 */
int oven_add_callback(OVEN_INSTANCEDATA_T *instanceVar, struct oven_callback *cb) {
  for (size_t n = 0; n < OVEN_MAX_CB; n++) {
    if (instanceVar->cb_array[n] == NULL) {
      instanceVar->cb_array[n] = cb;
      return n;
    }
  }
  return -1;
}

/*
 * Remove a callback from the oven state machine identified by and ID.
 */
int oven_remove_callback(OVEN_INSTANCEDATA_T *instanceVar, struct oven_callback *cb, size_t n) {
  if (n < OVEN_MAX_CB) {
    instanceVar->cb_array[n] = NULL;
    return 0;
  }
  return -1;
}

/*
 * Check if a callback has to be called. This function can be called from within the
 * oven state machine event handler functions or after exectuing the state machine event handler.
 * It iterates over the list of callbacks and calls the registered callback
 * if the machine is in the registered state.
 */
void oven_check_cb(OVEN_INSTANCEDATA_T *instanceVar) {
  for (size_t n = 0; n < OVEN_MAX_CB; n++) {
    if (instanceVar->cb_array[n] != NULL) {
      OVEN_STATES_T *ptr = &((instanceVar->cb_array[n])->s);
      if (*ptr == ovenGetInnermostActiveState(instanceVar)) {
        struct oven_callback *cb = container_of(ptr, struct oven_callback, s);
        cb->cb(cb);
      }
    }
  }
}

