#!/bin/bash

# Abort the script if any command failed to return 0
set -e

# Print the instructios
set -x

##########################
# Test BIKE constant time.
##########################
function check_constant_kats {
    # BIKE L1
    diff PQCkemKAT_4670.rsp kats/BIKE1_L1.const.kat > /dev/null
    diff PQCkemKAT_5941.rsp kats/BIKE2_L1.const.kat > /dev/null
    diff PQCkemKAT_7628.rsp kats/BIKE3_L1.const.kat > /dev/null

    # BIKE L3
    diff PQCkemKAT_8116.rsp kats/BIKE1_L3.const.kat > /dev/null
    diff PQCkemKAT_10598.rsp kats/BIKE2_L3.const.kat > /dev/null
    diff PQCkemKAT_13996.rsp kats/BIKE3_L3.const.kat > /dev/null

    # BIKE L5
    diff PQCkemKAT_12364.rsp kats/BIKE1_L5.const.kat > /dev/null
    diff PQCkemKAT_16458.rsp kats/BIKE2_L5.const.kat > /dev/null
    diff PQCkemKAT_22244.rsp kats/BIKE3_L5.const.kat > /dev/null

    # Clean
    rm PQCkemKAT*
}

function check_kats {
    # Compare all
    # BIKE L1
    diff PQCkemKAT_3606.rsp kats/BIKE1_L1.kat > /dev/null
    diff PQCkemKAT_4877.rsp kats/BIKE2_L1.kat > /dev/null
    diff PQCkemKAT_6572.rsp kats/BIKE3_L1.kat > /dev/null

    # BIKE L3
    diff PQCkemKAT_6540.rsp kats/BIKE1_L3.kat > /dev/null
    diff PQCkemKAT_9022.rsp kats/BIKE2_L3.kat > /dev/null
    diff PQCkemKAT_12420.rsp kats/BIKE3_L3.kat > /dev/null

    # BIKE L5
    diff PQCkemKAT_10276.rsp kats/BIKE1_L5.kat > /dev/null
    diff PQCkemKAT_14370.rsp kats/BIKE2_L5.kat > /dev/null
    diff PQCkemKAT_20156.rsp kats/BIKE3_L5.kat > /dev/null

    # Clean
    rm PQCkemKAT*
}

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make CONSTANT_TIME=1 USE_NIST_RAND=1 OPENSSL=`realpath ../openssl_orig` BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_constant_kats

####################################################
# Test BIKE constant time with USE_OPENSSL=1.
####################################################

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make CONSTANT_TIME=1 USE_NIST_RAND=1 USE_OPENSSL=1 OPENSSL=`realpath ../openssl_orig` BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_constant_kats

####################################################
# Test BIKE constant time with AVX2=1.
####################################################

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make CONSTANT_TIME=1 USE_NIST_RAND=1 AVX2=1 OPENSSL=`realpath ../openssl_orig` BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_constant_kats

###############################
# Test BIKE non constant time.
###############################

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make USE_NIST_RAND=1 OPENSSL=`realpath ../openssl_orig` BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_kats

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make USE_NIST_RAND=1 OPENSSL=`realpath ../openssl_orig` USE_OPENSSL=1 BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_kats

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make USE_NIST_RAND=1 OPENSSL=`realpath ../openssl_orig` AVX2=1 USE_OPENSSL=1 BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_kats

# Compile all
for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make USE_NIST_RAND=1 OPENSSL=`realpath ../openssl_orig` AVX2=1 BIKE_VER=$bike_ver LEVEL=$level;  > /dev/null
        ./bin/main
    done
done
check_kats

for bike_ver in "1" "2" "3"; do
    for level in "1" "3" "5"; do
        make clean; 
        make CHECK_COMPILATION=1 BIKE_VER=$bike_ver LEVEL=$level; 
        make clean; 
        make CHECK_COMPILATION=1 CONSTANT_TIME=1 BIKE_VER=$bike_ver LEVEL=$level; 
        make clean; 
        make CHECK_COMPILATION=1 USE_OPENSSL=1 BIKE_VER=$bike_ver LEVEL=$level; 
        make clean; 
        make CHECK_COMPILATION=1 CONSTANT_TIME=1 USE_OPENSSL=1 BIKE_VER=$bike_ver LEVEL=$level; 
        make clean; 
        make CHECK_COMPILATION=1 CONSTANT_TIME=1 USE_OPENSSL=1 AVX2=1 BIKE_VER=$bike_ver LEVEL=$level; 
        make clean; 
        make CHECK_COMPILATION=1 CONSTANT_TIME=1 AVX2=1 BIKE_VER=$bike_ver LEVEL=$level; 
        ./bin/main
    done
done
