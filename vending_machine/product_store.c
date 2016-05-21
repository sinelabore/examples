//
// This code simulates the product store of a vending machine
// Only code added that is absolutely needed to run the simulation 
//
// www.sinelabore.com
//


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "product_store.h"
#include "product_store_sm_ext.h"

// simulate an initial stock of product A,B,C
uint8_t numProducts[3]={3,2,1};

FIFO_T* fifo2Self; // incoming events

uint8_t selectedProduct=NO_PRODUCT_SELECTED;

void product_store_init(FIFO_T* const in){
		fifo2Self = in;
}

uint8_t getQuantityProduct(uint8_t product){
	// no error handing here
	return numProducts[product];
}

void releaseSelectedProduct(){

	// enque the event to handle the release process
	fifoPut(fifo2Self, evReleaseSelectedProduct);
	
	// update stocked items counters
	// no bounds check here ...
	if(numProducts[getSelectedProduct()]>0){
		numProducts[getSelectedProduct()]--;
		
		if(numProducts[getSelectedProduct()]==0){
			printf("ProductStore:: Product empty\n");
		}else{
			printf("ProductStore::Still %d in store\n",numProducts[getSelectedProduct()]);
		}
	}
	
}

void setSelectedProduct(uint8_t product){
	selectedProduct = product;
}

uint8_t getSelectedProduct(){
	return selectedProduct;
}

bool isEmpty(void){
	if(getQuantityProduct(PRODUCT_A)==0 && getQuantityProduct(PRODUCT_B)==0 && getQuantityProduct(PRODUCT_C)==0)
		return 1;
	else
		return 0;
}
