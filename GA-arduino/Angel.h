/*
 * Angel.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#ifndef ANGEL_H_
#define ANGEL_H_

#define ANG_MAX_NR_ANGELS  (5)
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

void ANG_init(ANG_config* cfg);
void ANG_add(char* number, char* message);
void ANG_remove(char* number);
void ANG_passive(char* number);
void ANG_active(char* number);
void ANG_getInitialConfig(ANG_config* cfg);

#endif /* ANGEL_H_ */
