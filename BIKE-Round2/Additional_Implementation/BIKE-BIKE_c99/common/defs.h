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

#pragma once

////////////////////////////////////////////
//             Basic defs
///////////////////////////////////////////

#ifdef __cplusplus
  #define EXTERNC extern "C"
  #define EXTERNC_START EXTERNC {
  #define EXTERNC_END }
#else
  #define EXTERNC
  #define EXTERNC_START
  #define EXTERNC_END
#endif

// For code clarity.
#define IN
#define OUT

#define ALIGN(n) __attribute__((aligned(n)))
#define BIKE_UNUSED(x) (void) (x)
#define BIKE_UNUSED_ATT __attribute__((unused))

#define _INLINE_ static inline

// In asm the symbols '==' and '?' are not allowed therefore if using divide_and_ceil
// in asm files we must ensure with static_assert its validity
#ifdef __ASM_FILE__
#define bike_static_assert(COND, MSG)
#elif (__cplusplus >= 201103L) || defined(static_assert)
#define bike_static_assert(COND, MSG) static_assert(COND, "MSG")
#else
#define bike_static_assert(COND, MSG) typedef char static_assertion_##MSG[(COND) ? 1 : -1] BIKE_UNUSED_ATT
#endif

// Divide by the divider and round up to next integer
#define DIVIDE_AND_CEIL(x, divider) ((x + divider) / divider)

//Bit manipations
//Linux Assemblies except for Ubuntu can't understand what UL mean.
//Therefore in that case len must be smaller than 31.
#ifdef __ASM_FILE__ 
  #define BIT(len) (1 << (len))
#else
  #define BIT(len) (1UL << (len))
#endif

#define MASK(len) (BIT(len) - 1)
#define SIZEOF_BITS(b) (sizeof(b)*8)

#define XMM_SIZE 0x10
#define YMM_SIZE 0x20
#define ZMM_SIZE 0x40

#define ALL_YMM_SIZE (16*YMM_SIZE)
#define ALL_ZMM_SIZE (32*ZMM_SIZE)

////////////////////////////////////////////
//             Debug
///////////////////////////////////////////

#ifndef VERBOSE 
  #define VERBOSE 0
#endif

#ifndef __ASM_FILE__
  #include <stdio.h>

  #if (VERBOSE == 3)
    #define MSG(...)     { printf(__VA_ARGS__); }
    #define DMSG(...)    MSG(__VA_ARGS__)
    #define EDMSG(...)   MSG(__VA_ARGS__)
    #define SEDMSG(...)  MSG(__VA_ARGS__)
  #elif (VERBOSE == 2)
    #define MSG(...)     { printf(__VA_ARGS__); }
    #define DMSG(...)    MSG(__VA_ARGS__)
    #define EDMSG(...)   MSG(__VA_ARGS__)
    #define SEDMSG(...)
  #elif (VERBOSE == 1)
    #define MSG(...)     { printf(__VA_ARGS__); }
    #define DMSG(...)    MSG(__VA_ARGS__)
    #define EDMSG(...)
    #define SEDMSG(...)
  #else
    #define MSG(...)     { printf(__VA_ARGS__); }
    #define DMSG(...)
    #define EDMSG(...)
    #define SEDMSG(...)
  #endif
#endif

////////////////////////////////////////////
//              Printing
///////////////////////////////////////////

//Show timer rests in cycles.
//#define RDTSC

//#define PRINT_IN_BE
//#define NO_SPACE
//#define NO_NEWLINE

////////////////////////////////////////////
//              Testing
///////////////////////////////////////////
#ifndef NUM_OF_TESTS
  #define NUM_OF_TESTS  1
#endif

//Disabled for random testing
//#define USE_NIST_RAND

