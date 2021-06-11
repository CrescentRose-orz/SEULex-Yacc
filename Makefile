CXX		  := g++
CXXFLAGS = -std=c++17 -stdlib=libstdc++ -Wall -Werror -Wextra -pedantic  -o2 -fsanitize=address
LDFLAGS =  -fsanitize=address

BIN		:= src/SeuLex/bin
SRC		:= src/SeuLex/src
INCLUDE	:= src/SeuLex/include
LIB		:= src/SeuLex/lib

LIBRARIES	:=
EXECUTABLE	:= Lex


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	install -d $(BIN)
	-rm -f $(BIN)/*