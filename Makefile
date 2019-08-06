
ifeq ($(MAKECMDGOALS), Test)
	DEBUG= -D DEBUG
else
	DEBUG=
endif

all: WS3

WS3: WS3.o data_decoder.o Observation.o network_utils.o
	g++ -o WS3 $(DEBUG) WS3.o data_decoder.o Observation.o network_utils.o -lusb-1.0 -lcurl -std=c++11

WS3.o: main.cpp main.h
	g++ -o WS3.o $(DEBUG) -c main.cpp -std=c++11

data_decoder.o: data_decoder.cpp data_decoder.h
	g++ -o data_decoder.o $(DEBUG) -c data_decoder.cpp -std=c++11

Observation.o: Observation.cpp Observation.h
	g++ -o Observation.o $(DEBUG) -c Observation.cpp -std=c++11

network_utils.o: network_utils.cpp network_utils.hpp json.hpp
	g++ -o network_utils.o $(DEBUG) -c network_utils.cpp -std=c++11

Test: all

clean:
	rm WS3 *.o