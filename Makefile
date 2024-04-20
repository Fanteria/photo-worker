PROJECT = photo-worker

CXX = g++
CXXFLAGS = -g -Wall -pedantic --std=c++2a
CXXFLAGS += -DPROJECT_NAME=\"$(PROJECT)\"
CXXLINKFLAGS = -ltbb -lraw_r -lturbojpeg

SRC = src
BIN = bin
TEST = test
 
.PHONY: build
build: CXXFLAGS += -Werror
build: $(BIN)/$(PROJECT)

.PHONY: release
release: CXXFLAGS += -O3 -DVERSION=\"$(shell git describe)\"
release: $(BIN)/$(PROJECT) 

.PHONY: run
run: $(BIN)/$(PROJECT)
	$(BIN)/$(PROJECT) convert -s $(TEST) -t 4

.PHONY: debug
debug: $(BIN)/$(PROJECT)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $(BIN)/$(PROJECT) -s $(TEST)

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

.PHONY: format
format:
	clang-format -i `find . -name '*.cpp' \
		-o -name '*.c' \
		-o -name '*.cc' \
		-o -name '*.cxx' \
		-o -name '*.hpp' \
		-o -name '*.h' \
		-o -name '*.hh' \
		-o -name '*.hxx'`

# $^ all rules for dependency (after :)
# $< first parametr of dependency (before :)
# $@ target of rule (before :)
$(BIN)/$(PROJECT): $(BIN)/main.o $(BIN)/argument_parse.o $(BIN)/convertor.o $(BIN)/pictures.o $(BIN)/pictureData.o $(BIN)/worker.o
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
