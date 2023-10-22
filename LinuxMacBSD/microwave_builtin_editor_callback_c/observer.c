/*
 * Example Observer getting callbacks from the oven state machine
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "observer.h"
#include "oven_ext.h"
#include "oven.h"
#include "oven_dbg.h"
#include "oven_cb.h"

// callback
static void observer_on_machine_cb(struct oven_callback *cb) {
  struct observer *self = container_of(cb, struct observer, cb);
  printf("Observer callback #%d for machine entering state %s\n", self->cb_id, ovenGetNameByState(self->cb.s));
}

void observer_init(struct observer *self, OVEN_INSTANCEDATA_T *machine, OVEN_STATES_T state) {
  self->sm = machine;
  self->cb.cb = observer_on_machine_cb;
  self->cb.s = state;
  self->cb_id = oven_add_callback(self->sm, &self->cb);
}

void observer_deinit(struct observer *self) {
  oven_remove_callback(self->sm, &self->cb, self->cb_id);
}

