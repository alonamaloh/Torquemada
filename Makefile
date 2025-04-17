CXX      := c++
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra -pedantic -I. -MMD -MP

SRCDIR    := .
BUILDDIR  := build

SRCS      := board.cpp test.cpp
OBJS      := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS      := $(OBJS:.o=.d)

TARGET    := $(BUILDDIR)/test

.PHONY: all clean

all: $(TARGET)

# make sure build/ exists
$(BUILDDIR):
	mkdir -p $@

# compile .cpp → .o and emit .d
$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# include the auto‑generated dependency files (ignore if missing)
-include $(DEPS)

clean:
	rm -rf $(BUILDDIR)/*
