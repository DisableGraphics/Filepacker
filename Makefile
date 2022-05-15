all:
	g++ src/main.cpp -o filepacker -std=c++17

install:
	cp filepacker /usr/bin/filepacker -f
	
setup-vscode:
	bear -- make