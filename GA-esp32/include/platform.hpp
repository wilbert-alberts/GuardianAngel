/*
 * platform.hpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_PLATFORM_HPP_
#define INCLUDE_PLATFORM_HPP_


// 'undefine' GA_POSIX for AVR ESP32 platform.

// #define GA_POSIX


#ifdef GA_POSIX

#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

#else

#include <Arduino.h>

#endif

#endif /* INCLUDE_PLATFORM_HPP_ */
