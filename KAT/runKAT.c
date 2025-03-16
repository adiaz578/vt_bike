#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "defs.h"
#include "aes_ctr_prf.h"
#include "sampling.h"
#include "ntl.h"

#define KAT_FILE "../KAT/PQCkemKAT_BIKE2-Level1_2542.rsp"
#define MAX_MARKER_LEN 50
#define CIPHERTEXT_BYTES 2542
#define PUBLICKEY_BYTES 1271
#define SECRETKEY_BYTES 2542
#define CRYPTO_BYTES 32

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int FindMarker(FILE *infile, const char *marker)
{
    char line[MAX_MARKER_LEN];
    int i, len;
    int curr_line;

    len = (int)strlen(marker);
    if (len > MAX_MARKER_LEN - 1)
        len = MAX_MARKER_LEN - 1;

    for (i = 0; i < len; i++)
    {
        curr_line = fgetc(infile);
        line[i] = curr_line;
        if (curr_line == EOF)
            return 0;
    }
    line[len] = '\0';

    while (1)
    {
        if (!strncmp(line, marker, len))
            return 1;

        for (i = 0; i < len - 1; i++)
            line[i] = line[i + 1];
        curr_line = fgetc(infile);
        line[len - 1] = curr_line;
        if (curr_line == EOF)
            return 0;
        line[len] = '\0';
    }

    // shouldn't get here
    return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
    int i, ch, started;
    unsigned char ich;

    if (Length == 0)
    {
        A[0] = 0x00;
        return 1;
    }
    memset(A, 0x00, Length);
    started = 0;
    if (FindMarker(infile, str))
        while ((ch = fgetc(infile)) != EOF)
        {
            if (!isxdigit(ch))
            {
                if (!started)
                {
                    if (ch == '\n')
                        break;
                    else
                        continue;
                }
                else
                    break;
            }
            started = 1;
            if ((ch >= '0') && (ch <= '9'))
                ich = ch - '0';
            else if ((ch >= 'A') && (ch <= 'F'))
                ich = ch - 'A' + 10;
            else if ((ch >= 'a') && (ch <= 'f'))
                ich = ch - 'a' + 10;
            else // shouldn't ever get here
                ich = 0;

            for (i = 0; i < Length - 1; i++)
                A[i] = (A[i] << 4) | (A[i + 1] >> 4);
            A[Length - 1] = (A[Length - 1] << 4) | ich;
        }
    else
        return 0;

    return 1;
}

void get_seeds(OUT double_seed_t *seeds)
{
    randombytes(seeds->s1.raw, sizeof(double_seed_t));
}

void generateH(char *h0_out, char *h0_inv_out, char *h1_out)
{
    uint8_t h0[R_SIZE];
    uint8_t h0_inv[R_SIZE];
    uint8_t h1[R_SIZE];

    // generate seed
    double_seed_t seeds = {0};
    get_seeds(&seeds);

    aes_ctr_prf_state_t h_prf_state = {0};
    init_aes_ctr_prf_state(&h_prf_state, MAX_AES_INVOKATION, &seeds.s1);
    generate_sparse_rep(h0, DV, R_BITS, &h_prf_state);
    generate_sparse_rep(h1, DV, R_BITS, &h_prf_state);
    ntl_mod_inv(h0_inv, h0);

    for (size_t i = 0; i < R_SIZE; i++)
    {
        char buf[3];
        sprintf(buf, "%02X", h0[i]);
        h0_out[2*i] = buf[0];
        h0_out[2*i+1] = buf[1];

        sprintf(buf, "%02X", h0_inv[i]);
        h0_inv_out[2*i] = buf[0];
        h0_inv_out[2*i+1] = buf[1];

        sprintf(buf, "%02X", h1[i]);
        h1_out[2*i] = buf[0];
        h1_out[2*i+1] = buf[1];
    }
}

void generateE(char *e_out)
{
    uint8_t e[N_SIZE] = {0};

    // generate seed
    double_seed_t seeds = {0};
    get_seeds(&seeds);

    aes_ctr_prf_state_t e_prf_state = {0};
    init_aes_ctr_prf_state(&e_prf_state, MAX_AES_INVOKATION, &seeds.s1);
    generate_sparse_rep(e, T1, N_BITS, &e_prf_state);

    for (size_t i = 0; i < N_SIZE; i++)
    {
        char buf[3];
        sprintf(buf, "%02X", e[i]);
        e_out[2*i] = buf[0];
        e_out[2*i+1] = buf[1];
    }
}

int main()
{
    FILE *fp_rsp;
    int count;
    int done;
    unsigned char seed[48];
    unsigned char ct[CIPHERTEXT_BYTES], ss[CRYPTO_BYTES], ss1[CRYPTO_BYTES];
    unsigned char pk[PUBLICKEY_BYTES], sk[SECRETKEY_BYTES];

    // parse the RESPONSE file
    if ((fp_rsp = fopen(KAT_FILE, "r")) == NULL)
    {
        printf("Couldn't open <%s> for read\n", KAT_FILE);
        return -1;
    }

    char *line_buffer;
    done = 0;
    do
    {
        // KAT info
        if (FindMarker(fp_rsp, "count = "))
        {
            int tmp = fscanf(fp_rsp, "%d", &count);
        }
        else
        {
            done = 1;
            break;
        }
        printf("Running KAT #%d\n", count);

        // Keygen
        if (!ReadHex(fp_rsp, seed, 48, (char *)"seed = "))
        {
            printf("ERROR: unable to read 'seed' from <%s>\n", KAT_FILE);
            fclose(fp_rsp);
            return -1;
        }
        randombytes_init(seed, NULL, 256);



        char h0[R_SIZE*2+1];
        char h0_inv[R_SIZE*2+1];
        char h1[R_SIZE*2+1];
        generateH(h0, h0_inv, h1);

        char cryptol_keygen[100000] = "cryptol -c ':l ../key_gen.cry' -c 'keyGen 0x";
        strcat(cryptol_keygen, h0);
        strcat(cryptol_keygen, " 0x");
        strcat(cryptol_keygen, h0_inv);        
        strcat(cryptol_keygen, " 0x");
        strcat(cryptol_keygen, h1);
        strcat(cryptol_keygen, "'");
        //system(cryptol_keygen);

        // Encryption
        if (!ReadHex(fp_rsp, pk, PUBLICKEY_BYTES, (char *)"pk = 0100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"))
        {
            printf("ERROR: unable to read 'pk' from <%s>\n", KAT_FILE);
            fclose(fp_rsp);
            return -1;
        }

        //reset cryptol cmd
        char cryptol_enc[100000] = "cryptol -c ':l ../encaps.cry' -c 'encrypt (0x";

        char e[N_SIZE*2+1];
        generateE(e);

        char pk_str[2*PUBLICKEY_BYTES+1];
        for (size_t i=0; i<PUBLICKEY_BYTES; i++)
        {
            char buf[3];
            sprintf(buf, "%02X", pk[i]);
            pk_str[2*i] = buf[0];
            pk_str[2*i+1] = buf[1];
        }

        strcat(cryptol_enc, e);
        strcat(cryptol_enc, ", 0x");
        strcat(cryptol_enc, pk_str);
        strcat(cryptol_enc, ")'");

        system(cryptol_enc);

        // todo remove
        done = 1;

    } while (!done);

    fclose(fp_rsp);
}