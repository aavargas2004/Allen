NAME=../allc
SRC=AST/BaseTree.cpp AST/ArithmeticExpressions.cpp Exceptions.cpp main.cpp \
 Scope.cpp Types/Types.cpp AST/BooleanExpressions.cpp AST/ValueNodes.cpp AST/Statement.cpp
BISON=bison
FLEX=flex
CC=g++
INCLUDE=-I../include -I ../grammar
LIBS=-lfl
CFLAGS=-Wall -std=c++11
BISONSRCFILE=parser.tab.c
LEXSRCFILE=lex.yy.c
OBJS=$(SRC:.cpp=.o)
BISOBJ=$(BISONSRCFILE:.c=.o)
LEXOBJ=$(LEXSRCFILE:.c=.o)

all: $(SRC) $(BISONSRCFILE) $(LEXSRCFILE) $(NAME)

$(BISONSRCFILE): ../grammar/parser.y
	$(BISON) -d ../grammar/parser.y

$(LEXSRCFILE): ../grammar/lex.l
	$(FLEX) ../grammar/lex.l

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@ $(LIBS) 

$(NAME): $(OBJS) $(BISOBJ) $(LEXOBJ)
	$(CC) -o $@ $(OBJS) $(BISOBJ) $(LEXOBJ) $(LIBS)

clean:
	rm $(OBJS) $(BISOBJ) $(LEXOBJ) parser.tab.h $(BISONSRCFILE) $(LEXSRCFILE) $(NAME)


