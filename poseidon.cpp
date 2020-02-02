#include "header.cpp"

// -----------------------------------------------------------------
// INSTANCE PARAMETERS
// -----------------------------------------------------------------
#if 1
    // Poseidon_S45a = HadesParams(field=F91, r=2, c=1, Rf=8, Rp=26)
    const int Rf = 8;
    const int Rp = 26;
    const int NWORDS = 3;
    const char *FOLDER = "datapos1/%09d";
    const char *TEST_VECTOR = "128fbf3865cb5f0587d9b1e";
    #include "Poseidon_S45a.h"
#else
    // Poseidon_S45b = HadesParams(field=F91, r=10, c=1, Rf=8, Rp=28)
    const int Rf = 8;
    const int Rp = 28;
    const int NWORDS = 11;
    #include "Poseidon_S45b.h"
    const char *FOLDER = "datapos2/%09d";
#endif


// -----------------------------------------------------------------
// MDS TABULATION
// -----------------------------------------------------------------
__uint128_t mdstab[NWORDS][NWORDS][NWORDS] = {};

void precomp_mds() {
    for (int y = 0; y < NWORDS; y++) {
        for (int x = 0; x < NWORDS; x++) {
            __uint128_t v = MDS[y][x].val;
            for (int i = 0; i < NWORDS; i++) {
                mdstab[y][x][i] = v;
                v = (v << 32) % MOD;
            }
        }
    }
}

void do_mds(F* state) {
    __uint128_t out[NWORDS] = {};
    for (int y = 0; y < NWORDS; y++) {
        for (int x = 0; x < NWORDS; x++) {
            __uint128_t v = state[x].val;
            for (int i = 0; i < NWORDS; i++) {
                out[y] = out[y] + ((uint32_t)v) * mdstab[y][x][i];
                v >>= 32;
            }
        }
        out[y] = reduce(out[y]);
    }
    for (int j = 0; j < NWORDS; j++)
        state[j] = F(out[j]);
}


// -----------------------------------------------------------------
// PERMUTATION
// -----------------------------------------------------------------
void precomp() {
    precomp_mds();
}

void print(F *state) {
    for (int i = 0; i < NWORDS; i++) {
        state[i].print();
    }
}

void permutation(F *state) {
    int round_idx = 0;
    for(int i = 0; i < Rf / 2; i++) {
        for(int j = 0; j < NWORDS; j++)
            state[j] = (state[j] + CONST[round_idx][j]).cube();

        do_mds(state);
        round_idx++;
    }

    for (int i = 0; i < Rp; i++) {
        for(int j = 0; j < NWORDS-1; j++)
            state[j] = (state[j] + CONST[round_idx][j]);
        state[NWORDS-1] = (state[NWORDS-1] + CONST[round_idx][NWORDS-1]).cube();

        do_mds(state);
        round_idx++;
    }

    for(int i = 0; i < Rf / 2; i++) {
        for(int j = 0; j < NWORDS; j++)
            state[j] = (state[j] + CONST[round_idx][j]).cube();

        do_mds(state);
        round_idx++;
    }
    return;
}


auto permutation_ref = permutation;

#include "main.cpp"