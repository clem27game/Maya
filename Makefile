
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = main
TARGET_DEBUG = main-debug
SOURCE = main.c

# Règle par défaut
all: $(TARGET)

# Compilation normale
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -O2 -o $(TARGET) $(SOURCE) -lm -ldl

# Compilation avec debug
$(TARGET_DEBUG): $(SOURCE)
	$(CC) $(CFLAGS) -DDEBUG -o $(TARGET_DEBUG) $(SOURCE) -lm -ldl

# Nettoyage
clean:
	rm -f $(TARGET) $(TARGET_DEBUG) *.o

# Exécution avec un fichier test
test: $(TARGET)
	./$(TARGET) test.my

# Règles phony
.PHONY: all clean test

# Règle par défaut pour make sans arguments
.DEFAULT_GOAL := all
