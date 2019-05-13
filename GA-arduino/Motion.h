/*
 * Motion.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include "ID.h"

#define MTN_MAX_LISTENERS (5)

typedef void (*MTN_Func)(void* context);

void  MTN_init();
ID_id MTN_addListener(MTN_Func listener, void* context);
void  MTN_removeListener(ID_id id);


#endif /* MOTION_H_ */
