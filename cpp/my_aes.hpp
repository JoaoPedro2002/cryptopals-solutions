//
// Created by joao on 02/09/22.
//
#ifndef CPP_MY_AES_H
#define CPP_MY_AES_H

#include "cryptoutils.hpp"

#ifndef CBC
#define CBC 1
#endif

#ifndef ECB
#define ECB 1
#endif

#define N_ROUNDS 10
#define AES_BLOCK_LEN 16

#define AES_KEY_LEN 16 // 128 bits
#define AES_KEY_EXPECTED_SIZE 176

typedef uint8_t state_t[4][4];

struct AES_ctx {
    uint8_t RoundKey[AES_KEY_EXPECTED_SIZE];
    uint8_t IV[AES_BLOCK_LEN];
};

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);

void AES_cipher(state_t state, const uint8_t* round_key);
void AES_inv_cipher(state_t state, const uint8_t* round_key);

void AES_ecb_encrypt_buffer(const struct AES_ctx* ctx, uint8_t* buffer);
void AES_ecb_decrypt_buffer(const struct AES_ctx* ctx, uint8_t* buffer);

void AES_cbc_encrypt_buffer(const struct AES_ctx* ctx, uint8_t* buffer);
void AES_cbc_decrypt_buffer(const struct AES_ctx* ctx, uint8_t* buffer);




#endif //CPP_MY_AES_H
