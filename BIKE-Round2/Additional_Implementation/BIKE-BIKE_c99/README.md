To compile:
    make

Default is non constant time BIKE1 in lEVEL=5 (128-bit quantum security) without AVX2/512 support.
The default assumes that the AES_NI, POPCNT, and PCLMUL instructions are available (if any other case use USE_OPENSSL=1).

Additional compilation flags:
 - USE_NIST_RAND - Using the RDBG of NIST.
 - USE_OPENSSL   - Use OpenSSL for AES/SHA and GF2X operations. (OpenSSL must be installed).
 - FIXED_SEED    - Using a fixed seed.
 - RDTSC         - Measure time in cycles not in mseconds
 - VERBOSE       - Add verbose (level1-3)
 - CHECK_COMPILATION - To validate that there are no compiler warnings.
 - CONSTANT_TIME - Compile the program in constant time mode (slower).
 - NUM_OF_TESTS  - Set the number of tests to be run.
 - AVX2          - Compile with AVX2 support.
 - AVX512        - Compile with AVX512 support.
 - VALIDATE_CONSTANT_TIME - Compile a non constant time versions that generate inputs compatible with the constant time implementaiton.
 - LEVEL         - Security level (1/3/5).
 - BIKE_VER      - Choose the BIKE to be used (1/2/3).
 - BATCH_SIZE    - Only in BIKE2, set the batch key generation optimization.
 
 To clean:
    make clean.
    
Note: A clean is required whenever different flags are used for compilation.

The package was compiled and tested with gcc (version 5.4.0) in 64-bit mode, using the
"O3" Optimization level, and run on a Linux (Ubuntu 16.04.3 LTS) OS. Other OSs compilers may require some small modifications.