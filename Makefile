CC = g++
CFLAGS = -g -Wall -Werror -pedantic -lraw
 
.PHONY: build
.PHONY: clean

build: main.cpp argument_parse.cpp
	$(CC) $(CFLAGS) -o main main.cpp
	#$(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
