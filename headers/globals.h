//
// Created by Wihy on 1/4/26.
//

/**
 * @file globals.h
 * @brief Global configuration and shared variables for the RoyaleDeliveryClient.
 */

#ifndef ROYALEDELIVERYCLIENT_GLOBALS_H
#define ROYALEDELIVERYCLIENT_GLOBALS_H
#include <QString>

/**
 * @brief The IP address of the server (Remote Pi).
 * Defaults to localhost for development.
 */
#define PI_ADDRESS "127.0.0.1" //FOR PROD USE 192.168.1.156

/**
 * @brief Global variable storing the currently authenticated client's name.
 */
inline QString CLIENT_NAME;

/**
 * @brief Global variable storing the currently authenticated client's raw password.
 * @note This is kept in memory for session operations.
 */
inline QString RAW_PASSWORD;

#endif //ROYALEDELIVERYCLIENT_GLOBALS_H
