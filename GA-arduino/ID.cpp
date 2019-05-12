/*
 * ID.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#include <stdint.h>

#include "ID.h"

void ID_init(ID_id* id)
{
	*id = ID_NULL;
}

ID_id ID_getNext(ID_id* id)
{
	do {
		(*id)++;
	} while ((*id) == ID_NULL);
	return (*id);
}
