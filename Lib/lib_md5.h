/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * This software was written by Alexander Peslyak in 2001.  No copyright is
 * claimed, and the software is hereby placed in the public domain.
 * In case this attempt to disclaim copyright and place the software in the
 * public domain is deemed null and void, then the software is
 * Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 * general public under the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 *
 * See md5.c for more information.
 */

#ifndef _LIB_MD5_H
#define _LIB_MD5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include<stdint.h>


/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD5_u32plus;

typedef struct {
    MD5_u32plus lo, hi;
    MD5_u32plus a, b, c, d;
    unsigned char buffer[64];
    MD5_u32plus block[16];
} MD5_CTX;

extern void lib_md5_init(MD5_CTX *ctx);
extern void lib_md5_update(MD5_CTX *ctx, const void *data, unsigned long size);
extern void lib_md5_final(unsigned char *result, MD5_CTX *ctx);
extern void lib_md5_encode(unsigned char *output,unsigned char *input,unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
