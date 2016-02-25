/*
 * u2f.h
 *
 *  Created on: Jan 26, 2016
 *      Author: pp
 */

#ifndef _U2F_H_
#define _U2F_H_

#include <stdint.h>

#define SW_NO_ERROR 						0x00
#define SW_CONDITIONS_NOT_SATISFIED 		0x01
#define SW_WRONG_DATA 						0x02

#define U2F_EC_FMT_UNCOMPRESSED				0x04

#define U2F_EC_POINT_SIZE					32
#define U2F_EC_PUBKEY_SIZE					65
#define U2F_APDU_SIZE                       7
#define U2F_CHALLENGE_SIZE                  32
#define U2F_APPLICATION_SIZE                32
#define U2F_REGISTER_REQUEST_SIZE           (U2F_CHALLENGE_SIZE+U2F_APPLICATION_SIZE)
#define U2F_KEY_HANDLE_SIZE                 4
#define U2F_MAX_REQUEST_PAYLOAD             (1 + U2F_CHALLENGE_SIZE+U2F_APPLICATION_SIZE + 1 + U2F_KEY_HANDLE_SIZE)


// U2F native commands
#define U2F_REGISTER 						0x01
#define U2F_AUTHENTICATE 					0x02
#define U2F_VERSION 						0x03
#define U2F_VENDOR_FIRST 					0xc0
#define U2F_VENDOR_LAST 					0xff

// U2F_CMD_REGISTER command defines
#define U2F_REGISTER_ID 					0x05
#define U2F_REGISTER_HASH_ID 				0x00


struct u2f_request_apdu
{
    uint8_t cla;
    uint8_t ins;
    uint8_t p1;
    uint8_t p2;
    uint8_t LC1;
    uint8_t LC2;
    uint8_t LC3;
    uint8_t payload[U2F_MAX_REQUEST_PAYLOAD];
};

struct u2f_ec_point
{
    uint8_t fmt;
    uint8_t x[U2F_EC_POINT_SIZE];
    uint8_t y[U2F_EC_POINT_SIZE];
};

struct u2f_register_request
{
    uint8_t chal[U2F_CHALLENGE_SIZE];
    uint8_t app[U2F_APPLICATION_SIZE];
};


int u2f_request(struct u2f_message* req, struct u2f_message* res);
void u2f_response_writeback(uint8_t * buf, uint8_t len);

#endif /* U2F_H_ */
