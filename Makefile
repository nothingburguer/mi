CXX      = g++
CXXFLAGS = -O2 -Wall -Wextra -std=c++17 -Isrc/include
LDFLAGS  = -lncurses

PREFIX ?= /usr/local
BINDIR  = $(PREFIX)/bin

SRC = \
	src/main.cpp \
	src/editor.cpp \
	src/buffer.cpp \
	src/file.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = mi

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: $(TARGET)
	install -Dm755 $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm -f $(BINDIR)/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all install uninstall clean

