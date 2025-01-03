CC = gcc
# Compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -MD   tells C preprocessor to write dependency files for headers
CFLAGS = -g -Wall -MD
OBJECTS = main.o chunk.o
TARGET = lox
# Auto create obj list from c files, 
# isn't playing nicely with my folder structure though
# SRCS=$(wildcard src/*.c)
# OBJECTS=$(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Define a pattern rule that compiles every .c file into a .o file
# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.d $(TARGET)

# Handle dependency files created for headers
-include $(OBJECTS:.o=.d)