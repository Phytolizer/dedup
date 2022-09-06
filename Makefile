SRCS := main.c str.c recdir.c
OBJS := $(SRCS:.c=.o)
EXE := dedup

CFLAGS := -Wall \
	-Wextra \
	-Wpedantic \
	-std=c11 \
	-ggdb \
	-Wmissing-prototypes \
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
