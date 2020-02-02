#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

#include "arithmetic.cpp"

const char * hexc(__uint128_t x) {
	string s = F(x).hex();
	char *buf = (char*) malloc(40);
	assert(buf);
	strcpy(buf, s.c_str());
    return buf;
}