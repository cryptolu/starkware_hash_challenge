const __uint64_t DIST_MASK = EXP2(DIST) - 1;
const __uint128_t TAKE_MASK = EXP2(TAKE) - 1;

#ifdef MAIN_SIEVE
int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    precomp();

    if (argc == 1) {
        F ma[NWORDS] = {1, 2};
        permutation(ma);
        print(ma);
        puts("");
        
        F mb[NWORDS] = {1, 2};
        permutation_ref(mb);
        print(mb);
        puts("");
        
        for(int i = 0; i < NWORDS; i++) {
            assert(ma[i] == mb[i]);
        }

        // benchmark
        auto t_start = high_resolution_clock::now();

        const uint64_t numitr = 1e5;
        F m1[NWORDS] = {1};
        for (int i = 0; i < numitr; i++) {
            permutation(m1);
        }
        print(m1);

        auto ms = duration_cast<microseconds>(high_resolution_clock::now() - t_start).count();
        fprintf(stderr, "%lu permutation calls, %.7lf microseconds per call\n", numitr, double(ms / double(numitr)));

        assert(F(m1[0]).hex() == TEST_VECTOR);
        return 0;
    }
    assert(argc == 2);
        
    int worker_id = atoi(argv[1]);
    srand(worker_id);

    fprintf(stderr, "worker %d dist %d take %d\n", worker_id, DIST, TAKE);
    
    char fpath[4096];
    sprintf(fpath, FOLDER, worker_id);
    FILE *fd = fopen(fpath, "a");
    assert(fd);

    __uint128_t last_a = worker_id;
    F state[NWORDS] = {F(last_a)};
    uint64_t numitr = 0;

    auto t_start = high_resolution_clock::now();

    while (1) {
        for(int i = 1; i < NWORDS; i++)
            state[i].val = 0;
        
        permutation(state);
        numitr++;

        state[0].val &= TAKE_MASK;
        __uint128_t h = state[0].val;
        if ((h & DIST_MASK) == 0) {
            auto ms = duration_cast<microseconds>(high_resolution_clock::now() - t_start).count();
            
            fprintf(stderr, "worker %09d: %s -> %s (%9ld calls in %.2lf seconds, %.7lf microseconds per permutation call)\n",
                worker_id,
                hexc(last_a), hexc(state[0].val),
                numitr, ms / 1e6, double(ms / (double) numitr)
            );
            
            fprintf(fd, "%s %s %ld\n",
                hexc(h), hexc(last_a), numitr
            );
            fflush(fd);
        }
        else if (numitr >= DIST_MASK * 5) { // refresh to avoid loops
        }
        else {
            continue;
        }


        last_a += uint64_t(rand() * 0xaabbccddeeff0011ull);
        if (last_a > MOD) last_a -= MOD;
        state[0].val = last_a;
        numitr = 0;
        t_start = high_resolution_clock::now();
    }
    return 0;
}
#endif

#ifdef MAIN_LOCATE
int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    precomp();

    __uint128_t a1 = strtoull(argv[1], NULL, 16) * EXP2(64) + strtoull(argv[2], NULL, 16);
    uint64_t n1 = strtoull(argv[3], NULL, 10);
    __uint128_t a2 = strtoull(argv[4], NULL, 16) * EXP2(64) + strtoull(argv[5], NULL, 16);
    uint64_t n2 = strtoull(argv[6], NULL, 10);

    unordered_map<uint64_t, uint64_t> tab;

    F state[NWORDS];
    state[0].val = a1;
    for (uint64_t i = 0; i < n1; i++) {
        uint64_t h = (state[0].val & TAKE_MASK);
        tab[h] = i;
        for (int ii = 1; ii < NWORDS; ii++)
            state[ii].val = 0;
        permutation(state);
        state[0].val &= TAKE_MASK;
    }
    printf("distinguished target 1: %s\n", hexc(state[0].val));

    state[0].val = a2;
    __uint128_t prev;
    for (uint64_t i = 0; i < n2; i++) {
        uint64_t h = (state[0].val & TAKE_MASK);
        if (tab.find(h) != tab.end()) {
            int j = tab[h];
            printf("Collision: steps %lu %lu\n", i, j);
            printf("  (0x%s, 0, ...) -> %016lx\n",
                hexc(prev), h
            );
            
            F tmp[NWORDS];
            __uint128_t tprev;
            tmp[0].val = a1;
            for (int k = 0; k < j; k++) {
                for (int ii = 1; ii < NWORDS; ii++)
                    tmp[ii].val = 0;
                tprev = tmp[0].val;
                permutation(tmp);
                tmp[0].val &= TAKE_MASK;
            }
            uint64_t h2 = tmp[0].val;
            printf("  (0x%s, 0, ...) -> %016lx\n",
                hexc(tprev), h2
            );
            printf("\n");
            return 0;
        }
        for (int ii = 1; ii < NWORDS; ii++)
            state[ii].val = 0;
        prev = state[0].val;
        permutation(state);
        state[0].val &= TAKE_MASK;
    }
    printf("distinguished target 2: %s\n", hexc(state[0].val));

    return 0;
}
#endif 