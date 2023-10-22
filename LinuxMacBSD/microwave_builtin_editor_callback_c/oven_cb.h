#ifndef __OVEN_CB_HH__
#define __OVEN_CB_HH__


// callback struct
struct oven_callback{
  void (*cb)(struct oven_callback *cb);
  OVEN_STATES_T s;
};


int oven_add_callback(OVEN_INSTANCEDATA_T *instanceVar, struct oven_callback *cb);
int oven_remove_callback(OVEN_INSTANCEDATA_T *instanceVar, struct oven_callback *cb, size_t n);
void oven_check_cb(OVEN_INSTANCEDATA_T *instanceVar);

#endif

