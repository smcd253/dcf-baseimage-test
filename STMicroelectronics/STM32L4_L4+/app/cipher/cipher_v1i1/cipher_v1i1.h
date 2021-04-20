// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.
// See LICENSE file in the project root for full license information.

#ifndef CIPHER_V1I1_H
#define CIPHER_V1I1_H

#include "az_ulib_result.h"
#include "azure/az_core.h"

#ifdef __cplusplus
#include <cstdint>
extern "C"
{
#else
#include <stdint.h>
#endif

  az_result cipher_v1i1_create(void);
  az_result cipher_v1i1_destroy(void);

  az_result cipher_v1i1_encrypt(uint32_t context, az_span src, az_span* dest);

  az_result cipher_v1i1_decrypt(az_span src, az_span* dest);

#ifdef __cplusplus
}
#endif

#endif /* CIPHER_V1I1_H */