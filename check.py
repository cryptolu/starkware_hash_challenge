#!/usr/bin/env python3

import os, sys

POINTS_FILE = sys.argv[1]
LOCATE_BIN = sys.argv[2]

prev = None
previ = None
ntotal = 0
nfake = 0
for line in open(POINTS_FILE):
    dst, src, itr = line.split()
    if dst == prev:
        src2, itr2 = previ
        if src == src2:
            nfake += 1
            continue
        print("REPEAT", dst, "FROM", (src, itr), previ)
        s1, n1 = src, int(itr)
        s2, n2 = src2, int(itr2)
        if n1 > n2:
            (s1, n1), (s2, n2) = (s2, n2), (s1, n1)

        cmd = [
            LOCATE_BIN,
            "%016x" % (int(s1, 16) >> 64),
            "%016x" % (int(s1, 16) % 2**64),
            str(n1),
            "%016x" % (int(s2, 16) >> 64),
            "%016x" % (int(s2, 16) % 2**64),
            str(n2),
        ]
        print("CMD", " ".join(cmd))
        os.system(" ".join(cmd))

    prev = dst
    previ = src, itr
    ntotal += 1
print("total", ntotal, "fake", nfake)
