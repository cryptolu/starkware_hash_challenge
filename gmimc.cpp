#include "header.cpp"

// -----------------------------------------------------------------
// INSTANCE PARAMETERS
// -----------------------------------------------------------------
#ifndef HEAVY
    // S45a = GMiMCParams(field=F91, r=2, c=1, num_rounds=121)
    const int NROUNDS = 121;
    const int NWORDS = 3;
    const int CAPACITY = 1;
    const char *FOLDER = "datagmimc1/%09d";
    const char *TEST_VECTOR = "674fd68da1e8297cb0f1f15";
    #include "gmimc1.h"
#else
    // S45b = GMiMCParams(field=F91, r=10, c=1, num_rounds=137)
    const int NROUNDS = 137;
    const int NWORDS = 11;
    const char *FOLDER = "datagmimc2/%09d";
    const char *TEST_VECTOR = "0e5a84bbae0d9d66bf6d329";
    #include "gmimc2.h"
#endif

void precomp() {}

void print(F *state) {
    for (int i = 0; i < NWORDS; i++) {
        state[i].print();
    }
}

void permutation(F *state) {
    F sum;
    
    int k = 0;
    for(int i = 0; i < NROUNDS; i++) {
        state[k] = state[k] + sum;
        sum = sum + (state[k] + CONST[i]).cube();
        state[k] = state[k] - sum;
        k++;
        if (k >= NWORDS) k = 0;
    }
    for(; k > 0; k--) {
        for(int j = 1; j < NWORDS; j++) {
            swap(state[j-1], state[j]);
        }
    }
    for(int j = 0; j < NWORDS; j++) {
        state[j] = state[j] + sum;
    }
    return;
}

void permutation_ref(F *state) {
    for(int i = 0; i < NROUNDS; i++) {
        F mask = (state[0] + CONST[i]).cube();
        for(int j = 1; j < NWORDS; j++) {
            state[j] = state[j] + mask;
            swap(state[j], state[j-1]);
        }
    }
    return;
}


#include "main.cpp"