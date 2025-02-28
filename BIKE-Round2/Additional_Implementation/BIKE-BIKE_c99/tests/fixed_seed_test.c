/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kem.h"
#include "utilities.h"
#include "measurements.h"

////////////////////////////////////////////////////////////////
//                 Main function for testing
////////////////////////////////////////////////////////////////
int main()
{
    srand(0);

    sk_t sk    = {0}; // private-key: (h0, h1)
    pk_t pk    = {0}; // public-key:  (g0, g1)
    ct_t ct    = {0}; // ciphertext:  (c0, c1)
    ss_t k_enc = {0}; // shared secret after encapsulate
    ss_t k_dec = {0}; // shared secret after decapsulate

    for (uint32_t i=1; i <= NUM_OF_TESTS; ++i)
    {
        int res = 0;

        MSG("Code test: %d\n\n", i);

        //Key generation
#if (BIKE_VER == 2)
        MEASURE("  keypair", res = crypto_kem_keypair(pk.raw, sk.u.raw););
#else
        MEASURE("  keypair", res = crypto_kem_keypair(pk.u.raw, sk.u.raw););
#endif

        if(res != 0)
        {
            MSG("Keypair failed with error: %d\n", res);
            continue;
        }

        uint32_t dec_rc = 0;

        //Encapsulate
#if (BIKE_VER == 2)
        MEASURE("  encaps",  res = crypto_kem_enc(ct.raw, k_enc.raw, pk.raw););
#else
        MEASURE("  encaps",  res = crypto_kem_enc(ct.u.raw, k_enc.raw, pk.u.raw););
#endif
        if(res != 0)
        {
            MSG("encapsulate failed with error: %d\n", res);
            continue;
        }

        //Decapsulate
#if (BIKE_VER == 2)
        MEASURE("  decaps", dec_rc = crypto_kem_dec(k_dec.raw, ct.raw, sk.u.raw););
#else
        MEASURE("  decaps", dec_rc = crypto_kem_dec(k_dec.raw, ct.u.raw, sk.u.raw););
#endif

        if (dec_rc != 0)
        {
            printf("Decoding failed after %d code tests!\n", i);
        }
        else
        {
            if (safe_cmp(k_enc.raw, k_dec.raw, sizeof(k_dec)/sizeof(uint64_t)))
            {
                MSG("Success! decapsulated key is the same as encapsulated key!\n");
            } else {
                MSG("Failure! decapsulated key is NOT the same as encapsulated key!\n");
            }
        }
        
        DMSG("Initiator's generated key (K) of 256 bits = ");
        print((uint64_t*)k_enc.raw, ELL_K_SIZE*8);
        DMSG("Responder's computed key (K) of 256 bits  = ");
        print((uint64_t*)k_dec.raw, ELL_K_SIZE*8);
    }

    return 0;
}
