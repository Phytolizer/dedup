SRCS := main.c str.c recdir.c
OBJS := $(SRCS:.c=.o)
EXE := dedup

CFLAGS := -Wall \
	-Wextra \
	-Wpedantic \
	-Wmissing-prototypes \
	-Wno-unused-function \
	-std=c11 \
	-ggdb \
	-fsanitize=address,undefined \
	-D_DEFAULT_SOURCE \
	-fno-omit-frame-pointer

LDFLAGS := -fsanitize=address,undefined

.PHONY: all clean

all: $(EXE)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(EXE)
