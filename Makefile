all: WS3

WS3: main.cpp main.h
	g++ -o WS3 main.cpp -lusb-1.0 -std=c++11

WS3_test: main.cpp main.h
	g++ -o Test -D DEBUG main.cpp -lusb-1.0 -std=c++11