CC = g++
CFLAGS = -g -Wall -Werror -pedantic -lraw --std=c++2a 
 
.PHONY: build
.PHONY: clean

build: main.cpp argument_parse.cpp
	$(CC) $(CFLAGS) -o main main.cpp argument_parse.cpp
	#$(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
