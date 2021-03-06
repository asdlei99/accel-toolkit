/*********************************************************************
* Filename:   SHA384.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../SHA384.h"
#include <intrin.h>

#define HASH_SHA384_BLOCKSIZE 128

const uint64_t HASH_SHA384_K[80] = {
    0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC, 0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
    0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2, 0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
    0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65, 0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
    0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4, 0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
    0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF, 0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
    0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30, 0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
    0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8, 0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
    0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC, 0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
    0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178, 0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
    0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C, 0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
};

void HASH_SHA384_Initialize(uint64_t HashBuffer[8]) {
    HashBuffer[0] = 0xCBBB9D5DC1059ED8;
    HashBuffer[1] = 0x629A292A367CD507;
    HashBuffer[2] = 0x9159015A3070DD17;
    HashBuffer[3] = 0x152FECD8F70E5939;
    HashBuffer[4] = 0x67332667FFC00B31;
    HashBuffer[5] = 0x8EB44A8768581511;
    HashBuffer[6] = 0xDB0C2E0D64F98FA7;
    HashBuffer[7] = 0x47B5481DBEFA4FA4;
}

void HASH_SHA384_MainCycle(const void* srcBytes, size_t srcBytesLength, uint64_t HashBuffer[8]) {
    uint64_t Buffer[80] = { 0 };
    uint64_t a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
    const uint64_t (*MessageBlock)[16] = srcBytes;

    size_t RoundsOfMainCycle = srcBytesLength / 128;
    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {

        for (int j = 0; j < 16; ++j)
            Buffer[j] = _byteswap_uint64(MessageBlock[i][j]);

        for (int j = 16; j < 80; ++j) {
            Buffer[j] = _rotr64(Buffer[j - 2], 19) ^ _rotr64(Buffer[j - 2], 61) ^ Buffer[j - 2] >> 6;
            Buffer[j] += Buffer[j - 7];
            Buffer[j] += _rotr64(Buffer[j - 15], 1) ^ _rotr64(Buffer[j - 15], 8) ^ Buffer[j - 15] >> 7;
            Buffer[j] += Buffer[j - 16];
        }

        a = HashBuffer[0];
        b = HashBuffer[1];
        c = HashBuffer[2];
        d = HashBuffer[3];
        e = HashBuffer[4];
        f = HashBuffer[5];
        g = HashBuffer[6];
        h = HashBuffer[7];

        for (int j = 0; j < 80; ++j) {
            uint64_t T1 = h + (_rotr64(e, 14) ^ _rotr64(e, 18) ^ _rotr64(e, 41)) + ((e & f) ^ (~e & g)) + HASH_SHA384_K[j] + Buffer[j];
            uint64_t T2 = (_rotr64(a, 28) ^ _rotr64(a, 34) ^ _rotr64(a, 39)) + ((a & b) ^ (a & c) ^ (b & c));
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        HashBuffer[0] += a;
        HashBuffer[1] += b;
        HashBuffer[2] += c;
        HashBuffer[3] += d;
        HashBuffer[4] += e;
        HashBuffer[5] += f;
        HashBuffer[6] += g;
        HashBuffer[7] += h;
    }
}

void HASH_SHA384_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, uint64_t HashBuffer[8]) {
    if (LeftBytesLength >= HASH_SHA384_BLOCKSIZE) {
        HASH_SHA384_MainCycle(LeftBytes, LeftBytesLength, HashBuffer);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / HASH_SHA384_BLOCKSIZE) * HASH_SHA384_BLOCKSIZE;
        LeftBytesLength %= HASH_SHA384_BLOCKSIZE;
    }

    uint8_t Extra[256] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    ((uint64_t*)(Extra + (LeftBytesLength >= 128 - 16 ? 256 - 16 : 128 - 16)))[0] = _byteswap_uint64(TotalBytesLength >> (sizeof(uint64_t) * 8 - 3));
    ((uint64_t*)(Extra + (LeftBytesLength >= 128 - 16 ? 256 - 16 : 128 - 16)))[1] = _byteswap_uint64(TotalBytesLength << 3);

    HASH_SHA384_MainCycle(Extra, LeftBytesLength >= 128 - 16 ? 256 : 128, HashBuffer);

    for (int i = 0; i < 8; ++i)
        HashBuffer[i] = _byteswap_uint64(HashBuffer[i]);
}
