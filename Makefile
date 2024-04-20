CXX = g++
CXXFLAGS = -g -Wall -pedantic --std=c++2a
CXXLINKFLAGS = -ltbb -lraw_r -lturbojpeg

SRC = src
BIN = bin
TEST = test
TARGET = photo-worker
 
.PHONY: build
build: CXXFLAGS += -Werror
build: $(BIN)/$(TARGET)

.PHONY: release
release: CXXFLAGS += -O3
release: $(BIN)/$(TARGET) 

.PHONY: run
run: $(BIN)/$(TARGET)
	$(BIN)/$(TARGET) convert -s $(TEST) -t 4

.PHONY: debug
debug: $(BIN)/$(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(BIN)/$(TARGET) -s $(TEST)

.PHONY: clean
clean:
	rm -rf $(BIN)
	rm -rf $(TEST)/jpg

.PHONY: tidy
tidy:
	clang-tidy -p . `find . -name '*.cpp'`

.PHONY: clang-check
clang-check:
	clang -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -fsyntax-only `find . -name '*.cpp'`

.PHONY: bear
bear:
	make clean
	bear -- make build -j

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
$(BIN)/pictureData.o: $(SRC)/pictureData.cpp $(SRC)/pictureData.hpp
$(BIN)/worker.o: $(SRC)/worker.cpp $(SRC)/worker.hpp
