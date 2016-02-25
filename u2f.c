#include "u2f.h"

#define DEBUG_PC
#ifdef DEBUG_PC
#include <stdio.h>
#else
#define printf(x)
#endif

// void u2f_response_writeback(uint8_t * buf, uint8_t len);
static int16_t u2f_register(struct u2f_register_request * req);

void u2f_request(struct u2f_request_apdu * req)
{
    uint16_t * rcode = req;
    u2f_response_start();
    switch(req->ins)
    {
        case U2F_REGISTER:
            *rcode = u2f_register(req->payload);
            break;
        case U2F_AUTHENTICATE:
        case U2F_VERSION:
        case U2F_VENDER_FIRST:
        case U2F_VENDER_LAST:
        default:
            printf("invalid u2f apdu command: %x\n", req->ins);
    }
    u2f_response_writeback(rcode,2);
    u2f_response_flush();
}

static int16_t u2f_register(struct u2f_register_request * req)
{
    uint8_t i[] = {0,U2F_EC_FMT_UNCOMPRESSED};

    uint8_t key_handle[U2F_KEY_HANDLE_SIZE];
    uint8_t pubkey[64];

    if (u2f_get_user_feedback() != 0)
    {
        return U2F_SW_CONDITIONS_NOT_SATISFIED;
    }

    u2f_new_keypair(key_handle, pubkey);

    u2f_sha_start();
    u2f_sha_update(i,1);
    u2f_sha_update(req->app,32);
    u2f_sha_update(req->chal,32);
    u2f_sha_update(key_handle,U2F_KEY_HANDLE_SIZE);
    u2f_sha_update(i+1,1);
    u2f_sha_update(pubkey,64);
    u2f_sha_finish(NULL, 0);
    
    u2f_ecdsa_sign(req, U2F_ATTESTATION_HANDLE);
    u2f_response_writeback(i,2);
    u2f_response_writeback(pubkey,64);
    pubkey[0] = U2F_KEY_HANDLE_SIZE;
    u2f_response_writeback(pubkey,1);
    u2f_response_writeback(key_handle,U2F_KEY_HANDLE_SIZE);
    u2f_response_writeback(u2f_get_attestation_cert(),U2F_ATTESTATION_CERT_SIZE);
    u2f_response_writeback(req, 64);

    return U2F_NO_ERROR;
}

