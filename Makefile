# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Informática Básica
#
#
# @author Noelia Perez Mojica
# @email alu0101710228@ull.edu.es
# @brief A Makefile template

CC = gcc
CFLAGS = -std=c11 -Wall
LDFLAGS =

TARGET = kilo
SRCS = kilo.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *~ a.out