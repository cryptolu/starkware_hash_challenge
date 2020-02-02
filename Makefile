dist = 25
take = 92
CXX = clang++ -lstdc++

all: gmimc1 gmimc2 pos1

pos1: FORCE
	mkdir datapos1 || true
	$(CXX) -DDIST=$(dist) -DTAKE=$(take) -DMAIN_SIEVE poseidon.cpp -o pos1 -Ofast
	$(CXX) -DDIST=$(dist) -DTAKE=$(take) -DMAIN_LOCATE poseidon.cpp -o pos1locate -Ofast
	time ./pos1

gmimc1: FORCE
	mkdir datagmimc1 || true
	$(CXX) -DDIST=$(dist) -DTAKE=$(take) -DMAIN_SIEVE gmimc.cpp -o gmimc1 -Ofast
	$(CXX) -DDIST=$(dist) -DTAKE=$(take) -DMAIN_LOCATE gmimc.cpp -o gmimc1locate -Ofast
	time ./gmimc1


gmimc2: FORCE
	mkdir datagmimc2 || true
	$(CXX) -DDIST=$(dist) -DTAKE=$(take) -DMAIN_SIEVE -DHEAVY gmimc.cpp -o gmimc2 -Ofast
	$(CXX) -DDIST=$(dist) -DTAKE=$(take) -DMAIN_LOCATE -DHEAVY gmimc.cpp -o gmimc2locate -Ofast
	time ./gmimc2

pos1check: FORCE
	sort datapos1/* >alldatapos1
	python3 check.py ./alldatapos1 ./pos1locate

gmimc1check: FORCE
	sort datagmimc1/* >alldatagmimc1
	python3 check.py ./alldatagmimc1 ./gmimc1locate

gmimc2check: FORCE
	sort datagmimc2/* >alldatagmimc2
	python3 check.py ./alldatagmimc2 ./gmimc2locate

FORCE: