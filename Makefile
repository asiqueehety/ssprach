# Makefile for SPRACH Compiler
# German-inspired Programming Language
# Compiler phases: Lexical Analysis, Syntax Analysis, Semantic Analysis

CC = gcc
YACC = bison
LEX = flex
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

# Source files
LEXER_SRC = sprach.l
PARSER_SRC = sprach.y
SYMBOL_TABLE_SRC = symbol_table.c
SEMANTIC_SRC = semantic_analyzer.c

# Generated files
LEXER_C = lex.yy.c
PARSER_C = sprach.tab.c
PARSER_H = sprach.tab.h

# Object files
LEXER_OBJ = lex.yy.o
PARSER_OBJ = sprach.tab.o
SYMBOL_TABLE_OBJ = symbol_table.o
SEMANTIC_OBJ = semantic_analyzer.o

# Executable
EXECUTABLE = sprach

# Default target
all: $(EXECUTABLE)

# Main executable
$(EXECUTABLE): $(PARSER_C) $(LEXER_C) $(SYMBOL_TABLE_SRC) $(SEMANTIC_SRC)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(PARSER_OBJ)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(LEXER_OBJ)
	$(CC) $(CFLAGS) -c $(SYMBOL_TABLE_SRC) -o $(SYMBOL_TABLE_OBJ)
	$(CC) $(CFLAGS) -c $(SEMANTIC_SRC) -o $(SEMANTIC_OBJ)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(PARSER_OBJ) $(LEXER_OBJ) $(SYMBOL_TABLE_OBJ) $(SEMANTIC_OBJ) $(LDFLAGS)
	@echo "✓ Compilation successful!"

# Generate parser
$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	$(YACC) -d $(PARSER_SRC)
	@echo "✓ Parser generated"

# Generate lexer
$(LEXER_C): $(LEXER_SRC)
	$(LEX) $(LEXER_SRC)
	@echo "✓ Lexer generated"

# Clean build artifacts
clean:
	rm -f $(LEXER_C) $(PARSER_C) $(PARSER_H)
	rm -f *.o
	rm -f $(EXECUTABLE)
	@echo "✓ Clean complete"

# Run tests
test: all
	@echo "Running test with input.sprach..."
	./$(EXECUTABLE) < input.sprach
	@echo ""
	@echo "Running test with input.txt..."
	./$(EXECUTABLE) < input.txt

# Help
help:
	@echo "SPRACH Compiler - Build Commands"
	@echo "=================================="
	@echo "make          - Build the compiler"
	@echo "make clean    - Remove generated files"
	@echo "make test     - Run tests"
	@echo "make help     - Show this help message"

.PHONY: all clean test help
