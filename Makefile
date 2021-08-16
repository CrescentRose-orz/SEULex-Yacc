CXX		  := g++
CXXFLAGS = -std=c++17 -stdlib=libstdc++ -Wall -Werror -Wextra -pedantic  -o2 -fsanitize=address
LDFLAGS =  -fsanitize=address

LIBRARIES :=

LEX_BIN		:= src/SeuLex/bin
LEX_SRC		:= src/SeuLex/src
LEX_INCLUDE	:= src/SeuLex/include
LEX_LIB		:= src/SeuLex/lib
LEX_EXECUTABLE	:= Lex


YACC_BIN		:= src/SeuYacc/bin
YACC_SRC		:= src/SeuYacc/src
YACC_INCLUDE	:= src/SeuYacc/include
YACC_LIB		:= src/SeuYacc/lib
YACC_EXECUTABLE	:= Yacc

.PHONY:all
all: Lex Yacc
	

Lex: $(LEX_BIN)/$(LEX_EXECUTABLE)
$(LEX_BIN)/$(LEX_EXECUTABLE): $(LEX_SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(LEX_INCLUDE) -L$(LEX_LIB) $^ -o $@ $(LIBRARIES)

Yacc: $(YACC_BIN)/$(YACC_EXECUTABLE)
$(YACC_BIN)/$(YACC_EXECUTABLE): $(YACC_SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(YACC_INCLUDE) -L$(YACC_LIB) $^ -o $@ $(LIBRARIES)

.PHONY:clean
clean:
	install -d $(LEX_BIN)
	-rm -f $(LEX_BIN)/*
	install -d $(YACC_BIN)
	-rm -f $(YACC_BIN)/*