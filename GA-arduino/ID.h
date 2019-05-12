/*
 * ID.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#ifndef ID_H_
#define ID_H_

typedef uint16_t ID_id;

#define ID_NULL (0)

void ID_init(ID_id* id);
ID_id ID_getNext(ID_id* id);

#endif /* ID_H_ */
