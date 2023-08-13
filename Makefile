# Compiler
CC := x86_64-w64-mingw32-gcc

# Directories
SRCDIR := src
BINDIR := bin
LIBDIR := lib
INCDIR := include

# Files
EXECUTABLE_NAME := tcp_router.exe
SOURCES := $(wildcard $(SRCDIR)/config/*.c) $(wildcard $(SRCDIR)/database/*.c) $(wildcard $(SRCDIR)/model/*.c) $(wildcard $(SRCDIR)/service_manager/*.c) $(wildcard $(SRCDIR)/tcp_server/*.c) $(wildcard $(SRCDIR)/util/*.c) $(SRCDIR)/main.c
OBJECTS := $(SOURCES:.c=.o)

# Flags
CFLAGS := -I$(SRCDIR) -I$(INCDIR) -I$(LIBDIR) -L$(LIBDIR) -lmysqlclient -lws2_32

all: $(BINDIR)/$(EXECUTABLE_NAME)

$(BINDIR)/$(EXECUTABLE_NAME): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(CFLAGS)
	@echo "Compilation successful."

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@rm -f $(OBJECTS)
	@rm -f $(BINDIR)/$(EXECUTABLE_NAME)
	@echo "Cleanup complete."

