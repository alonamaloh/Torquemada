CXX      := c++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -I.

SRCDIR    := .
BUILDDIR  := build

SRCS    := board.cpp test.cpp
OBJS    := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))
TARGET  := $(BUILDDIR)/test

.PHONY: all clean

all: $(TARGET)

# ensure build directory exists
$(BUILDDIR):
	mkdir -p $@

# compile any .cpp → .o in build/
$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BUILDDIR)/*

