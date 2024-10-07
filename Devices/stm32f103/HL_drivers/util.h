#ifndef UTIL_H
#define UTIL_H


#include <stdint.h>


/********************************* Variables **********************************/



/********************************* Typedefs ***********************************/
typedef enum {
    HL_SUCCESS = 0,
    HL_ERROR   = 1,
} hl_status_t;

typedef enum {
    HL_RESET = 0,
    HL_SET   = 1
} hl_state_t;





/****************************** Functions *************************************/
void HL_UTIL_InitDeviceTimer2();

void HL_UTIL_DelayMs(delay);

uint32_t HL_GetDeviceTime(void);






#endif