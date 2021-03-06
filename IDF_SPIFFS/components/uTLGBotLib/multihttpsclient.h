/**************************************************************************************************/
// File: multihttpsclient.h
// Description: Basic Multiplatform HTTPS Client (Implement network HALs for differents devices).
// Created on: 04 may. 2019
// Last modified date: 11 may. 2019
// Version: 0.0.1
/**************************************************************************************************/

/* Include Guard */

#ifndef MULTIHTTPSCLIENT_H_
#define MULTIHTTPSCLIENT_H_

/**************************************************************************************************/

/* Check Build System */

#if !defined(ARDUINO) && !defined(ESP_IDF) && !defined(WIN32) && !defined(_WIN32) && \
!defined(__linux__)
    #error Unsupported system (Supported: Windows, Linux and ESP32)
#endif

/**************************************************************************************************/

/* Use Specific HAL for build system */


#include "multihttpsclient_espidf.h"

/**************************************************************************************************/

#endif
