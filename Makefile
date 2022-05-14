all:
	g++ src/main.cpp -o packer -std=c++17

install:
	cp packer /usr/bin/packer -f