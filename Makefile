CXX = g++
CXXFLAGS = -g -Wall -Werror -pedantic --std=c++2a 
CXXLINKFLAGS = -lraw

SRC = src
BIN = bin
TARGET = photo-worker
 
.PHONY: build
build: $(BIN)/$(TARGET)

.PHONY: run
run: $(BIN)/$(TARGET)
	$(BIN)/$(TARGET)

.PHONY: debug
debug: $(BIN)/$(TARGET)
	valgrind $(BIN)/$(TARGET)

.PHONY: clean
clean:
	rm -rf $(BIN)

# $^ all rules for dependency (after :)
# $< first parametr of dependency (before :)
# $@ target of rule (before :)
$(BIN)/$(TARGET): $(BIN)/main.o $(BIN)/argument_parse.o $(BIN)/convertor.o $(BIN)/pictures.o $(BIN)/pictureData.o $(BIN)/worker.o
	@mkdir -p $(BIN)
	$(CXX) $(CXXLINKFLAGS) $^ -o $@

$(BIN)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN)/main.o: $(SRC)/main.cpp $(SRC)/argument_parse.hpp $(SRC)/convertor.hpp $(SRC)/worker.hpp $(SRC)/pictures.hpp
$(BIN)/argument_parse.o: $(SRC)/argument_parse.cpp $(SRC)/argument_parse.hpp
$(BIN)/convertor.o: $(SRC)/convertor.cpp $(SRC)/convertor.hpp $(SRC)/pictures.hpp $(SRC)/pictureData.hpp
$(BIN)/pictures.o: $(SRC)/pictures.cpp $(SRC)/pictures.hpp $(SRC)/pictureData.hpp
$BIN)/pictureData.o: $(SRC)/pictureData.cpp $(SRC)/pictureData.hpp
$(BIN)/worker.o: $(SRC)/worker.cpp $(SRC)/worker.hpp
