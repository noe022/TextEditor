# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Informática Básica
#
#
# @author Noelia Perez Mojica
# @email alu0101710228@ull.edu.es
# @brief A Makefile template

CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS =

TARGET = kilo
SRCS = kilo.cc
OBJS = $(SRCS:.cc=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET)
	./$(TARGET)

clean :
	rm -f $(OBJS) $(TARGET) *.d *~ a.out

-include $(OBJS:.o=.d)
