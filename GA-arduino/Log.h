/*
 * log.h
 *
 *  Created on: 10 jun. 2019
 *      Author: wilbert
 */

#ifndef LOG_H_
#define LOG_H_

#define _log(x) Serial.println(F(x));

#define LOG_entry(x) Serial.print(F("> ")); \
	                 _log(x);

#define LOG_exit(x)  Serial.print(F("< ")); \
	                 _log(x);

#define LOG(x)       Serial.print(F("= ")); \
					 _log(x)

#define LOG_nf(x)    Serial.print(x);

#endif /* LOG_H_ */
