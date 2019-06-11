/*
 * Angel_cfg.h
 *
 *  Created on: 11 jun. 2019
 *      Author: wilbert
 */

#ifndef ANGEL_CFG_H_
#define ANGEL_CFG_H_


#define ANG_MAX_NR_ANGELS  (3)
#define ANG_NUMBERSIZE     (10)
#define ANG_MESSAGESIZE    (160)


typedef enum {
	ANG_STATE_EMPTY, ANG_STATE_PASSIVE, ANG_STATE_ACTIVE
} ang_StateEnum;

typedef struct {
	char phonenumber[ANG_NUMBERSIZE + 1];
	char message[ANG_MESSAGESIZE + 1];
	ang_StateEnum state;
} ang_AngelStruct;


typedef struct
{
	ang_AngelStruct angels[ANG_MAX_NR_ANGELS];
} ANG_config;



#endif /* ANGEL_CFG_H_ */
