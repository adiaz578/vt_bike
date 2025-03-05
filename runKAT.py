#!/usr/bin/env python3

import os
from pexpect import spawn
from enum import Enum

# constants
KAT_DIR_NAME = "KAT"
KAT_RSP_EXTNESION = ".rsp"
LEVEL_1 = "Level1"
LEVEL_3 = "Level3"
LEVEL_5 = "Level5"

class KAT_Test:
    count: int
    seed: str
    pk: str
    sk: str
    ct: str
    ss: str

class KAT_Group:
    level: int
    n: int
    r: int
    w: int
    t: int
    tests: list[KAT_Test]

class ParseState(Enum):
    COUNT = 1
    SEED = 2
    PK = 3
    SK = 4
    CT = 5
    SS = 6

def parseRsp(filepath):
    kat_tests = []
    rsp = open(filepath, "r")
    
    state = ParseState.COUNT
    kat_test = KAT_Test()
    for line in rsp:
        line = line.strip()
        match state:
            case ParseState.COUNT:
                if "count = " in line:
                    kat_test.count = int(line.split("count = ")[-1])
                    state = ParseState.SEED

            case ParseState.SEED:
                if "seed = " in line:
                    kat_test.seed = line.split("seed = ")[-1]
                    state = ParseState.PK

            case ParseState.PK:
                if "pk = " in line:
                    kat_test.pk = line.split("pk = ")[-1]
                    state = ParseState.SK

            case ParseState.SK:
                if "sk = " in line:
                    kat_test.sk = line.split("sk = ")[-1]
                    state = ParseState.CT

            case ParseState.CT:
                if "ct = " in line:
                    kat_test.ct = line.split("ct = ")[-1]
                    state = ParseState.SS

            case ParseState.SS:
                if "ss = " in line:
                    kat_test.ss = line.split("ss = ")[-1]
                    kat_tests.append(kat_test)
                    kat_test = KAT_Test()
                    state = ParseState.COUNT

    rsp.close()
    return kat_tests

# parse KAT files
kats = []
for kat in os.listdir(KAT_DIR_NAME):
    if kat.endswith(KAT_RSP_EXTNESION):
        # identify level
        parsed_kat = KAT_Group()
        if LEVEL_1 in kat:
            kats.append(KAT_Group(1, 20326, 10163, 142, 134, []))
        elif LEVEL_3 in kat:
            kats.append(KAT_Group(3, 39706, 19853, 206, 199, []))
        elif LEVEL_5 in kat:
            kats.append(KAT_Group(5, 65498, 32749, 274, 264, []))
        
        parsed_kat.tests = parseRsp(os.path.join(os.path.join(KAT_DIR_NAME, kat)))
        kats.append(parsed_kat)

# start cryptol
cryptol = spawn("cryptol")
cryptol.expect("Cryptol> ")

# set ascii mode
cryptol.sendline(":set ascii = on")
cryptol.expect("Cryptol> ")

for kat in kats:
    print(kat)

#cryptol.interact()