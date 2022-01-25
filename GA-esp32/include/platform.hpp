/*
 * platform.hpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_PLATFORM_HPP_
#define INCLUDE_PLATFORM_HPP_


// 'undefine' GA_POSIX for AVR ESP32 platform.

#ifdef GA_POSIX

#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

#define LOG(msg) std::cout << msg << std::endl;

#else

#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 5
#ifdef LOG_LOCAL_LEVEL
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <Arduino.h>

// #define LOG(msg) /* Serial.printf(msg) */
// #define LOG(msg)  Serial.printf(msg); Serial.printf("\n")
#define LOG(msg, ...)  ESP_LOGD("GA", "= ( " msg ")", ##__VA_ARGS__)
#define LOG_ENTRY(msg, ...) ESP_LOGD("GA","> ("  msg  ")", ##__VA_ARGS__)
#define LOG_EXIT(msg, ...) ESP_LOGD("GA","< ("  msg  ")", ##__VA_ARGS__)
#endif


#define TIME24_NORMAL // TIME24_DEBUG


#endif /* INCLUDE_PLATFORM_HPP_ */
