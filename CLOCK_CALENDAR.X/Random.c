#include "Random.h"

extern void Init_Random_Number(void){
    srand(time(NULL));
}

extern uint16_t Get_Random_Number(uint16_t lowerLimit, uint16_t upperLimit){
    return ( (rand() % (upperLimit - lowerLimit + 1U)) + lowerLimit);
}
