#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "mydefs.h"
#include "oven_ext.h"
#include "oven_hlp.h"
#include "oven.h"
#include "oven_cb.h"
#include "observer.h"

// structure representing the observer
struct observer{
  OVEN_INSTANCEDATA_T *sm;  // state machine instance
  struct oven_callback cb;  // our callback
  int cb_id;  // id of the registered callback
};

// initialize the observer and set callback
void observer_init(struct observer *self, OVEN_INSTANCEDATA_T *machine, OVEN_STATES_T state);

// unset the callback
void observer_deinit(struct observer *self);


#endif

