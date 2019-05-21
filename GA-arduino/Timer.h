/*
 * Timer.h
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */


#include <stdint.h>

#include "ID.h"

#define TMR_MAX_NR_CALLBACKS (16)

typedef void (*TMR_cb)(void*);



void TMR_init();
// 1 period = 1ms
ID_id TMR_registerCB(TMR_cb cb, void* context, uint16_t period);

