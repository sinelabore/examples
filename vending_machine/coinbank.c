
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "coinbank.h"

uint8_t numTenCentCoins = 20;
uint8_t numTwentyCentCoints = 10;
uint8_t numFifetyCentCoins = 4;



void releaseCoins(uint8_t centsToRelease){
	printf("Releasing %d\n",centsToRelease);
}
