CC_GCC = gcc

CFLAGS_GCC = -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-unused-parameter

SRCS = main.c

OBJS_GCC = $(SRCS:.c=.gcc.o)

TARGET_GCC = main_gcc

LIBS = -lpam -lpam_misc

.PHONY: all gcc clean

all: gcc

gcc: $(TARGET_GCC)

$(TARGET_GCC): $(OBJS_GCC)
	$(CC_GCC) $(CFLAGS_GCC) -o $@ $^ $(LIBS)

%.gcc.o: %.c
	$(CC_GCC) $(CFLAGS_GCC) -c $< -o $@

clean:
	rm -f $(OBJS_GCC) $(TARGET_GCC)
