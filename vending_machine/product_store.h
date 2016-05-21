#ifndef __PRODUCT_STORE__
#define __PRODUCT_STORE__

#include <stdint.h>
#include "lib/fifo.h"

#define PRODUCT_A 0
#define PRODUCT_B 1
#define PRODUCT_C 2
#define NO_PRODUCT_SELECTED 4

void product_store_init(FIFO_T* const in);

uint8_t getQuantityProduct(uint8_t product);

void releaseSelectedProduct();

void setSelectedProduct(uint8_t product);

uint8_t getSelectedProduct();

bool isEmpty(void);

#endif
