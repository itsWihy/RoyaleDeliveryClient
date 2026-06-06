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
 * @brief The IP address or hostname of the server (Remote Pi).
 * Updated to use the production domain.
 */
#define PI_ADDRESS "100.97.85.5" // Tailscale IP for secure remote access without port forwarding

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
