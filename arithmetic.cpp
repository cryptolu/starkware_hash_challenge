#define EXP2(i) (E<<(i))
const __uint128_t E = 1;

// F91 = GF(2**91 + 5 * 2**64 + 1)

const __uint128_t MOD = EXP2(91) + 5*EXP2(64) + 1;
const __uint128_t MODMASK = EXP2(91) - 1;
const __uint128_t MODe10 = MOD << 10;

__uint128_t reduce(__uint128_t a) {
    uint64_t t = (a >> 91);
    a &= MODMASK;
    a += MODe10;
    a -= ((__uint128_t)(5*t)<<64) + t;
    uint64_t tt = (a >> 91);
    a &= MODMASK;
    a += MOD;
    a -= ((__uint128_t)(5*tt)<<64) + tt;
    if (a >= MOD) a -= MOD;
    return a;
}
__uint128_t add(__uint128_t a, __uint128_t b) {
    __uint128_t res = a + b;
    if (res >= MOD) res -= MOD;
    return res;
}
__uint128_t sub(__uint128_t a, __uint128_t b) {
    __uint128_t res = a - b + MOD;
    if (res >= MOD) res -= MOD;
    return res;
}
__uint128_t multiply(__uint128_t a, __uint128_t b) {
    __uint128_t res = 0;

    res += a * (uint32_t)b;
    b >>= 32;
    a = reduce(a << 32);

    res += a * (uint32_t)b;
    b >>= 32;
    a = reduce(a << 32);

    res += a * (uint32_t)b;
    return reduce(res);
}
__uint128_t cube(__uint128_t a) {
    __uint128_t a1 = a;
    __uint128_t a2 = reduce(a1 << 32);
    __uint128_t a3 = reduce(a2 << 32);

    __uint128_t res = 0;
    res += a1 * (uint32_t)a; a >>= 32;
    res += a2 * (uint32_t)a; a >>= 32;
    res += a3 * (uint32_t)a;
    a = reduce(res);

    res = 0;
    res += a1 * (uint32_t)a; a >>= 32;
    res += a2 * (uint32_t)a; a >>= 32;
    res += a3 * (uint32_t)a;
    return reduce(res);
}

struct F {
    __uint128_t val;
    F () : val(0) {}
    F (const __uint128_t v) : val(v) {}

    F operator+(const F &other) const {
        return {add(val, other.val)};
    }
    F operator-(const F &other) const {
        return {sub(val, other.val)};
    }
    F operator*(const F &other) const {
        return {multiply(val, other.val)};
    }
    bool operator==(const F &other) const {
        return val == other.val;
    }
    F cube() const {
        return {::cube(val)};
    }
    string hex() const {
        char out[32] = {};
        sprintf(out, "%07lx%016lx", (uint64_t)(val >> 64), (uint64_t)val);
        return string(out);
    }
    void print() const {
        printf("%s\n", hex().c_str());
    }
};
