/*
 * sntp_app.h
 *
 *  Created on: 10 џэт. 2021 у.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_SNTP_SNTP_APP_H_
#define COMPONENTS_SNTP_SNTP_APP_H_

#pragma once

void sntp_start();
#ifdef __cplusplus
extern "C" {
#endif

//extern uint8_t sntp_connected;


uint8_t sntp_getStat();

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_SNTP_SNTP_APP_H_ */
