# Optimized Collision Search for StarkWare Hash Challenge

This repository contains an implementation of the generic collision search method applied to 91-bit hash functions from the [StarkWare Hash Challenge](https://starkware.co/hash-challenge/).

The attack and optimizations are discussed in [the blog post](https://affine.group/2020/02/stark-hash-challenge).

The code can be used to break small and large instances of GMiMC-erf and small instance of Poseidon in about 1000-2000 core-days.

## Running the attack

The following example is for small instance of GMiMC-erf, which is labelled **gmimc1**. The other instances are **gmimc2** and **pos1**.

The first step is to compile the code.

```bash
$ make gmimc1 take=50 dist=20
```

- `take` defines the number of bits in the hash functon (default is 92);
- `dist` defines the number of zero bits in distinguished points (default is 25).

The second step is to run many instances of the program to collect distinguished points:

```bash
$ ./gmimc1 [worker_id]
worker 1 dist 20 take 50                                                                                                                           
worker 000000001: 00000000000000000000001 -> 00000000001c82513d00000 (   318309 calls in 0.90 seconds, 2.8328762 microseconds per permutation call)
worker 000000001: 00000004f2c983207d89bd8 -> 000000000003f5516300000 (  2386364 calls in 6.87 seconds, 2.8805392 microseconds per permutation call)
worker 000000001: 0000000d0bf060e183ffbfe -> 0000000000128ed4f400000 (    78798 calls in 0.23 seconds, 2.9581461 microseconds per permutation call) 
...
```

Here `[worker_id]` is an integer seed to randomize the worker.

Finally, the collected pool of distinguished points is checked for a collision:

```bash
$ make gmimc1check
...
Collision: steps 315173 298519
  (0x00000000001bcb0673eaca2, 0, ...) -> 00007e309caf5769
  (0x000000000029d327c4c9d54, 0, ...) -> 00007e309caf5769
```